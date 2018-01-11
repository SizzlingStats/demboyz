
#include "svc_setpausetimed.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"

namespace NetHandlers
{
    bool SVC_SetPauseTimed_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_SetPauseTimed* data)
    {
        data->isPaused = bitbuf.ReadOneBit() != 0;
        data->time = bitbuf.ReadFloat();
        return !bitbuf.IsOverflowed();
    }

    bool SVC_SetPauseTimed_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_SetPauseTimed* data)
    {
        bitbuf.WriteOneBit(data->isPaused);
        bitbuf.WriteFloat(data->time);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_SetPauseTimed_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_SetPauseTimed* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        data->isPaused = reader.ReadBool("isPaused");
        data->time = reader.ReadFloat("time");
        return !reader.HasReadError();
    }

    bool SVC_SetPauseTimed_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_SetPauseTimed* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteBool("isPaused", data->isPaused);
        jsonbuf.WriteFloat("time", data->time);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void SVC_SetPauseTimed_ToString_Internal(std::ostringstream& out, NetMsg::SVC_SetPauseTimed* data)
    {
        out << "svc_SetPauseTimed: " << (data->isPaused ? "paused" : "unpaused");
    }
}
