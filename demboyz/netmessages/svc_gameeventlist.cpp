
#include "svc_gameeventlist.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"
#include "netcontants.h"
#include "netmath.h"

using EventDescriptor = NetMsg::SVC_GameEventList::EventDescriptor;
using EventValue = NetMsg::SVC_GameEventList::EventValue;

namespace NetHandlers
{
    bool SVC_GameEventList_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_GameEventList* data)
    {
        data->eventDescriptors.resize(bitbuf.ReadUBitLong(MAX_EVENT_BITS));
        data->dataLengthInBits = bitbuf.ReadUBitLong(20);
        for (EventDescriptor& event : data->eventDescriptors)
        {
            event.id = bitbuf.ReadUBitLong(MAX_EVENT_BITS);
            bitbuf.ReadString(event.name, sizeof(event.name));
            EventValue value;
            while ((value.type = bitbuf.ReadUBitLong(3)) > 0)
            {
                bitbuf.ReadString(value.name, sizeof(value.name));
                event.values.push_back(value);
            }
            event.values.shrink_to_fit();
        }
        return !bitbuf.IsOverflowed();
    }

    bool SVC_GameEventList_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_GameEventList* data)
    {
        bitbuf.WriteUBitLong(data->eventDescriptors.size(), MAX_EVENT_BITS);
        bitbuf.WriteUBitLong(data->dataLengthInBits, 20);
        for (EventDescriptor& event : data->eventDescriptors)
        {
            bitbuf.WriteUBitLong(event.id, MAX_EVENT_BITS);
            bitbuf.WriteString(event.name);
            for (EventValue& value : event.values)
            {
                bitbuf.WriteUBitLong(value.type, 3);
                bitbuf.WriteString(value.name);
            }
            bitbuf.WriteUBitLong(0, 3);
        }
        return !bitbuf.IsOverflowed();
    }

    bool SVC_GameEventList_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_GameEventList* data)
    {
        return true;
    }

    bool SVC_GameEventList_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_GameEventList* data)
    {
        jsonbuf.StartObject("svc_gameeventlist");
        jsonbuf.StartArray("eventDescriptors");
        for (const EventDescriptor& event : data->eventDescriptors)
        {
            jsonbuf.StartObject();
            jsonbuf.WriteUInt32("id", event.id);
            jsonbuf.WriteString("name", event.name);
            jsonbuf.StartArray("values");
            for (const EventValue& value : event.values)
            {
                jsonbuf.StartObject();
                jsonbuf.WriteUInt32("type", value.type);
                jsonbuf.WriteString("name", value.name);
                jsonbuf.EndObject();
            }
            jsonbuf.EndArray();
            jsonbuf.EndObject();
        }
        jsonbuf.EndArray();
        jsonbuf.EndObject();
        return true;
    }

    void SVC_GameEventList_ToString_Internal(std::ostringstream& out, NetMsg::SVC_GameEventList* data)
    {
        out << "svc_GameEventList: number " << data->eventDescriptors.size()
            << ", bytes " << math::BitsToBytes(data->dataLengthInBits);
    }
}
