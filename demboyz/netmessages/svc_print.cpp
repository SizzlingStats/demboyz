
#include "svc_print.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"

namespace NetHandlers
{
    bool SVC_Print_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_Print* data)
    {
        bitbuf.ReadString(data->text, sizeof(data->text));
        return !bitbuf.IsOverflowed();
    }

    bool SVC_Print_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_Print* data)
    {
        bitbuf.WriteString(data->text);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_Print_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_Print* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        reader.ReadString("text", data->text, sizeof(data->text));
        return !reader.HasReadError();
    }

    bool SVC_Print_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_Print* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteString("text", data->text);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void SVC_Print_ToString_Internal(std::ostringstream& out, NetMsg::SVC_Print* data)
    {
        out << "svc_Print: \"" << data->text << '"';
    }
}
