
#pragma once

#include "idemowriter.h"
#include <cstdio>

class JsonWriter: public IDemoWriter
{
public:
    JsonWriter(FILE* outputFp);

    virtual void StartWriting(demoheader_t& header) override final;
    virtual void EndWriting() override final;

    virtual void StartCommandPacket(CommandPacket& packet) override final;
    virtual void EndCommandPacket() override final;

    virtual void WriteNetPacket(NetPacket& packet) override final;
};

IDemoWriter* IDemoWriter::CreateJsonWriter(void* outputFp)
{
    return new JsonWriter(reinterpret_cast<FILE*>(outputFp));
}
