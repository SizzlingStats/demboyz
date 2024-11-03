
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

    void* data;
};

struct NetPacket
{
    int32_t type;
    void* data;
};

struct PacketTrailingBits
{
    uint32_t numTrailingBits;
    uint32_t value;
};

class IDemoWriter
{
public:
    virtual ~IDemoWriter() {}

    virtual void StartWriting(demoheader_t& header) = 0;
    virtual void EndWriting() = 0;

    virtual void StartCommandPacket(const CommandPacket& packet) = 0;
    virtual void EndCommandPacket(const PacketTrailingBits& trailingBits) = 0;

    virtual void WriteNetPacket(NetPacket& packet, SourceGameContext& context) = 0;

public:
    static IDemoWriter* CreateJsonWriter(void* outputFp);
    static IDemoWriter* CreateDemoWriter(void* outputFp);
    static IDemoWriter* CreateConLogWriter(void* outputFp);
    static IDemoWriter* CreateVoiceDataWriter(const char* outputPath);

    static IDemoWriter* CreateRocketModeFixerDemoWriter(void* outputFp);

    static void FreeDemoWriter(IDemoWriter* writer)
    {
        delete writer;
    }
};
