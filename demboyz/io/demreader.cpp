
#include "demoreader.h"
#include "idemowriter.h"
#include "demofile/demofile.h"
#include "demofile/demotypes.h"

#include "netmessages/nethandlers.h"
#include "netmessages/netcontants.h"
#include "demmessages/demhandlers.h"
#include "sourcesdk/bitbuf.h"
#include <vector>
#include <cstdint>
#include <memory>

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
    NetHandlers::CreateNetMsgStructs(netDataStructs);
    DemHandlers::CreateDemMsgStructs(demDataStructs);

    SourceGameContext context = SourceGameContext();
    DemoFileReader reader(inputFp);
    {
        demoheader_t header;
        reader.ReadDemoHeader(header);
        writer->StartWriting(header);
        context.protocol = header.networkprotocol;
    }

    CommandPacket packet;
    int frame = -1;
    do
    {
        size_t rawDataSize = 0;
        reader.ReadCmdHeader(packet.cmd, packet.tick);
        packet.data = demDataStructs[packet.cmd];
        DemHandlers::DemMsg_FileRead(packet.cmd, reader, packet.data);

        PacketTrailingBits trailingBits = PacketTrailingBits();
        writer->StartCommandPacket(packet);
        frame += (packet.cmd == dem_packet);
        if (packet.cmd == dem_packet || packet.cmd == dem_signon)
        {
            Array<uint8_t> buffer = reader.ReadRawData(NET_MAX_PAYLOAD);
            trailingBits = ParsePacket(buffer.begin(), buffer.length(), context, writer, netDataStructs);
        }
        writer->EndCommandPacket(trailingBits);
    } while (packet.cmd != dem_stop);
    writer->EndWriting();

    DemHandlers::DestroyDemMsgStructs(demDataStructs);
    NetHandlers::DestroyNetMsgStructs(netDataStructs);
}
