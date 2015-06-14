
#include "svc_print.h"
#include "sourcesdk/bitbuf.h"

namespace NetHandlers
{
    bool SVC_Print_BitRead_Internal(bf_read& bitbuf, SourceGameContext& context, NetMsg::SVC_Print* data)
    {
        bitbuf.ReadString(data->text, sizeof(data->text));
        return !bitbuf.IsOverflowed();
    }

    bool SVC_Print_BitWrite_Internal(bf_write& bitbuf, const SourceGameContext& context, NetMsg::SVC_Print* data)
    {
        bitbuf.WriteString(data->text);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_Print_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_Print* data)
    {
        return true;
    }

    bool SVC_Print_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_Print* data)
    {
        return true;
    }

    void SVC_Print_ToString_Internal(std::ostringstream& out, NetMsg::SVC_Print* data)
    {
        out << "svc_Print: \"" << data->text << '"';
    }
}
