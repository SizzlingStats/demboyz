
#pragma once

#include "base/array.h"
#include "base/jsonfile.h"
#include <cstdint>
#include <cstdio>

struct demoheader_t;
struct democmdinfo_t;
class Vector;
class QAngle;

namespace DemoJsonReader
{
    bool ReadDemoHeader(base::JsonReaderObject& reader, demoheader_t& header);
    bool ReadSequenceInfo(base::JsonReaderObject& reader, int32_t& seqNum1, int32_t& seqNum2);
    bool ReadCmdInfo(base::JsonReaderObject& reader, democmdinfo_t& info);
    bool ReadCmdHeader(base::JsonReaderObject& reader, unsigned char& cmd, int32_t& tick);
    bool ReadUserCmd(base::JsonReaderObject& reader, int32_t& cmdNum, Array<uint8_t>& dest, int32_t maxLength);
    bool ReadVector(base::JsonReaderObject& reader, const char* name, Vector& vec);
    bool ReadAngle(base::JsonReaderObject& reader, const char* name, QAngle& angle);
}

namespace DemoJsonWriter
{
    void WriteDemoHeader(base::JsonWriterFile& writer, const demoheader_t& header);
    void WriteSequenceInfo(base::JsonWriterFile& writer, int32_t seqNum1, int32_t seqNum2);
    void WriteCmdInfo(base::JsonWriterFile& writer, const democmdinfo_t& info);
    void WriteCmdHeader(base::JsonWriterFile& writer, unsigned char cmd, int32_t tick);
    void WriteUserCmd(base::JsonWriterFile& writer, int32_t cmdNum, const uint8_t* buffer, int32_t length);
    void WriteVector(base::JsonWriterFile& writer, const char* name, const Vector& vec);
    void WriteAngle(base::JsonWriterFile& writer, const char* name, const QAngle& angle);
}
