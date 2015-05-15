
#include "svc_getcvarvalue.h"
#include "bitbuf.h"

namespace NetHandlers
{
    bool SVC_GetCvarValue_BitRead_Internal(bf_read& bitbuf, SourceGameContext& context, NetMsg::SVC_GetCvarValue* data)
    {
        data->cookie = bitbuf.ReadSBitLong(32);
        bitbuf.ReadString(data->cvarName, sizeof(data->cvarName));
        return !bitbuf.IsOverflowed();
    }

    bool SVC_GetCvarValue_BitWrite_Internal(bf_write& bitbuf, const SourceGameContext& context, NetMsg::SVC_GetCvarValue* data)
    {
        bitbuf.WriteSBitLong(data->cookie, 32);
        bitbuf.WriteString(data->cvarName);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_GetCvarValue_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_GetCvarValue* data)
    {
        return true;
    }

    bool SVC_GetCvarValue_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_GetCvarValue* data)
    {
        return true;
    }

    void SVC_GetCvarValue_ToString_Internal(std::ostringstream& out, NetMsg::SVC_GetCvarValue* data)
    {
        out << "svc_GetCvarValue: cvar: " << data->cvarName
            << ", cookie: " << data->cookie;
    }
}
