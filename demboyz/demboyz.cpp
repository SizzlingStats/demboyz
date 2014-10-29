
#include "demofile.h"
#include <assert.h>

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
    demoFile.ReadCmdHeader(cmd, tick);

    assert(cmd == dem_synctick && tick == 0);

    demoFile.Close();

    return 0;
}
