
#include "../idemowriter.h"
#include "netmessages/netmessages.h"

#include "netmessages/svc_voiceinit.h"
#include "netmessages/svc_voicedata.h"

#include "celt/celt.h"

#include <cassert>

#include "wavfilewriter.h"

#define USE_VAUDIO_CELT
#define MAX_PLAYERS 33

#ifdef USE_VAUDIO_CELT
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

struct CeltConfig
{
    celt_int32 sampleRate;
    uint32_t frameSizeSamples;
    uint32_t encodedFrameSizeBytes;
};

static CeltConfig sCeltConfigs[] =
{
    { 44100, 256, 120 },    // unused
    { 22050, 120, 60 },     // unused
    { 22050, 256, 60 },     // unused
    { 22050, 512, 64 },     // vaudio_celt
    { 44100, 1024, 128 }    // vaudio_celt_high
};

#ifdef USE_VAUDIO_CELT

class IVoiceCodec
{
protected:
    virtual ~IVoiceCodec() {}

public:
    // Initialize the object. The uncompressed format is always 8-bit signed mono.
    virtual bool Init( int quality ) = 0;

    // Use this to delete the object.
    virtual void Release() = 0;

    // Compress the voice data.
    // pUncompressed        -   16-bit signed mono voice data.
    // maxCompressedBytes   -   The length of the pCompressed buffer. Don't exceed this.
    // bFinal               -   Set to true on the last call to Compress (the user stopped talking).
    //                          Some codecs like big block sizes and will hang onto data you give them in Compress calls.
    //                          When you call with bFinal, the codec will give you compressed data no matter what.
    // Return the number of bytes you filled into pCompressed.
    virtual int Compress(const char *pUncompressed, int nSamples, char *pCompressed, int maxCompressedBytes, bool bFinal) = 0;

    // Decompress voice data. pUncompressed is 16-bit signed mono.
    virtual int Decompress(const char *pCompressed, int compressedBytes, char *pUncompressed, int maxUncompressedBytes) = 0;

    // Some codecs maintain state between Compress and Decompress calls. This should clear that state.
    virtual bool ResetState() = 0;
};

typedef void* (CreateInterfaceFn)(const char *pName, int *pReturnCode);
static HINSTANCE celtDll;
static CreateInterfaceFn* createInterfaceFunc;

#else

class CeltVoiceDecoder
{
public:
    bool DoInit(CELTMode* celtMode, uint32_t frameSizeSamples, uint32_t encodedFrameSizeBytes)
    {
        if(m_celtDecoder)
        {
            return false;
        }

        int error = CELT_OK;
        m_celtDecoder = celt_decoder_create_custom(celtMode, sCeltChannels, &error);
        assert(error == CELT_OK);
        assert(m_celtDecoder);

        m_frameSizeSamples = frameSizeSamples;
        m_encodedFrameSizeBytes = encodedFrameSizeBytes;
        return true;
    }

    void Destroy()
    {
        celt_decoder_destroy(m_celtDecoder);
        m_celtDecoder = NULL;
    }

    void Reset()
    {
    }

    int Decompress(
        const uint8_t* compressedData,
        int compressedBytes,
        int16_t* uncompressedData,
        int maxUncompressedSamples)
    {
        int curCompressedByte = 0;
        int curDecompressedSample = 0;

        const uint32_t encodedframeSizeBytes = m_encodedFrameSizeBytes;
        const uint32_t frameSizeSamples = m_frameSizeSamples;
        while(
            ((compressedBytes - curCompressedByte) >= encodedframeSizeBytes) &&
            ((maxUncompressedSamples - curDecompressedSample) >= frameSizeSamples))
        {
            DecodeFrame(&compressedData[curCompressedByte], &uncompressedData[curDecompressedSample]);
            curCompressedByte += encodedframeSizeBytes;
            curDecompressedSample += frameSizeSamples;
        }
        return curDecompressedSample;
    }

private:
    void DecodeFrame(const uint8_t* compressedData, int16_t* uncompressedData)
    {
        int error = celt_decode(m_celtDecoder, compressedData, m_encodedFrameSizeBytes, uncompressedData, m_frameSizeSamples);
        assert(error >= CELT_OK);
    }

private:
    static const int sCeltChannels = 1;

private:
    CELTDecoder* m_celtDecoder = NULL;
    uint32_t m_frameSizeSamples;
    uint32_t m_encodedFrameSizeBytes;
};

#endif // USE_VAUDIO_CELT

class VoiceDataWriter: public IDemoWriter
{
public:
    VoiceDataWriter(const char* outputPath);

    virtual void StartWriting(demoheader_t& header) override final;
    virtual void EndWriting() override final;

    virtual void StartCommandPacket(const CommandPacket& packet) override final;
    virtual void EndCommandPacket(const PacketTrailingBits& trailingBits) override final;

