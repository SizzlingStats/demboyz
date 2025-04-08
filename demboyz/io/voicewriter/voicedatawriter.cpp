
#include "../idemowriter.h"
#include "netmessages/netmessages.h"

#include "netmessages/net_setconvar.h"
#include "netmessages/svc_voiceinit.h"
#include "netmessages/svc_voicedata.h"

#include "demofile/demotypes.h"

#include <cassert>
#include <cstring>

#include "wavfilewriter.h"

#include "ivoicecodecmanager.h"

#define MAX_PLAYERS 101

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
        IVoiceCodec* voiceDecoder = nullptr;

        WaveFileWriter wavWriter;
    };

private:
    IVoiceCodecManager* mVoiceCodecManager;
    PlayerVoiceState m_playerVoiceStates[MAX_PLAYERS];

    const char* m_outputPath;
    VoiceCodec m_voiceInitCodec;

    int16_t m_decodeBuffer[22528];
};

IDemoWriter* IDemoWriter::CreateVoiceDataWriter(const char* outputPath)
{
    return new VoiceDataWriter(outputPath);
}

VoiceDataWriter::VoiceDataWriter(const char* outputPath):
    mVoiceCodecManager(nullptr),
    m_playerVoiceStates(),
    m_outputPath(outputPath),
    m_voiceInitCodec(VoiceCodec::Unknown)
{
}

void VoiceDataWriter::StartWriting(demoheader_t& header)
{
}

void VoiceDataWriter::EndWriting()
{
    for(PlayerVoiceState& state : m_playerVoiceStates)
    {
        if (state.voiceDecoder)
        {
            state.voiceDecoder->Destroy();
            delete state.voiceDecoder;
            state.voiceDecoder = nullptr;
        }

        state.wavWriter.Close();
    }

    if (mVoiceCodecManager)
    {
        mVoiceCodecManager->Destroy();
        delete mVoiceCodecManager;
        mVoiceCodecManager = nullptr;
    }
}

void VoiceDataWriter::StartCommandPacket(const CommandPacket& packet)
{
}

void VoiceDataWriter::EndCommandPacket(const PacketTrailingBits& trailingBits)
{
}

static VoiceCodec CodecFromString(const char* codec)
{
    if (!strcmp(codec, "steam"))
    {
        return VoiceCodec::Steam;
    }
    else if (!strcmp(codec, "vaudio_speex"))
    {
        return VoiceCodec::Speex;
    }
    else if (!strcmp(codec, "vaudio_celt"))
    {
        return VoiceCodec::Celt;
    }
    else if (!strcmp(codec, "vaudio_celt_high"))
    {
        return VoiceCodec::Celt_High;
    }
    return VoiceCodec::Unknown;
}

void VoiceDataWriter::WriteNetPacket(NetPacket& packet, SourceGameContext& context)
{
    if (packet.type == NetMsg::svc_VoiceInit)
    {
        NetMsg::SVC_VoiceInit* voiceInit = static_cast<NetMsg::SVC_VoiceInit*>(packet.data);
        m_voiceInitCodec = CodecFromString(voiceInit->voiceCodec);
    }
    else if(packet.type == NetMsg::svc_VoiceData)
    {
        NetMsg::SVC_VoiceData* voiceData = static_cast<NetMsg::SVC_VoiceData*>(packet.data);
        assert(voiceData->fromClientIndex < MAX_PLAYERS);

        if (voiceData->dataLengthInBits == 0)
        {
            return;
        }

        assert((voiceData->dataLengthInBits % 8) == 0);
        const int numCompressedBytes = voiceData->dataLengthInBits / 8;
        const uint8_t* compressedData = voiceData->data.get();

        PlayerVoiceState& state = m_playerVoiceStates[voiceData->fromClientIndex];
        if (!state.voiceDecoder)
        {
            if (!mVoiceCodecManager)
            {
                mVoiceCodecManager = IVoiceCodecManager::Create(m_voiceInitCodec, compressedData, numCompressedBytes);
                assert(mVoiceCodecManager);

                const bool result = mVoiceCodecManager->Init();
                assert(result);
            }

            state.voiceDecoder = mVoiceCodecManager->CreateVoiceCodec();
            state.voiceDecoder->Init();

            int sampleRate = mVoiceCodecManager->GetSampleRate();

            // Init output file
            std::string name = std::string(m_outputPath) + "/client_" + std::to_string((uint32_t)voiceData->fromClientIndex) + ".wav";
            state.wavWriter.Init(name.c_str(), sampleRate);
        }

        const uint32_t numBytesWritten = state.voiceDecoder->Decompress(compressedData, numCompressedBytes, (uint8_t*)m_decodeBuffer, sizeof(m_decodeBuffer));
        const uint32_t numDecompressedSamples = numBytesWritten / sizeof(int16_t);

        state.wavWriter.WriteSamples(m_decodeBuffer, numDecompressedSamples);
    }
}
