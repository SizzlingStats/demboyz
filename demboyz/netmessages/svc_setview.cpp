
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
        return true;
    }

    bool SVC_SetView_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_SetView* data)
    {
        jsonbuf.StartObject("svc_setview");
        jsonbuf.WriteUInt32("entIndex", data->entIndex);
        jsonbuf.EndObject();
        return true;
    }

    void SVC_SetView_ToString_Internal(std::ostringstream& out, NetMsg::SVC_SetView* data)
    {
        out << "svc_SetView: view entity " << data->entIndex;
    }
}
