
#include "svc_serverinfo.h"
#include "bitbuf.h"

namespace NetHandlers
{
    bool SVC_ServerInfo_BitRead_Internal(bf_read& bitbuf, SourceGameContext& context, NetMsg::SVC_ServerInfo* data)
    {
        data->protocol = bitbuf.ReadShort();
        data->serverCount = bitbuf.ReadLong();
        data->isHLTV = bitbuf.ReadOneBit() != 0;
        data->isDedicated = bitbuf.ReadOneBit() != 0;
        data->clientCRC = bitbuf.ReadLong();
        data->maxClasses = bitbuf.ReadWord();
        if (context.protocol <= 17)
        {
            data->mapCRC = bitbuf.ReadLong();
        }
        else
        {
            bitbuf.ReadBytes(data->unk1, sizeof(data->unk1));
        }
        data->playerSlot = bitbuf.ReadByte();
        data->maxClients = bitbuf.ReadByte();
        data->tickInterval = bitbuf.ReadFloat();
        data->os = bitbuf.ReadChar();
        bitbuf.ReadString(data->gameDir, sizeof(data->gameDir));
        bitbuf.ReadString(data->mapName, sizeof(data->mapName));
        bitbuf.ReadString(data->skyName, sizeof(data->skyName));
        bitbuf.ReadString(data->hostName, sizeof(data->hostName));
        if (context.protocol > 15)
        {
            data->unk2 = bitbuf.ReadOneBit() != 0;
        }
        return !bitbuf.IsOverflowed();
    }

    bool SVC_ServerInfo_BitWrite_Internal(bf_write& bitbuf, const SourceGameContext& context, NetMsg::SVC_ServerInfo* data)
    {
        bitbuf.WriteShort(data->protocol);
        bitbuf.WriteLong(data->serverCount);
        bitbuf.WriteOneBit(data->isHLTV);
        bitbuf.WriteOneBit(data->isDedicated);
        bitbuf.WriteLong(data->clientCRC);
        bitbuf.WriteWord(data->maxClasses);
        if (context.protocol <= 17)
        {
            bitbuf.WriteLong(data->mapCRC);
        }
        else
        {
            bitbuf.WriteBytes(data->unk1, sizeof(data->unk1));
        }
        bitbuf.WriteByte(data->playerSlot);
        bitbuf.WriteByte(data->maxClients);
        bitbuf.WriteFloat(data->tickInterval);
        bitbuf.WriteChar(data->os);
        bitbuf.WriteString(data->gameDir);
        bitbuf.WriteString(data->mapName);
        bitbuf.WriteString(data->skyName);
        bitbuf.WriteString(data->hostName);
        if (context.protocol > 15)
        {
            bitbuf.WriteOneBit(data->unk2);
        }
        return !bitbuf.IsOverflowed();
    }

    bool SVC_ServerInfo_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_ServerInfo* data)
    {
        return true;
    }

    bool SVC_ServerInfo_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_ServerInfo* data)
    {
        return true;
    }

    void SVC_ServerInfo_ToString_Internal(std::ostringstream& out, NetMsg::SVC_ServerInfo* data)
    {
        out << "svc_ServerInfo: game \"" << data->gameDir
            << "\", map \"" << data->mapName
            << "\", max " << static_cast<uint32_t>(data->maxClients);
    }
}
