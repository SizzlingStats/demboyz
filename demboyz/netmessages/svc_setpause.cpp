
#include "svc_setpause.h"
#include "bitbuf.h"

namespace NetHandlers
{
    bool SVC_SetPause_BitRead_Internal(bf_read& bitbuf, SourceGameContext& context, NetMsg::SVC_SetPause* data)
    {
        data->isPaused = bitbuf.ReadOneBit() != 0;
        return !bitbuf.IsOverflowed();
    }

    bool SVC_SetPause_BitWrite_Internal(bf_write& bitbuf, const SourceGameContext& context, NetMsg::SVC_SetPause* data)
    {
        bitbuf.WriteOneBit(data->isPaused);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_SetPause_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_SetPause* data)
    {
        return true;
    }

    bool SVC_SetPause_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_SetPause* data)
    {
        return true;
    }

    void SVC_SetPause_ToString_Internal(std::ostringstream& out, NetMsg::SVC_SetPause* data)
    {
        out << "svc_SetPause: " << (data->isPaused ? "paused" : "unpaused");
    }
}
