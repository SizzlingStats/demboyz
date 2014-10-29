
#include "demofile.h"

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

    demoFile.Close();

    return 0;
}
