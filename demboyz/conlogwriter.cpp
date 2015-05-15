
#pragma once

#include "idemowriter.h"
#include "netmessages/nethandlers.h"
#include "demotypes.h"
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

private:
    FILE* m_outputFp;
};

IDemoWriter* IDemoWriter::CreateConLogWriter(void* outputFp)
{
    return new ConLogWriter(reinterpret_cast<FILE*>(outputFp));
}

ConLogWriter::ConLogWriter(FILE* outputFp):
    m_outputFp(outputFp)
{
}

void ConLogWriter::StartWriting(demoheader_t& header)
{
    //std::ostringstream ss;
    //ss << "Playing demo from " <<  << ".\n";
    //fputs(ss.str().c_str(), m_outputFp);
    //std::clog << ss.str() << std::endl;
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
    ss << "\n";
    fputs(ss.str().c_str(), m_outputFp);
    std::clog << ss.str();
}
