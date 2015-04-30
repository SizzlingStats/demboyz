
#include "demofile.h"
#include "demofilebitbuf.h"
#include <assert.h>
#include <vector>
#include <fstream>

std::vector<std::string> eventNames;

void ParseGameEvent(const std::string& eventBuf)
{
    CBitRead bitBuf(eventBuf.data(), eventBuf.size());
    uint32 eventId = bitBuf.ReadUBitLong(9);
    printf("%s\n", eventNames[eventId-1].c_str());
}

void ParsePacket(const std::string& packetBuf)
{
    auto data = packetBuf.data();
    CBitRead bitBuf(packetBuf.data(), packetBuf.size());
    uint32 typeId = bitBuf.ReadUBitLong(5);
    printf("%i\n", typeId);
    if (typeId != 25)
    {
        return;
    }

    uint32 length = bitBuf.ReadUBitLong(11);
    int numBytes = (length / 8) + (length % 8 > 0);
    
    std::string subpacket;
    subpacket.resize(numBytes);

    bitBuf.ReadBits(&subpacket[0], length);
    ParseGameEvent(subpacket);
}

void ParseEventNames(const char* eventfile, std::vector<std::string>& eventNames)
{
    using namespace std;
    ifstream eventStream(eventfile);
    if (eventStream)
    {
        move(istream_iterator<string>(eventStream), istream_iterator<string>(), back_inserter(eventNames));
    }
}

int main(const int argc, const char* argv[])
{
    if (argc < 3)
    {
        return -1;
    }

    ParseEventNames(argv[1], eventNames);

    CDemoFile demoFile;
    if (!demoFile.Open(argv[2]))
    {
        return -1;
    }

    auto demoHeader = demoFile.GetDemoHeader();

    unsigned char cmd;
    int32 tick;
    int32 sequenceInfo1;
    int32 sequenceInfo2;
    democmdinfo_t cmdInfo;
    std::string packetBuf;
    demoFile.ReadCmdHeader(cmd, tick);

    assert(cmd == dem_synctick && tick == 0);
    
    const int numFrames = demoHeader->playback_frames;
    const int numTicks = demoHeader->playback_ticks;
    for (int i = 0; i <= numFrames; ++i)
    {
        demoFile.ReadCmdHeader(cmd, tick);
        //printf("tick: %i\n", tick);
        switch (cmd)
        {
            case dem_packet:
                demoFile.ReadCmdInfo(cmdInfo);
                demoFile.ReadSequenceInfo(sequenceInfo1, sequenceInfo2);
                assert(sequenceInfo1 == sequenceInfo2);
                demoFile.ReadRawData(packetBuf);
                ParsePacket(packetBuf);
                break;
            case dem_stop:
                assert(i == numFrames && tick == numTicks);
                break;
            case dem_synctick:
            case dem_consolecmd:
            case dem_usercmd:
            case dem_datatables:
            default:
                break;
        }
    }

    demoFile.Close();

    return 0;
}
