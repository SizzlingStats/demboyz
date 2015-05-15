
#include "net_tick.h"
#include "bitbuf.h"

namespace NetHandlers
{
    bool Net_Tick_BitRead_Internal(bf_read& bitbuf, SourceGameContext& context, NetMsg::Net_Tick* data)
    {
        data->tick = bitbuf.ReadUBitLong(32);
        data->hostFrameTime = bitbuf.ReadUBitLong(16);
        data->hostFrameTimeStdDev = bitbuf.ReadUBitLong(16);
        return !bitbuf.IsOverflowed();
    }

    bool Net_Tick_BitWrite_Internal(bf_write& bitbuf, SourceGameContext& context, NetMsg::Net_Tick* data)
    {
        bitbuf.WriteUBitLong(data->tick, 32);
        bitbuf.WriteUBitLong(data->hostFrameTime, 16);
        bitbuf.WriteUBitLong(data->hostFrameTimeStdDev, 16);
        return !bitbuf.IsOverflowed();
    }

    bool Net_Tick_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::Net_Tick* data)
    {
        return true;
    }

    bool Net_Tick_JsonWrite_Internal(JsonWrite& jsonbuf, SourceGameContext& context, NetMsg::Net_Tick* data)
    {
        return true;
    }

    void Net_Tick_ToString_Internal(std::ostringstream& out, NetMsg::Net_Tick* data)
    {
        out << "net_Tick: tick " << data->tick;
    }
}
