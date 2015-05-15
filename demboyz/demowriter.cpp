
#pragma once

#include "idemowriter.h"
#include <cstdio>

class DemoWriter: public IDemoWriter
{
public:
    DemoWriter(FILE* outputFp);

    virtual void StartWriting(demoheader_t& header) override final;
    virtual void EndWriting() override final;

    virtual void StartCommandPacket(CommandPacket& packet) override final;
    virtual void EndCommandPacket() override final;

    virtual void WriteNetPacket(NetPacket& packet) override final;
};

IDemoWriter* IDemoWriter::CreateDemoWriter(void* outputFp)
{
    return new DemoWriter(reinterpret_cast<FILE*>(outputFp));
}

DemoWriter::DemoWriter(FILE* outputFp)
{
}

void DemoWriter::StartWriting(demoheader_t& header)
{
}

void DemoWriter::EndWriting()
{
}

void DemoWriter::StartCommandPacket(CommandPacket& packet)
{
}

void DemoWriter::EndCommandPacket()
{
}

void DemoWriter::WriteNetPacket(NetPacket& packet)
{
}
