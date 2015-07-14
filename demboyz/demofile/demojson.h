
#pragma once

#include "base/jsonfile.h"
#include <cstdint>
#include <cstdio>

struct demoheader_t;
struct democmdinfo_t;
class Vector;
class QAngle;

namespace DemoJsonWriter
{
    void WriteDemoHeader(base::JsonWriterFile& writer, const demoheader_t& header);
    void WriteSequenceInfo(base::JsonWriterFile& writer, int32_t seqNum1, int32_t seqNum2);
    void WriteCmdInfo(base::JsonWriterFile& writer, const democmdinfo_t& info);
    void WriteUserCmd(base::JsonWriterFile& writer, int32_t cmdNum, const uint8_t* buffer, int32_t length);
    void WriteVector(base::JsonWriterFile& writer, const char* name, const Vector& vec);
    void WriteAngle(base::JsonWriterFile& writer, const char* name, const QAngle& angle);
}
