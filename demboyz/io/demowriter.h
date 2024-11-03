
#pragma once

#include "idemowriter.h"
#include "demofile/demofile.h"
#include "sourcesdk/bitbuf.h"
#include <memory>

class DemoWriter : public IDemoWriter
{
public:
    DemoWriter(FILE* outputFp);

    virtual void StartWriting(demoheader_t& header) override;
    virtual void EndWriting() override;

    virtual void StartCommandPacket(const CommandPacket& packet) override;
    virtual void EndCommandPacket(const PacketTrailingBits& trailingBits) override;

    virtual void WriteNetPacket(NetPacket& packet, SourceGameContext& context) override;

protected:
    bf_write& GetBuffer()
    {
        return m_cmdPacketBuf;
    }

private:
    DemoFileWriter m_writer;
    bf_write m_cmdPacketBuf;
    std::unique_ptr<uint8_t[]> m_packetBuffer;
};
