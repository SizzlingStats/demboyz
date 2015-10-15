
#include "demoreader.h"
#include "demofile/demotypes.h"
#include "demofile/demojson.h"
#include "base/jsonfile.h"
#include "io/idemowriter.h"

#include "netmessages/nethandlers.h"
#include "demmessages/demhandlers.h"

int32_t ReadNetpacket(base::JsonReaderFile& jsonReader, PacketTrailingBits& trailingBits)
{
    base::JsonReaderObject reader = jsonReader.ParseObject();
    assert(!reader.HasReadError());
    const int32_t type = reader.ReadInt32("netpacket");
    if (type < 0)
    {
        base::JsonReaderObject reader = jsonReader.ParseObject();
        assert(!reader.HasReadError());
        trailingBits.numTrailingBits = reader.ReadInt32("numTrailingBits");
        trailingBits.value = reader.ReadInt32("trailingBitsValue");
    }
    return type;
}

PacketTrailingBits ParsePacket(base::JsonReaderFile& jsonReader,
                               SourceGameContext& context, IDemoWriter* writer,
                               const NetHandlers::NetDataStructArray& netDataStructs)
{
    PacketTrailingBits trailingBits = PacketTrailingBits();
    NetPacket netPacket = NetPacket();
    while ((netPacket.type = ReadNetpacket(jsonReader, trailingBits)) >= 0)
    {
        netPacket.data = netDataStructs[netPacket.type];
        NetHandlers::NetMsg_JsonRead(netPacket.type, jsonReader, context, netPacket.data);
        writer->WriteNetPacket(netPacket, context);
    }
    return trailingBits;
}

void DemoReader::ProcessJson(std::FILE* inputFp, IDemoWriter* writer)
{
    NetHandlers::NetDataStructArray netDataStructs;
    void* demDataStructs[9];
    NetHandlers::CreateNetMsgStructs(netDataStructs);
    DemHandlers::CreateDemMsgStructs(demDataStructs);

    SourceGameContext context = SourceGameContext();
    char buffer[4096];
    base::JsonReaderFile jsonReader(inputFp, buffer, sizeof(buffer));
    {
        base::JsonReaderObject reader = jsonReader.ParseObject();
        assert(!reader.HasReadError());

        demoheader_t header;
        if (!DemoJsonReader::ReadDemoHeader(reader, header))
        {
            return;
        }
        writer->StartWriting(header);
        context.protocol = header.networkprotocol;
    }

    CommandPacket packet;
    do
    {
        {
            base::JsonReaderObject reader = jsonReader.ParseObject();
            if (reader.HasReadError())
            {
                assert(!reader.HasReadError());
            }
            
            DemoJsonReader::ReadCmdHeader(reader, packet.cmd, packet.tick);
        }
        packet.data = demDataStructs[packet.cmd];
        DemHandlers::DemMsg_JsonRead(packet.cmd, jsonReader, packet.data);

        PacketTrailingBits trailingBits = PacketTrailingBits();
        writer->StartCommandPacket(packet);
        if (packet.cmd == dem_packet || packet.cmd == dem_signon)
        {
            trailingBits = ParsePacket(jsonReader, context, writer, netDataStructs);
        }
        writer->EndCommandPacket(trailingBits);
    } while (packet.cmd != dem_stop);
    writer->EndWriting();

    DemHandlers::DestroyDemMsgStructs(demDataStructs);
    NetHandlers::DestroyNetMsgStructs(netDataStructs);
}
