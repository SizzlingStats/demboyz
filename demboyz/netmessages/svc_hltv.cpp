
#include "svc_hltv.h"
#include <cassert>

namespace NetHandlers
{
    bool SVC_HLTV_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_HLTV* data)
    {
        assert(false);
        return true;
    }

    bool SVC_HLTV_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_HLTV* data)
    {
        assert(false);
        return true;
    }

    bool SVC_HLTV_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_HLTV* data)
    {
        assert(false);
        return true;
    }

    bool SVC_HLTV_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_HLTV* data)
    {
        assert(false);
        return true;
    }

    void SVC_HLTV_ToString_Internal(std::ostringstream& out, NetMsg::SVC_HLTV* data)
    {
        out << "svc_HLTV";
    }
}
