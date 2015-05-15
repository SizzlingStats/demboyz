
#include "demoreader.h"
#include "idemowriter.h"
#include "demofile.h"
#include "demotypes.h"

#include "netmessages/netmessages.h"
#include "netmessages/netcontants.h"
#include "bitbuf.h"
#include <vector>
#include <cstdint>

typedef bool (*NetMsgReadFn)(bf_read& bitbuf, SourceGameContext& context, void* data);

static const NetMsgReadFn netHandlers[] = DECLARE_NET_HANDLER_ARRAY(BitRead);
//static const void* netDataStructs[] = DECLARE_NET_DATA_ARRAY;

void ParsePacket(uint8_t* packet, size_t length, IDemoWriter* writer)
{
    assert(length <= NET_MAX_PAYLOAD);
    bf_read bitbuf(packet, length);
    NetPacket netPacket;
    while (bitbuf.GetNumBitsLeft() >= NETMSG_TYPE_BITS)
    {
        netPacket.type = bitbuf.ReadUBitLong(NETMSG_TYPE_BITS);
        //netPacket.data = netDataStructs[netPacket.type];
        //netHandlers[netPacket.type](bitbuf, context, );
    }
}

void ParseDemo(DemoFileReader& reader, IDemoWriter* writer)
{
    democmdinfo_t cmdInfo;
    CommandPacket packet;
    packet.cmdInfo = &cmdInfo;
    std::vector<uint8_t> buffer;
    buffer.resize(NET_MAX_PAYLOAD);

    do
    {
        reader.ReadCmdHeader(packet.cmd, packet.tick);
        switch (packet.cmd)
        {
        case dem_signon:
        case dem_packet:
            reader.ReadCmdInfo(*packet.cmdInfo);
            reader.ReadSequenceInfo(packet.sequenceInfo1, packet.sequenceInfo2);
            assert(packet.sequenceInfo1 == packet.sequenceInfo2);
            reader.ReadRawData(buffer.data(), buffer.size());
            break;
        case dem_synctick:
            // nothing
            break;
        case dem_consolecmd:
            reader.ReadRawData(nullptr, 1024);
            break;
        case dem_usercmd:
            reader.ReadUserCmd(buffer.data(), 256);
            break;
        case dem_datatables:
            // TODO: datatables
            reader.ReadRawData(nullptr, 64*1024);
            break;
        case dem_stop:
            // TODO assert frame and tick numbers
            break;
        case dem_customdata:
            reader.ReadRawData(nullptr, 0);
            break;
        case dem_stringtables:
            reader.ReadRawData(nullptr, 0);
            break;
        default:
            assert(false);
            break;
        }
        writer->StartCommandPacket(packet);
        if (packet.cmd == dem_packet || packet.cmd == dem_signon)
        {
            ParsePacket(buffer.data(), buffer.size(), writer);
        }
        writer->EndCommandPacket();
    } while (packet.cmd != dem_stop);
}

void DemoReader::ProcessDem(void* inputFp, IDemoWriter* writer)
{
    DemoFileReader demoFile(reinterpret_cast<FILE*>(inputFp));
    {
        demoheader_t header;
        demoFile.ReadDemoHeader(header);
        writer->StartWriting(header);
    }
    ParseDemo(demoFile, writer);
    writer->EndWriting();
}
