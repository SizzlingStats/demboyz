
#include "idemowriter.h"
#include "demofile/demojson.h"
#include "demofile/demotypes.h"
#include "base/jsonfile.h"
#include "demmessages/demhandlers.h"
#include "netmessages/nethandlers.h"
#include <cstdio>
#include <cassert>

class JsonWriter: public IDemoWriter
{
public:
    JsonWriter(FILE* outputFp);

    virtual void StartWriting(demoheader_t& header) override final;
    virtual void EndWriting() override final;

    virtual void StartCommandPacket(const CommandPacket& packet) override final;
    virtual void EndCommandPacket(const PacketTrailingBits& trailingBits) override final;

    virtual void WriteNetPacket(NetPacket& packet, SourceGameContext& context) override final;

private:
    base::JsonWriterFile m_jsonFile;
    bool m_writingNetPackets;
    char m_buffer[4096];
};

IDemoWriter* IDemoWriter::CreateJsonWriter(void* outputFp)
{
    return new JsonWriter(reinterpret_cast<FILE*>(outputFp));
}

JsonWriter::JsonWriter(FILE* outputFp):
    m_jsonFile(outputFp, m_buffer, sizeof(m_buffer)),
    m_writingNetPackets(false)
{
}

void JsonWriter::StartWriting(demoheader_t& header)
{
    auto& jsonFile = m_jsonFile;
    jsonFile.StartObject();

    DemoJsonWriter::WriteDemoHeader(jsonFile, header);
    jsonFile.StartArray("demmessages");
}

void JsonWriter::EndWriting()
{
    auto& jsonFile = m_jsonFile;
    jsonFile.EndArray();
    jsonFile.EndObject();
    assert(jsonFile.IsComplete());
}

void JsonWriter::StartCommandPacket(const CommandPacket& packet)
{
    auto& jsonFile = m_jsonFile;
    jsonFile.StartObject();
    jsonFile.WriteInt32("tick", packet.tick);
    jsonFile.StartObject(DemoCmdToString(packet.cmd));
    DemHandlers::DemMsg_JsonWrite(packet.cmd, jsonFile, packet.data);
    if (packet.cmd == dem_packet || packet.cmd == dem_signon)
    {
        m_writingNetPackets = true;
        jsonFile.StartArray("netpackets");
    }
}

void JsonWriter::EndCommandPacket(const PacketTrailingBits& trailingBits)
{
    auto& jsonFile = m_jsonFile;
    if (m_writingNetPackets)
    {
        m_writingNetPackets = false;
        jsonFile.EndArray();
    }
    jsonFile.EndObject();
    jsonFile.EndObject();
}

void JsonWriter::WriteNetPacket(NetPacket& packet, SourceGameContext& context)
{
    auto& jsonFile = m_jsonFile;
    jsonFile.StartObject();
    NetHandlers::NetMsg_JsonWrite(packet.type, jsonFile, context, packet.data);
    jsonFile.EndObject();
}
