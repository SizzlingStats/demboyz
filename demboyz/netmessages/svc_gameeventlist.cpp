
#include "svc_gameeventlist.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"
#include "netcontants.h"
#include "netmath.h"

using EventDescriptor = NetMsg::SVC_GameEventList::EventDescriptor;
using EventValue = NetMsg::SVC_GameEventList::EventValue;

uint32_t CalculateNumDataBits(const std::vector<EventDescriptor>& eventDescriptors)
{
    uint32_t numBits = 0;
    for (const EventDescriptor& event : eventDescriptors)
    {
        numBits += MAX_EVENT_BITS;
        // +1 for null char
        numBits += (8 * (1 + strlen(event.name)));
        // +1 for null bits
        numBits += (3 * (1 + event.values.size()));
        for (const EventValue& value : event.values)
        {
            // +1 for null char
            numBits += (8 * (1 + strlen(value.name)));
        }
    }
    return numBits;
}

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
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());

        base::JsonReaderArray eventDescriptors = reader.ReadArray("eventDescriptors");
        eventDescriptors.TransformTo(data->eventDescriptors, [](base::JsonReaderObject& obj, EventDescriptor& event)
        {
            event.id = obj.ReadUInt32("id");
            obj.ReadString("name", event.name, sizeof(event.name));
            base::JsonReaderArray values = obj.ReadArray("values");
            values.TransformTo(event.values, [](base::JsonReaderObject& obj, EventValue& value)
            {
                value.type = obj.ReadUInt32("type");
                obj.ReadString("name", value.name, sizeof(value.name));
            });
        });
        data->dataLengthInBits = CalculateNumDataBits(data->eventDescriptors);
        return !reader.HasReadError();
    }

    bool SVC_GameEventList_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_GameEventList* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
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
        return jsonbuf.IsComplete();
    }

    void SVC_GameEventList_ToString_Internal(std::ostringstream& out, NetMsg::SVC_GameEventList* data)
    {
        out << "svc_GameEventList: number " << data->eventDescriptors.size()
            << ", bytes " << math::BitsToBytes(data->dataLengthInBits);
    }
}
