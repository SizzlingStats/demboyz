
#include "svc_prefetch.h"
#include "sourcesdk/bitbuf.h"
#include "netcontants.h"

namespace NetHandlers
{
    bool SVC_Prefetch_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_Prefetch* data)
    {
        data->type = NetMsg::SVC_Prefetch::SOUND;
        if (context.protocol > 23)
        {
            data->soundIndex = bitbuf.ReadUBitLong(MAX_SOUND_INDEX_BITS);
        }
        else
        {
            data->soundIndex = bitbuf.ReadUBitLong(MAX_SOUND_INDEX_BITS_OLD);
        }
        return !bitbuf.IsOverflowed();
    }

    bool SVC_Prefetch_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_Prefetch* data)
    {
        if (context.protocol > 23)
        {
            bitbuf.WriteUBitLong(data->soundIndex, MAX_SOUND_INDEX_BITS);
        }
        else
        {
            bitbuf.WriteUBitLong(data->soundIndex, MAX_SOUND_INDEX_BITS_OLD);
        }
        return !bitbuf.IsOverflowed();
    }

    bool SVC_Prefetch_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_Prefetch* data)
    {
        return true;
    }

    bool SVC_Prefetch_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_Prefetch* data)
    {
        return true;
    }

    void SVC_Prefetch_ToString_Internal(std::ostringstream& out, NetMsg::SVC_Prefetch* data)
    {
        out << "svc_Prefetch: type " << data->type
            << " index " << data->soundIndex;
    }
}
