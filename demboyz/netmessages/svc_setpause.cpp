
#include "svc_setpause.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"

namespace NetHandlers
{
    bool SVC_SetPause_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_SetPause* data)
    {
        data->isPaused = bitbuf.ReadOneBit() != 0;
        return !bitbuf.IsOverflowed();
    }

    bool SVC_SetPause_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_SetPause* data)
    {
        bitbuf.WriteOneBit(data->isPaused);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_SetPause_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_SetPause* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        data->isPaused = reader.ReadBool("isPaused");
        return !reader.HasReadError();
    }

    bool SVC_SetPause_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_SetPause* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteBool("isPaused", data->isPaused);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void SVC_SetPause_ToString_Internal(std::ostringstream& out, NetMsg::SVC_SetPause* data)
    {
        out << "svc_SetPause: " << (data->isPaused ? "paused" : "unpaused");
    }
}
