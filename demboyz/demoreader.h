
#pragma once

class IDemoWriter;

namespace DemoReader
{
    void ProcessDem(void* inputFp, IDemoWriter* writer);
    void ProcessJson(void* inputFp, IDemoWriter* writer);
}