    virtual void WriteNetPacket(NetPacket& packet, SourceGameContext& context) override final;

private:
    struct PlayerVoiceState
    {
#ifdef USE_VAUDIO_CELT
        IVoiceCodec* celtDecoder;
#else
        CeltVoiceDecoder decoder;
#endif
        WaveFileWriter wavWriter;
        int32_t lastVoiceDataTick = -1;
    };

private:
    CELTMode* m_celtMode;
    PlayerVoiceState m_playerVoiceStates[MAX_PLAYERS];

    int32_t m_curTick;
    const char* m_outputPath;

    int16_t m_decodeBuffer[8192];

    static const int sQuality = 3;
};

IDemoWriter* IDemoWriter::CreateVoiceDataWriter(const char* outputPath)
{
    return new VoiceDataWriter(outputPath);
}

VoiceDataWriter::VoiceDataWriter(const char* outputPath):
    m_celtMode(nullptr),
    m_playerVoiceStates(),
    m_curTick(-1),
    m_outputPath(outputPath)
{
}

void VoiceDataWriter::StartWriting(demoheader_t& header)
{
#ifdef USE_VAUDIO_CELT
    celtDll = LoadLibrary(TEXT("vaudio_celt.dll"));
    createInterfaceFunc = (CreateInterfaceFn*)GetProcAddress(celtDll, "CreateInterface");
#else
    int error = CELT_OK;
    const CeltConfig& config = sCeltConfigs[sQuality];
    m_celtMode = celt_mode_create(config.sampleRate, config.frameSizeSamples, &error);
    assert(error == CELT_OK);
    assert(m_celtMode);
#endif
}

void VoiceDataWriter::EndWriting()
{
    for(PlayerVoiceState& state : m_playerVoiceStates)
    {
#ifdef USE_VAUDIO_CELT
        if(state.celtDecoder)
        {
            state.celtDecoder->Release();
        }
#else
        state.decoder.Destroy();
#endif
        state.wavWriter.Close();
        state.lastVoiceDataTick = -1;
    }
#ifndef USE_VAUDIO_CELT
    if(m_celtMode)
    {
        celt_mode_destroy(m_celtMode);
        m_celtMode = nullptr;
    }
#endif
}

void VoiceDataWriter::StartCommandPacket(const CommandPacket& packet)
{
    m_curTick = packet.tick;
}

void VoiceDataWriter::EndCommandPacket(const PacketTrailingBits& trailingBits)
{
}

void VoiceDataWriter::WriteNetPacket(NetPacket& packet, SourceGameContext& context)
{
    if(packet.type == NetMsg::svc_VoiceInit)
    {
        NetMsg::SVC_VoiceInit* voiceInit = static_cast<NetMsg::SVC_VoiceInit*>(packet.data);
        assert(!strcmp(voiceInit->voiceCodec, "vaudio_celt"));
        assert(voiceInit->quality == NetMsg::SVC_VoiceInit::QUALITY_HAS_SAMPLE_RATE);
        assert(voiceInit->sampleRate == sCeltConfigs[sQuality].sampleRate);
    }
    else if(packet.type == NetMsg::svc_VoiceData)
    {
        NetMsg::SVC_VoiceData* voiceData = static_cast<NetMsg::SVC_VoiceData*>(packet.data);
        assert(voiceData->fromClientIndex < MAX_PLAYERS);

        PlayerVoiceState& state = m_playerVoiceStates[voiceData->fromClientIndex];

        const CeltConfig& config = sCeltConfigs[sQuality];
#ifdef USE_VAUDIO_CELT
        const bool initWavWriter = !state.celtDecoder;
        if(!state.celtDecoder)
        {
            int ret = 0;
            state.celtDecoder = static_cast<IVoiceCodec*>(createInterfaceFunc("vaudio_celt", &ret));
            state.celtDecoder->Init(sQuality);
        }
#else
        const bool initWavWriter = state.decoder.DoInit(m_celtMode, config.frameSizeSamples, config.encodedFrameSizeBytes);
#endif
        if(initWavWriter)
        {
            std::string name = std::string(m_outputPath) + "/client_" + std::to_string((uint32_t)voiceData->fromClientIndex) + ".wav";
            state.wavWriter.Init(name.c_str(), config.sampleRate);
            assert(state.lastVoiceDataTick == -1);
            state.lastVoiceDataTick = m_curTick;
        }

        assert((voiceData->dataLengthInBits % 8) == 0);
        const int numBytes = voiceData->dataLengthInBits / 8;

#ifdef USE_VAUDIO_CELT
        const int numDecompressedSamples = state.celtDecoder->Decompress((const char*)voiceData->data.get(), numBytes, (char*)m_decodeBuffer, 8192*2);
#else
        const int numDecompressedSamples = state.decoder.Decompress(voiceData->data.get(), numBytes, m_decodeBuffer, 8192);
#endif
        state.wavWriter.WriteSamples(m_decodeBuffer, numDecompressedSamples);

        state.lastVoiceDataTick = m_curTick;
    }
}
