
#include "svc_gameevent.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"
#include "netcontants.h"
#include "netmath.h"

namespace NetHandlers
{
    bool SVC_GameEvent_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_GameEvent* data)
    {
        data->dataLengthInBits = bitbuf.ReadUBitLong(11);
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        bitbuf.ReadBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_GameEvent_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_GameEvent* data)
    {
        bitbuf.WriteUBitLong(data->dataLengthInBits, 11);
        bitbuf.WriteBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_GameEvent_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_GameEvent* data)
    {
        return true;
    }

    bool SVC_GameEvent_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_GameEvent* data)
    {
        jsonbuf.StartObject("svc_gameevent");
        jsonbuf.WriteUInt32("dataLengthInBits", data->dataLengthInBits);
        jsonbuf.WriteBits("data", data->data.get(), data->dataLengthInBits);
        jsonbuf.EndObject();
        return true;
    }

    void SVC_GameEvent_ToString_Internal(std::ostringstream& out, NetMsg::SVC_GameEvent* data)
    {
        out << "svc_GameEvent: bytes " << math::BitsToBytes(data->dataLengthInBits);
    }
}
