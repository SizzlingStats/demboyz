
#include "idemowriter.h"
#include <cstdio>

class JsonWriter: public IDemoWriter
{
public:
    JsonWriter(FILE* outputFp);

    virtual void StartWriting(demoheader_t& header) override final;
    virtual void EndWriting() override final;

    virtual void StartCommandPacket(CommandPacket& packet) override final;
    virtual void EndCommandPacket(const PacketTrailingBits& trailingBits) override final;

    virtual void WriteNetPacket(NetPacket& packet, SourceGameContext& context) override final;
};

IDemoWriter* IDemoWriter::CreateJsonWriter(void* outputFp)
{
    return new JsonWriter(reinterpret_cast<FILE*>(outputFp));
}

JsonWriter::JsonWriter(FILE* outputFp)
{
}

void JsonWriter::StartWriting(demoheader_t& header)
{
}

void JsonWriter::EndWriting()
{
}

void JsonWriter::StartCommandPacket(CommandPacket& packet)
{
}

void JsonWriter::EndCommandPacket(const PacketTrailingBits& trailingBits)
{
}

void JsonWriter::WriteNetPacket(NetPacket& packet, SourceGameContext& context)
{
}
