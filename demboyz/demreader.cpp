
#include "demoreader.h"
#include "idemowriter.h"
#include "demofile.h"
#include "demotypes.h"

#include "netmessages/netmessages.h"
#include "netmessages/netcontants.h"
#include "sourcesdk/bitbuf.h"
#include <vector>
#include <cstdint>

static void* netDataStructs[32];

static void CreateNetMsgStructs()
{
    netDataStructs[0] = new NetMsg::Net_NOP();
    netDataStructs[1] = new NetMsg::Net_Disconnect();
    netDataStructs[2] = new NetMsg::Net_File();
    netDataStructs[3] = new NetMsg::Net_Tick();
    netDataStructs[4] = new NetMsg::Net_StringCmd();
    netDataStructs[5] = new NetMsg::Net_SetConVar();
    netDataStructs[6] = new NetMsg::Net_SignonState();
    netDataStructs[7] = new NetMsg::SVC_Print();
    netDataStructs[8] = new NetMsg::SVC_ServerInfo();
    netDataStructs[9] = new NetMsg::SVC_SendTable();
    netDataStructs[10] = new NetMsg::SVC_ClassInfo();
    netDataStructs[11] = new NetMsg::SVC_SetPause();
    netDataStructs[12] = new NetMsg::SVC_CreateStringTable();
    netDataStructs[13] = new NetMsg::SVC_UpdateStringTable();
    netDataStructs[14] = new NetMsg::SVC_VoiceInit();
    netDataStructs[15] = new NetMsg::SVC_VoiceData();
    netDataStructs[16] = new NetMsg::SVC_HLTV();
    netDataStructs[17] = new NetMsg::SVC_Sounds();
    netDataStructs[18] = new NetMsg::SVC_SetView();
    netDataStructs[19] = new NetMsg::SVC_FixAngle();
    netDataStructs[20] = new NetMsg::SVC_CrosshairAngle();
    netDataStructs[21] = new NetMsg::SVC_BSPDecal();
    netDataStructs[22] = new NetMsg::SVC_TerrainMod();
    netDataStructs[23] = new NetMsg::SVC_UserMessage();
    netDataStructs[24] = new NetMsg::SVC_EntityMessage();
    netDataStructs[25] = new NetMsg::SVC_GameEvent();
    netDataStructs[26] = new NetMsg::SVC_PacketEntities();
    netDataStructs[27] = new NetMsg::SVC_TempEntities();
    netDataStructs[28] = new NetMsg::SVC_Prefetch();
    netDataStructs[29] = new NetMsg::SVC_Menu();
    netDataStructs[30] = new NetMsg::SVC_GameEventList();
    netDataStructs[31] = new NetMsg::SVC_GetCvarValue();
}

static void DestroyNetMsgStructs()
{
    delete reinterpret_cast<NetMsg::Net_NOP*>(netDataStructs[0]);
    delete reinterpret_cast<NetMsg::Net_Disconnect*>(netDataStructs[1]);
    delete reinterpret_cast<NetMsg::Net_File*>(netDataStructs[2]);
    delete reinterpret_cast<NetMsg::Net_Tick*>(netDataStructs[3]);
    delete reinterpret_cast<NetMsg::Net_StringCmd*>(netDataStructs[4]);
    delete reinterpret_cast<NetMsg::Net_SetConVar*>(netDataStructs[5]);
    delete reinterpret_cast<NetMsg::Net_SignonState*>(netDataStructs[6]);
    delete reinterpret_cast<NetMsg::SVC_Print*>(netDataStructs[7]);
    delete reinterpret_cast<NetMsg::SVC_ServerInfo*>(netDataStructs[8]);
    delete reinterpret_cast<NetMsg::SVC_SendTable*>(netDataStructs[9]);
    delete reinterpret_cast<NetMsg::SVC_ClassInfo*>(netDataStructs[10]);
    delete reinterpret_cast<NetMsg::SVC_SetPause*>(netDataStructs[11]);
    delete reinterpret_cast<NetMsg::SVC_CreateStringTable*>(netDataStructs[12]);
    delete reinterpret_cast<NetMsg::SVC_UpdateStringTable*>(netDataStructs[13]);
    delete reinterpret_cast<NetMsg::SVC_VoiceInit*>(netDataStructs[14]);
    delete reinterpret_cast<NetMsg::SVC_VoiceData*>(netDataStructs[15]);
    delete reinterpret_cast<NetMsg::SVC_HLTV*>(netDataStructs[16]);
    delete reinterpret_cast<NetMsg::SVC_Sounds*>(netDataStructs[17]);
    delete reinterpret_cast<NetMsg::SVC_SetView*>(netDataStructs[18]);
    delete reinterpret_cast<NetMsg::SVC_FixAngle*>(netDataStructs[19]);
    delete reinterpret_cast<NetMsg::SVC_CrosshairAngle*>(netDataStructs[20]);
    delete reinterpret_cast<NetMsg::SVC_BSPDecal*>(netDataStructs[21]);
    delete reinterpret_cast<NetMsg::SVC_TerrainMod*>(netDataStructs[22]);
    delete reinterpret_cast<NetMsg::SVC_UserMessage*>(netDataStructs[23]);
    delete reinterpret_cast<NetMsg::SVC_EntityMessage*>(netDataStructs[24]);
    delete reinterpret_cast<NetMsg::SVC_GameEvent*>(netDataStructs[25]);
    delete reinterpret_cast<NetMsg::SVC_PacketEntities*>(netDataStructs[26]);
    delete reinterpret_cast<NetMsg::SVC_TempEntities*>(netDataStructs[27]);
    delete reinterpret_cast<NetMsg::SVC_Prefetch*>(netDataStructs[28]);
    delete reinterpret_cast<NetMsg::SVC_Menu*>(netDataStructs[29]);
    delete reinterpret_cast<NetMsg::SVC_GameEventList*>(netDataStructs[30]);
    delete reinterpret_cast<NetMsg::SVC_GetCvarValue*>(netDataStructs[31]);
}

