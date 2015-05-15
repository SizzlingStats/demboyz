
#include "net_disconnect.h"
#include "bitbuf.h"

namespace NetHandlers
{
    bool Net_Disconnect_BitRead_Internal(bf_read& bitbuf, SourceGameContext& context, NetMsg::Net_Disconnect* data)
    {
        bitbuf.ReadString(data->message, sizeof(data->message));
        return !bitbuf.IsOverflowed();
    }

    bool Net_Disconnect_BitWrite_Internal(bf_write& bitbuf, const SourceGameContext& context, NetMsg::Net_Disconnect* data)
    {
        bitbuf.WriteString(data->message);
        return !bitbuf.IsOverflowed();
    }

    bool Net_Disconnect_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::Net_Disconnect* data)
    {
        return true;
    }

    bool Net_Disconnect_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::Net_Disconnect* data)
    {
        return true;
    }

    void Net_Disconnect_ToString_Internal(std::ostringstream& out, NetMsg::Net_Disconnect* data)
    {
        // nothing
    }
}
