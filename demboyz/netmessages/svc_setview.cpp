
#include "svc_setview.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"
#include "netcontants.h"

namespace NetHandlers
{
    bool SVC_SetView_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_SetView* data)
    {
        data->entIndex = bitbuf.ReadUBitLong(MAX_EDICT_BITS);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_SetView_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_SetView* data)
    {
        bitbuf.WriteUBitLong(data->entIndex, MAX_EDICT_BITS);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_SetView_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_SetView* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        data->entIndex = reader.ReadUInt32("entIndex");
        return !reader.HasReadError();
    }

    bool SVC_SetView_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_SetView* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteUInt32("entIndex", data->entIndex);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void SVC_SetView_ToString_Internal(std::ostringstream& out, NetMsg::SVC_SetView* data)
    {
        out << "svc_SetView: view entity " << data->entIndex;
    }
}
