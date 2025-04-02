
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
        int32_t lastVoiceDataTick = -1;
    };

private:
    IVoiceCodecManager* mVoiceCodecManager;
    PlayerVoiceState m_playerVoiceStates[MAX_PLAYERS];

    int32_t m_curTick;
    const char* m_outputPath;

    // If true, override to use steam voice
    // when given a valid SVC_VoiceInit quality value.
    bool m_bSVUseSteamVoice;

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
    m_outputPath(outputPath),
    m_bSVUseSteamVoice(false)
{
}

void VoiceDataWriter::StartWriting(demoheader_t& header)
{
    // Steam voice codec and cvar sv_use_steam_voice released during network protocol 15.
    // We can't trust the SVC_VoiceInit codec on this protocol because we
    // don't know if the demo was recorded before or after the steam voice update.
    if (header.networkprotocol >= 16)
    {
        // sv_use_steam_voice defaulted to true on release and
        // we can guarantee that value on network protocol 16 or later.
        m_bSVUseSteamVoice = true;
    }
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
    if (packet.type == NetMsg::net_SetConVar)
    {
        NetMsg::Net_SetConVar* setConvar = static_cast<NetMsg::Net_SetConVar*>(packet.data);
        for (const NetMsg::Net_SetConVar::cvar_t& cvar : setConvar->cvars)
        {
            if (!strcmp(cvar.name, "sv_use_steam_voice") && !strcmp(cvar.value, "0"))
            {
                m_bSVUseSteamVoice = false;
            }
        }
    }
    else if (packet.type == NetMsg::svc_VoiceInit)
    {
        assert(!mVoiceCodecManager);

        NetMsg::SVC_VoiceInit* voiceInit = static_cast<NetMsg::SVC_VoiceInit*>(packet.data);

        const char* codec = voiceInit->voiceCodec;
        int32_t sampleRate = voiceInit->sampleRate;
        if (voiceInit->quality != NetMsg::SVC_VoiceInit::QUALITY_HAS_SAMPLE_RATE)
        {
            if (m_bSVUseSteamVoice)
            {
                codec = "steam";
                sampleRate = 0;
            }
            else if (!strcmp(codec, "vaudio_celt"))
            {
                sampleRate = 22050;
            }
            else
            {
                sampleRate = 11025;
            }
        }

        mVoiceCodecManager = IVoiceCodecManager::Create(codec);
        assert(mVoiceCodecManager);

        bool result = mVoiceCodecManager->Init(voiceInit->quality, sampleRate);
        assert(result);
    }
    else if(packet.type == NetMsg::svc_VoiceData)
    {
        NetMsg::SVC_VoiceData* voiceData = static_cast<NetMsg::SVC_VoiceData*>(packet.data);
        assert(voiceData->fromClientIndex < MAX_PLAYERS);

        if (voiceData->dataLengthInBits > 0)
        {
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
}
