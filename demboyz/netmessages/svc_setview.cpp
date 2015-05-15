
#include "svc_setview.h"
#include "bitbuf.h"
#include "netcontants.h"

namespace NetHandlers
{
    bool SVC_SetView_BitRead_Internal(bf_read& bitbuf, SourceGameContext& context, NetMsg::SVC_SetView* data)
    {
        data->entIndex = bitbuf.ReadUBitLong(MAX_EDICT_BITS);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_SetView_BitWrite_Internal(bf_write& bitbuf, const SourceGameContext& context, NetMsg::SVC_SetView* data)
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
        return true;
    }

    void SVC_SetView_ToString_Internal(std::ostringstream& out, NetMsg::SVC_SetView* data)
    {
        out << "svc_SetView: view entity " << data->entIndex;
    }
}
