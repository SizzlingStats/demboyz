
#include "../idemowriter.h"
#include "netmessages/netmessages.h"

#include "netmessages/svc_voiceinit.h"
#include "netmessages/svc_voicedata.h"

#include "demofile/demotypes.h"

#include <cassert>

#include "wavfilewriter.h"

#include "ivoicecodecmanager.h"

#define MAX_PLAYERS 65

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
        int32_t lastVoiceDataTick = -1;
    };

private:
    IVoiceCodecManager* mVoiceCodecManager;
    PlayerVoiceState m_playerVoiceStates[MAX_PLAYERS];

    int32_t m_curTick;
    const char* m_outputPath;

    int16_t m_decodeBuffer[22528];
};

IDemoWriter* IDemoWriter::CreateVoiceDataWriter(const char* outputPath)
{
    return new VoiceDataWriter(outputPath);
}

VoiceDataWriter::VoiceDataWriter(const char* outputPath):
    mVoiceCodecManager(nullptr),
    m_playerVoiceStates(),
    m_curTick(0),
    m_outputPath(outputPath)
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
        state.lastVoiceDataTick = -1;
    }

    mVoiceCodecManager->Destroy();
    delete mVoiceCodecManager;
    mVoiceCodecManager = nullptr;
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

        assert(!mVoiceCodecManager);
        mVoiceCodecManager = IVoiceCodecManager::Create(voiceInit->voiceCodec);
        assert(mVoiceCodecManager);

        bool result = mVoiceCodecManager->Init(voiceInit->quality, voiceInit->sampleRate);
        assert(result);
    }
    else if(packet.type == NetMsg::svc_VoiceData)
    {
        NetMsg::SVC_VoiceData* voiceData = static_cast<NetMsg::SVC_VoiceData*>(packet.data);
        assert(voiceData->fromClientIndex < MAX_PLAYERS);

        PlayerVoiceState& state = m_playerVoiceStates[voiceData->fromClientIndex];
        if (!state.voiceDecoder)
        {
            state.voiceDecoder = mVoiceCodecManager->CreateVoiceCodec();
            state.voiceDecoder->Init();

            int sampleRate = mVoiceCodecManager->GetSampleRate();

            // Init output file
            std::string name = std::string(m_outputPath) + "/client_" + std::to_string((uint32_t)voiceData->fromClientIndex) + ".wav";
            state.wavWriter.Init(name.c_str(), sampleRate);
            assert(state.lastVoiceDataTick == -1);
            state.lastVoiceDataTick = m_curTick;
        }

        assert((voiceData->dataLengthInBits % 8) == 0);
        const int numBytes = voiceData->dataLengthInBits / 8;

        const int numBytesWritten = state.voiceDecoder->Decompress(voiceData->data.get(), numBytes, (uint8_t*)m_decodeBuffer, sizeof(m_decodeBuffer));
        const int numDecompressedSamples = numBytesWritten / sizeof(int16_t);

        state.wavWriter.WriteSamples(m_decodeBuffer, numDecompressedSamples);

        state.lastVoiceDataTick = m_curTick;
    }
}
