
#include "net_signonstate.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"

namespace NetHandlers
{
    bool Net_SignonState_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::Net_SignonState* data)
    {
        data->signonState = bitbuf.ReadByte();
        data->spawnCount = bitbuf.ReadLong();
        //assert(signonState >= SIGNONSTATE_NONE && signonState <= SIGNONSTATE_CHANGELEVEL);
        return !bitbuf.IsOverflowed();
    }

    bool Net_SignonState_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::Net_SignonState* data)
    {
        bitbuf.WriteByte(data->signonState);
        bitbuf.WriteLong(data->spawnCount);
        return !bitbuf.IsOverflowed();
    }

    bool Net_SignonState_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::Net_SignonState* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        data->signonState = reader.ReadUInt32("signonState");
        data->spawnCount = reader.ReadUInt32("spawnCount");
        return !reader.HasReadError();
    }

    bool Net_SignonState_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::Net_SignonState* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteUInt32("signonState", data->signonState);
        jsonbuf.WriteUInt32("spawnCount", data->spawnCount);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void Net_SignonState_ToString_Internal(std::ostringstream& out, NetMsg::Net_SignonState* data)
    {
        out << "net_SignonState: state " << static_cast<uint32_t>(data->signonState)
            << ", count " << data->spawnCount;
    }
}
