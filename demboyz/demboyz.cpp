
#include "demofile.h"
#include "demofilebitbuf.h"
#include "netmessages.h"
#include <assert.h>
#include <vector>

void ParsePacket(const std::vector<unsigned char>& packet)
{
    assert(packet.size() <= NET_MAX_PAYLOAD);
    CBitRead bitbuf(packet.data(), packet.size());
    while (bitbuf.GetNumBitsLeft() >= NETMSG_TYPE_BITS)
    {
        uint32 typeId = bitbuf.ReadUBitLong(NETMSG_TYPE_BITS);
        printf("%i\n", typeId);
        ProcessNetMsg(typeId, bitbuf);
    }
}

void ParseDemoSequence(const std::vector<unsigned char>& sequenceData)
{
    unsigned char cmd;
    int32 tick;
    int32 sequenceInfo1;
    int32 sequenceInfo2;
    democmdinfo_t cmdInfo;
    std::vector<unsigned char> buffer;

    DemoSequenceReader reader(sequenceData);
    for (;;)
    {
        reader.ReadCmdHeader(cmd, tick);
        switch (cmd)
        {
        case dem_signon:
        case dem_packet:
            reader.ReadCmdInfo(cmdInfo);
            reader.ReadSequenceInfo(sequenceInfo1, sequenceInfo2);
            assert(sequenceInfo1 == sequenceInfo2);
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
}

int main(const int argc, const char* argv[])
{
    if (argc < 2)
    {
        return -1;
    }

    CDemoFile demoFile;
    if (!demoFile.Open(argv[1]))
    {
        return -1;
    }

    auto demoHeader = demoFile.GetDemoHeader();
    ParseDemoSequence(demoFile.GetSignOnData());
    ParseDemoSequence(demoFile.GetDemoData());
    demoFile.Close();
    return 0;
}
