
#include "demoreader.h"
#include "idemowriter.h"
#include "demofile.h"

/*void ParsePacket(const std::vector<unsigned char>& packet)
{
    assert(packet.size() <= NET_MAX_PAYLOAD);
    bf_read bitbuf(packet.data(), packet.size());
    while (bitbuf.GetNumBitsLeft() >= NETMSG_TYPE_BITS)
    {
        uint32 typeId = bitbuf.ReadUBitLong(NETMSG_TYPE_BITS);
        printf("%i\n", typeId);
        //ProcessNetMsg(typeId, bitbuf);
    }
}

void ParseDemoSequence(const std::vector<unsigned char>& sequenceData)
{
    CommandPacket packet;
    std::vector<uint8_t> buffer;

    DemoSequenceReader reader(sequenceData);
    for (; reader.ReadCmdHeader(packet.cmd, packet.tick);)
    {
        switch (packet.cmd)
        {
        case dem_signon:
        case dem_packet:
            reader.ReadCmdInfo(packet.cmdInfo);
            reader.ReadSequenceInfo(packet.sequenceInfo1, packet.sequenceInfo2);
            assert(packet.sequenceInfo1 == packet.sequenceInfo2);
            reader.ReadRawData(buffer);
            ParsePacket(buffer);
            break;
        case dem_synctick:
            // nothing
            break;
        case dem_consolecmd:
            reader.ReadRawData(nullptr, 1024);
            break;
        case dem_usercmd:
            reader.ReadUserCmd(buffer, 256);
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
    }
}*/

void DemoReader::ProcessDem(void* inputFp, IDemoWriter* writer)
{
    DemoFileReader demoFile(reinterpret_cast<FILE*>(inputFp));

    //auto demoHeader = demoFile.GetDemoHeader();
    //ParseDemoSequence(demoFile.GetSignOnData());
    //ParseDemoSequence(demoFile.GetDemoData());
}
