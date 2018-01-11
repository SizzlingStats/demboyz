
#include "svc_cmdkeyvalues.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"

namespace NetHandlers
{
    // TODO
    bool SVC_CmdKeyValues_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_CmdKeyValues* data)
    {
        assert(false);
        return true;
    }

    bool SVC_CmdKeyValues_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_CmdKeyValues* data)
    {
        assert(false);
        return true;
    }

    bool SVC_CmdKeyValues_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_CmdKeyValues* data)
    {
        assert(false);
        return true;
    }

    bool SVC_CmdKeyValues_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_CmdKeyValues* data)
    {
        assert(false);
        return true;
    }

    void SVC_CmdKeyValues_ToString_Internal(std::ostringstream& out, NetMsg::SVC_CmdKeyValues* data)
    {
        assert(false);
        out << "svc_CmdKeyValues";
    }
}
