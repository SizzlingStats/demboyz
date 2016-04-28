
#include "svc_gameevent.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"
#include "netcontants.h"
#include "netmath.h"

#ifdef WIP_GAMEEVENTS
#include "svc_gameeventlist.h"
#endif

namespace NetHandlers
{
    bool SVC_GameEvent_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_GameEvent* data)
    {
        const unsigned int numBits = bitbuf.ReadUBitLong(11);
        const size_t numBytes = math::BitsToBytes(numBits);

        data->dataLengthInBits = numBits;
        data->data.reset(new uint8_t[numBytes]);
        bitbuf.ReadBits(data->data.get(), numBits);

#ifdef WIP_GAMEEVENTS
        {
            BitRead bitbuf2(data->data.get(), numBytes, numBits);
            const size_t id = bitbuf2.ReadUBitLong(9);
            //std::vector<char> stringMem;
            //GameEvents::ParseEventData(bitbuf2, context.gameEventList->eventDescriptors[id], stringMem);
            GameEvents::PrintEventData(bitbuf2, context.gameEventList->eventDescriptors[id]);
            printf("%i\n", id);
        }
#endif // WIP_GAMEEVENTS

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
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        data->dataLengthInBits = reader.ReadUInt32("dataLengthInBits");
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        reader.ReadBits("data", data->data.get(), data->dataLengthInBits);
        return !reader.HasReadError();
    }

    bool SVC_GameEvent_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_GameEvent* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteUInt32("dataLengthInBits", data->dataLengthInBits);
        jsonbuf.WriteBits("data", data->data.get(), data->dataLengthInBits);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void SVC_GameEvent_ToString_Internal(std::ostringstream& out, NetMsg::SVC_GameEvent* data)
    {
        out << "svc_GameEvent: bytes " << math::BitsToBytes(data->dataLengthInBits);
    }
}
