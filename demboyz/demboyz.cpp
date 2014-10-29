
#include "demofile.h"
#include <assert.h>
#include <memory>

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
        printf("tick: %i\n", tick);
        switch (cmd)
        {
            case dem_packet:
                demoFile.ReadCmdInfo(cmdInfo);
                demoFile.ReadSequenceInfo(sequenceInfo1, sequenceInfo2);
                demoFile.ReadRawData(packetBuf);
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
