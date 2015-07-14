
#include "net_disconnect.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"

namespace NetHandlers
{
    bool Net_Disconnect_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::Net_Disconnect* data)
    {
        bitbuf.ReadString(data->message, sizeof(data->message));
        return !bitbuf.IsOverflowed();
    }

    bool Net_Disconnect_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::Net_Disconnect* data)
    {
        bitbuf.WriteString(data->message);
        return !bitbuf.IsOverflowed();
    }

    bool Net_Disconnect_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::Net_Disconnect* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        reader.ReadString("message", data->message, sizeof(data->message));
        return !reader.HasReadError();
    }

    bool Net_Disconnect_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::Net_Disconnect* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteString("message", data->message);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void Net_Disconnect_ToString_Internal(std::ostringstream& out, NetMsg::Net_Disconnect* data)
    {
        // nothing
    }
}