void ParsePacket(uint8_t* packet, size_t length, SourceGameContext& context, IDemoWriter* writer)
{
    assert(length <= NET_MAX_PAYLOAD);
    bf_read bitbuf(packet, length);
    NetPacket netPacket;
    while (bitbuf.GetNumBitsLeft() >= NETMSG_TYPE_BITS)
    {
        netPacket.type = bitbuf.ReadUBitLong(NETMSG_TYPE_BITS);
        netPacket.data = netDataStructs[netPacket.type];
        NetHandlers::NetMsg_BitRead(netPacket.type, bitbuf, context, netPacket.data);
        writer->WriteNetPacket(netPacket, context);
    }
}

void ReadStringTable(bf_read& bitbuf)
{
    std::vector<uint8_t> data;
    size_t numEntries = bitbuf.ReadWord();
    char entryName[4096];
    while (numEntries--)
    {
        bitbuf.ReadString(entryName, sizeof(entryName));
        printf("%s ", entryName);
        if (bitbuf.ReadOneBit() != 0)
        {
            size_t numDataBytes = bitbuf.ReadWord();
            data.resize(numDataBytes);
            bitbuf.ReadBytes(data.data(), numDataBytes);
            printf("bytes %i ", numDataBytes);
        }
        printf("\n");
    }
    if (bitbuf.ReadOneBit() == 0)
    {
        return;
    }
    while (numEntries--)
    {
        bitbuf.ReadString(entryName, sizeof(entryName));
        printf("%s ", entryName);
        if (bitbuf.ReadOneBit() != 0)
        {
            size_t numDataBytes = bitbuf.ReadWord();
            data.resize(numDataBytes);
            bitbuf.ReadBytes(data.data(), numDataBytes);
            printf("bytes %i ", numDataBytes);
        }
        printf("\n");
    }
}

void ParseStringTables(uint8_t* stringtables, size_t length, SourceGameContext& context, IDemoWriter* writer)
{
    bf_read bitbuf(stringtables, length);
    size_t numTables = bitbuf.ReadByte();
    char tablename[256];
    while (numTables--)
    {
        bitbuf.ReadString(tablename, sizeof(tablename));
        printf("stringtable %s\n", tablename);
        ReadStringTable(bitbuf);
    }
}

void DemoReader::ProcessDem(void* inputFp, IDemoWriter* writer)
{
    CreateNetMsgStructs();
    SourceGameContext context;
    DemoFileReader reader(reinterpret_cast<FILE*>(inputFp));
    {
        demoheader_t header;
        reader.ReadDemoHeader(header);
        writer->StartWriting(header);
        context.protocol = header.networkprotocol;
    }
    
    democmdinfo_t cmdInfo;
    CommandPacket packet;
    packet.cmdInfo = &cmdInfo;
    std::unique_ptr<uint8_t[]> buffer(new uint8_t[NET_MAX_PAYLOAD]);
    do
    {
        size_t rawDataSize = 0;
        reader.ReadCmdHeader(packet.cmd, packet.tick);
        if (packet.cmd == dem_packet || packet.cmd == dem_signon)
        {
            reader.ReadCmdInfo(*packet.cmdInfo);
            reader.ReadSequenceInfo(packet.sequenceInfo1, packet.sequenceInfo2);
            assert(packet.sequenceInfo1 == packet.sequenceInfo2);
        }

        writer->StartCommandPacket(packet);
        switch (packet.cmd)
        {
        case dem_signon:
        case dem_packet:
            rawDataSize = reader.ReadRawData(buffer.get(), NET_MAX_PAYLOAD);
            ParsePacket(buffer.get(), rawDataSize, context, writer);
            break;
        case dem_synctick:
            // nothing
            break;
        case dem_consolecmd:
            reader.ReadRawData(nullptr, 1024);
            break;
        case dem_usercmd:
            {
                int32_t sequenceNum;
                reader.ReadUserCmd(sequenceNum, buffer.get(), 256);
            }
            break;
        case dem_datatables:
            // TODO: datatables
            reader.ReadRawData(nullptr, 64*1024);
            break;
        case dem_stop:
            // TODO assert frame and tick numbers
            break;
        case dem_stringtables:
            {
                uint8_t* stringtables = reinterpret_cast<uint8_t*>(malloc(MAX_STRINGTABLE_DATA));
                size_t length = reader.ReadRawData(stringtables, MAX_STRINGTABLE_DATA);
                ParseStringTables(stringtables, length, context, writer);
                free(stringtables);
            }
            break;
        default:
            assert(false);
            break;
        }
        writer->EndCommandPacket();
    } while (packet.cmd != dem_stop);
    writer->EndWriting();
    DestroyNetMsgStructs();
}
