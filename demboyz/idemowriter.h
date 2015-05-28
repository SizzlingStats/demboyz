
#pragma once

#include <cstdint>

struct democmdinfo_t;
struct demoheader_t;
struct CommandPacket;
struct NetPacket;
struct SourceGameContext;

struct CommandPacket
{
    unsigned char cmd;
    int32_t tick;

    democmdinfo_t* cmdInfo;
    int32_t sequenceInfo1;
    int32_t sequenceInfo2;
};

struct NetPacket
{
    uint32_t type;
    void* data;
};

class IDemoWriter
{
public:
    virtual ~IDemoWriter() {}

    virtual void StartWriting(demoheader_t& header) = 0;
    virtual void EndWriting() = 0;

    virtual void StartCommandPacket(CommandPacket& packet) = 0;
    virtual void EndCommandPacket() = 0;

    virtual void WriteNetPacket(NetPacket& packet, SourceGameContext& context) = 0;

public:
    static IDemoWriter* CreateJsonWriter(void* outputFp);
    static IDemoWriter* CreateDemoWriter(void* outputFp);
    static IDemoWriter* CreateConLogWriter(void* outputFp);

    static void FreeDemoWriter(IDemoWriter* writer)
    {
        delete writer;
    }
};
