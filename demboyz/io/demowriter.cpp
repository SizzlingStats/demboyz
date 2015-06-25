
#include "idemowriter.h"
#include "demofile/demotypes.h"
#include "demofile/demofile.h"
#include "netmessages/nethandlers.h"
#include "netmessages/netcontants.h"
#include "demmessages/demhandlers.h"
#include "sourcesdk/bitbuf.h"
#include <cstdio>
#include <memory>

class DemoWriter: public IDemoWriter
{
public:
    DemoWriter(FILE* outputFp);

    virtual void StartWriting(demoheader_t& header) override final;
    virtual void EndWriting() override final;

    virtual void StartCommandPacket(const CommandPacket& packet) override final;
    virtual void EndCommandPacket(const PacketTrailingBits& trailingBits) override final;

    virtual void WriteNetPacket(NetPacket& packet, SourceGameContext& context) override final;

private:
    DemoFileWriter m_writer;
    bf_write m_cmdPacketBuf;
    std::unique_ptr<uint8_t[]> m_packetBuffer;
};

IDemoWriter* IDemoWriter::CreateDemoWriter(void* outputFp)
{
    return new DemoWriter(reinterpret_cast<FILE*>(outputFp));
}

DemoWriter::DemoWriter(FILE* outputFp):
    m_writer(outputFp),
    m_packetBuffer(new uint8_t[NET_MAX_PAYLOAD])
{
    m_cmdPacketBuf.StartWriting(m_packetBuffer.get(), NET_MAX_PAYLOAD);
}

void DemoWriter::StartWriting(demoheader_t& header)
{
    m_writer.WriteDemoHeader(header);
}

void DemoWriter::EndWriting()
{
}

void DemoWriter::StartCommandPacket(const CommandPacket& packet)
{
    m_writer.WriteCmdHeader(packet.cmd, packet.tick);
    DemHandlers::DemMsg_FileWrite(packet.cmd, m_writer, packet.data);
    m_cmdPacketBuf.Reset();
}

void DemoWriter::EndCommandPacket(const PacketTrailingBits& trailingBits)
{
    if (trailingBits.numTrailingBits > 0)
    {
        m_cmdPacketBuf.WriteUBitLong(trailingBits.value, trailingBits.numTrailingBits);
    }
    const int numBitsToWrite = m_cmdPacketBuf.GetNumBitsWritten() % 8;
    if (numBitsToWrite != 0)
    {
        assert(false);
    }
    if (m_cmdPacketBuf.GetNumBytesWritten() > 0)
    {
        m_writer.WriteRawData(m_cmdPacketBuf.GetBasePointer(), m_cmdPacketBuf.GetNumBytesWritten());
    }
}

void DemoWriter::WriteNetPacket(NetPacket& packet, SourceGameContext& context)
{
    m_cmdPacketBuf.WriteUBitLong(packet.type, NETMSG_TYPE_BITS);
    NetHandlers::NetMsg_BitWrite(packet.type, m_cmdPacketBuf, context, packet.data);
}
