
#include "demoreader.h"
#include "idemowriter.h"
#include "demofile/demofile.h"
#include "demofile/demotypes.h"

#include "netmessages/netmessages.h"
#include "netmessages/netcontants.h"
#include "demmessages/demmessages.h"
#include "demmessages/demhandlers.h"
#include "sourcesdk/bitbuf.h"
#include <vector>
#include <cstdint>
#include <memory>

static void CreateNetMsgStructs(void* netDataStructs[32])
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

static void DestroyNetMsgStructs(void* netDataStructs[32])
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

static void CreateDemMsgStructs(void* demDataStructs[9])
{
    demDataStructs[0] = new DemMsg::Dem_Unknown();
    demDataStructs[1] = new DemMsg::Dem_Packet();
    demDataStructs[2] = new DemMsg::Dem_Packet();
    demDataStructs[3] = new DemMsg::Dem_SyncTick();
    demDataStructs[4] = new DemMsg::Dem_ConsoleCmd();
    demDataStructs[5] = new DemMsg::Dem_UserCmd();
    demDataStructs[6] = new DemMsg::Dem_DataTables();
    demDataStructs[7] = new DemMsg::Dem_Stop();
    demDataStructs[8] = new DemMsg::Dem_StringTables();
}

static void DestroyDemMsgStructs(void* demDataStructs[9])
{
    delete reinterpret_cast<DemMsg::Dem_Unknown*>(demDataStructs[0]);
    delete reinterpret_cast<DemMsg::Dem_Packet*>(demDataStructs[1]);
    delete reinterpret_cast<DemMsg::Dem_Packet*>(demDataStructs[2]);
    delete reinterpret_cast<DemMsg::Dem_SyncTick*>(demDataStructs[3]);
    delete reinterpret_cast<DemMsg::Dem_ConsoleCmd*>(demDataStructs[4]);
    delete reinterpret_cast<DemMsg::Dem_UserCmd*>(demDataStructs[5]);
    delete reinterpret_cast<DemMsg::Dem_DataTables*>(demDataStructs[6]);
    delete reinterpret_cast<DemMsg::Dem_Stop*>(demDataStructs[7]);
    delete reinterpret_cast<DemMsg::Dem_StringTables*>(demDataStructs[8]);
}

PacketTrailingBits ParsePacket(uint8_t* packet, size_t length,
                               SourceGameContext& context, IDemoWriter* writer,
                               void* netDataStructs[32])
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

    PacketTrailingBits trailingBits;
    trailingBits.numTrailingBits = bitbuf.GetNumBitsLeft();
    if (trailingBits.numTrailingBits)
    {
        trailingBits.value = bitbuf.ReadUBitLong(trailingBits.numTrailingBits);
    }
    else
    {
        trailingBits.value = 0;
    }
    return trailingBits;
}

void DemoReader::ProcessDem(std::FILE* inputFp, IDemoWriter* writer)
{
    void* netDataStructs[32];
    void* demDataStructs[9];
    CreateNetMsgStructs(netDataStructs);
    CreateDemMsgStructs(demDataStructs);

    SourceGameContext context = SourceGameContext();
    DemoFileReader reader(inputFp);
    {
        demoheader_t header;
        reader.ReadDemoHeader(header);
        writer->StartWriting(header);
        context.protocol = header.networkprotocol;
    }

    CommandPacket packet;
    do
    {
        size_t rawDataSize = 0;
        reader.ReadCmdHeader(packet.cmd, packet.tick);
        packet.data = demDataStructs[packet.cmd];
        DemHandlers::DemMsg_FileRead(packet.cmd, reader, packet.data);

        PacketTrailingBits trailingBits = PacketTrailingBits();
        writer->StartCommandPacket(packet);
        if (packet.cmd == dem_packet || packet.cmd == dem_signon)
        {
            Array<uint8_t> buffer = reader.ReadRawData(NET_MAX_PAYLOAD);
            trailingBits = ParsePacket(buffer.begin(), buffer.length(), context, writer, netDataStructs);
        }
        writer->EndCommandPacket(trailingBits);
    } while (packet.cmd != dem_stop);
    writer->EndWriting();

    DestroyDemMsgStructs(demDataStructs);
    DestroyNetMsgStructs(netDataStructs);
}
