
#include "net_stringcmd.h"
#include "bitbuf.h"

namespace NetHandlers
{
    bool Net_StringCmd_BitRead_Internal(bf_read& bitbuf, SourceGameContext& context, NetMsg::Net_StringCmd* data)
    {
        bitbuf.ReadString(data->command, sizeof(data->command));
        return !bitbuf.IsOverflowed();
    }

    bool Net_StringCmd_BitWrite_Internal(bf_write& bitbuf, SourceGameContext& context, NetMsg::Net_StringCmd* data)
    {
        bitbuf.WriteString(data->command);
        return !bitbuf.IsOverflowed();
    }

    bool Net_StringCmd_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::Net_StringCmd* data)
    {
        return true;
    }

    bool Net_StringCmd_JsonWrite_Internal(JsonWrite& jsonbuf, SourceGameContext& context, NetMsg::Net_StringCmd* data)
    {
        return true;
    }

    void Net_StringCmd_ToString_Internal(std::ostringstream& out, NetMsg::Net_StringCmd* data)
    {
        out << "net_StringCmd: \"" << data->command << '"';
    }
}
