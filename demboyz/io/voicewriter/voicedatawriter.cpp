
#include "../idemowriter.h"
#include "netmessages/netmessages.h"

#include "netmessages/svc_voiceinit.h"
#include "netmessages/svc_voicedata.h"

#include "demmessages/dem_stringtables.h"
#include "demofile/demofile.h"

#include <cassert>

#include "wavfilewriter.h"

#include "ivoicecodecmanager.h"

#include <filesystem>
#include <fstream> // Include for file operations
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <map>

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
    std::map<int, std::string> m_playerNames;

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

    if (packet.cmd == dem_stringtables)
    {
        DemMsg::Dem_StringTables* stringTables = (DemMsg::Dem_StringTables*)packet.data;
        for (const DemMsg::Dem_StringTables::StringTable& table : stringTables->stringtables)
        {
            if (!strcmp(table.tableName.c_str(), "userinfo"))
            {
                int index_counter = 0;
                for (const DemMsg::Dem_StringTables::StringTableEntry& entry : table.entries)
                {
                    const char* nameUtf8 = (const char*)entry.data.begin();
                    if (nameUtf8) // Check if nameUtf8 is not null
                    {
                        m_playerNames[index_counter] = std::string(nameUtf8);
                    }
                    index_counter++;
                }
            }
        }
    }

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
        }

        // Check if we need to start a new WAV file
        if (state.lastVoiceDataTick == -1 || m_curTick - state.lastVoiceDataTick >= 300)
        {
            // Close the previous WAV file if open
            state.wavWriter.Close();

            int sampleRate = mVoiceCodecManager->GetSampleRate();

            // Create directory for the client
            std::string clientDir = std::string(m_outputPath) + "/client_" + std::to_string((uint32_t)voiceData->fromClientIndex);
            std::filesystem::create_directories(clientDir);

            // Write the name.txt file with the player's name
            std::string nameFilePath = clientDir + "/name.txt";
            std::ofstream nameFile(nameFilePath);
            if (nameFile.is_open())
            {
                nameFile << m_playerNames[int(voiceData->fromClientIndex)] << "\n";
                nameFile.close();
            }

            // Init new output file with tick number
            std::string fileName = clientDir + "/" + std::to_string(m_curTick) + ".wav";
            state.wavWriter.Init(fileName.c_str(), sampleRate);
        }

        assert((voiceData->dataLengthInBits % 8) == 0);
        const int numBytes = voiceData->dataLengthInBits / 8;

        const int numBytesWritten = state.voiceDecoder->Decompress(voiceData->data.get(), numBytes, (uint8_t*)m_decodeBuffer, sizeof(m_decodeBuffer));
        const int numDecompressedSamples = numBytesWritten / sizeof(int16_t);

        state.wavWriter.WriteSamples(m_decodeBuffer, numDecompressedSamples);

        // Update last voice data tick
        state.lastVoiceDataTick = m_curTick;
    }
}
