
#include "svc_prefetch.h"
#include "bitbuf.h"
#include "netcontants.h"

namespace NetHandlers
{
    bool SVC_Prefetch_BitRead_Internal(bf_read& bitbuf, SourceGameContext& context, NetMsg::SVC_Prefetch* data)
    {
        data->type = NetMsg::SVC_Prefetch::SOUND;
        data->soundIndex = bitbuf.ReadUBitLong(MAX_SOUND_INDEX_BITS);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_Prefetch_BitWrite_Internal(bf_write& bitbuf, SourceGameContext& context, NetMsg::SVC_Prefetch* data)
    {
        bitbuf.WriteUBitLong(data->soundIndex, MAX_SOUND_INDEX_BITS);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_Prefetch_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_Prefetch* data)
    {
        return true;
    }

    bool SVC_Prefetch_JsonWrite_Internal(JsonWrite& jsonbuf, SourceGameContext& context, NetMsg::SVC_Prefetch* data)
    {
        return true;
    }

    void SVC_Prefetch_ToString_Internal(std::ostringstream& out, NetMsg::SVC_Prefetch* data)
    {
        out << "svc_Prefetch: type " << data->type
            << " index " << data->soundIndex;
    }
}
