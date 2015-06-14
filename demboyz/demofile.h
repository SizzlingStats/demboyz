
#pragma once

#include <cstdint>
#include <cstdio>

struct demoheader_t;
struct democmdinfo_t;

class DemoFileReader
{
public:
    DemoFileReader(FILE* fp);

    void ReadDemoHeader(demoheader_t& header);
    int32_t ReadRawData(uint8_t* buffer, int32_t maxLength);
    void ReadSequenceInfo(int32_t& seqNum1, int32_t& seqNum2);
    void ReadCmdInfo(democmdinfo_t& info);
    void ReadCmdHeader(unsigned char& cmd, int32_t& tick);
    int32_t ReadUserCmd(int32_t& cmdNum, uint8_t* buffer, int32_t maxLength);

private:
    FILE* m_demoFp;
};

class DemoFileWriter
{
public:
    DemoFileWriter(FILE* fp);

    void WriteDemoHeader(const demoheader_t& header);
    void WriteRawData(const uint8_t* buffer, int32_t length);
    void WriteSequenceInfo(int32_t seqNum1, int32_t seqNum2);
    void WriteCmdInfo(const democmdinfo_t& info);
    void WriteCmdHeader(unsigned char cmd, int32_t tick);
    void WriteUserCmd(int32_t cmdNum, const uint8_t* buffer, int32_t length);

private:
    FILE* m_demoFp;
};
