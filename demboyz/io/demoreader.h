
#pragma once

#include <cstdio>

class IDemoWriter;

namespace DemoReader
{
    void ProcessDem(std::FILE* inputFp, IDemoWriter* writer);
    void ProcessJson(std::FILE* inputFp, IDemoWriter* writer);
}
