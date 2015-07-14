
#include "svc_getcvarvalue.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"

namespace NetHandlers
{
    bool SVC_GetCvarValue_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_GetCvarValue* data)
    {
        data->cookie = bitbuf.ReadSBitLong(32);
        bitbuf.ReadString(data->cvarName, sizeof(data->cvarName));
        return !bitbuf.IsOverflowed();
    }

    bool SVC_GetCvarValue_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_GetCvarValue* data)
    {
        bitbuf.WriteSBitLong(data->cookie, 32);
        bitbuf.WriteString(data->cvarName);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_GetCvarValue_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_GetCvarValue* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        data->cookie = reader.ReadInt32("cookie");
        reader.ReadString("cvarName", data->cvarName, sizeof(data->cvarName));
        return !reader.HasReadError();
    }

    bool SVC_GetCvarValue_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_GetCvarValue* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteInt32("cookie", data->cookie);
        jsonbuf.WriteString("cvarName", data->cvarName);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void SVC_GetCvarValue_ToString_Internal(std::ostringstream& out, NetMsg::SVC_GetCvarValue* data)
    {
        out << "svc_GetCvarValue: cvar: " << data->cvarName
            << ", cookie: " << data->cookie;
    }
}
