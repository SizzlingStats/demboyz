
#include "net_nop.h"

namespace NetHandlers
{
    bool Net_NOP_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::Net_NOP* data)
    {
        return true;
    }

    bool Net_NOP_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::Net_NOP* data)
    {
        return true;
    }

    bool Net_NOP_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::Net_NOP* data)
    {
        return true;
    }

    bool Net_NOP_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::Net_NOP* data)
    {
        return true;
    }

    void Net_NOP_ToString_Internal(std::ostringstream& out, NetMsg::Net_NOP* data)
    {
    }
}
