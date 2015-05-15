
#pragma once

#include "idemowriter.h"
#include "netmessages/nethandlers.h"
#include <cstdio>
#include <iostream>

class ConLogWriter: public IDemoWriter
{
public:
    ConLogWriter(FILE* outputFp);

    virtual void StartWriting(demoheader_t& header) override final;
    virtual void EndWriting() override final;

    virtual void StartCommandPacket(CommandPacket& packet) override final;
    virtual void EndCommandPacket() override final;

    virtual void WriteNetPacket(NetPacket& packet) override final;
};

IDemoWriter* IDemoWriter::CreateConLogWriter(void* outputFp)
{
    return new ConLogWriter(reinterpret_cast<FILE*>(outputFp));
}

ConLogWriter::ConLogWriter(FILE* outputFp)
{
}

void ConLogWriter::StartWriting(demoheader_t& header)
{
}

void ConLogWriter::EndWriting()
{
}

void ConLogWriter::StartCommandPacket(CommandPacket& packet)
{
}

void ConLogWriter::EndCommandPacket()
{
}

void ConLogWriter::WriteNetPacket(NetPacket& packet)
{
    std::ostringstream ss;
    NetHandlers::NetMsg_ToString(packet.type, ss, packet.data);
    std::cout << ss.str() << std::endl;
}
