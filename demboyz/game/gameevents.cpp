
#include "gameevents.h"
#include "base/bitfile.h"
#include <stdio.h>

namespace GameEvents
{
    EventDataMap ParseEventData(bf_read& bitbuf, const EventDescriptor& desc, std::vector<char>& stringMem)
    {
        stringMem.reserve(stringMem.size() + MAX_EVENT_BYTES);
        EventDataMap data;
        char tempStr[MAX_EVENT_BYTES];
        for (const EventValue& value : desc.values)
        {
            EventData& eventData = data[value.name];
            eventData.type = value.type;
            switch(value.type)
            {
            case EventValueType::String:
            {
                int length = 0;
                const bool ok = bitbuf.ReadString(tempStr, sizeof(tempStr), false, &length);
                assert(ok);
                length += 1; // for null terminator

                eventData.strOffset = stringMem.size();
                stringMem.insert(stringMem.end(), tempStr, tempStr + length + 1);
                break;
            }
            case EventValueType::Float:
            {
                eventData.flValue = bitbuf.ReadFloat();
                break;
            }
            case EventValueType::Long:
            {
                eventData.i32Value = bitbuf.ReadSBitLong(32);
                break;
            }
            case EventValueType::Short:
            {
                eventData.i16Value = bitbuf.ReadSBitLong(16);
                break;
            }
            case EventValueType::Byte:
            {
                eventData.u8Value = bitbuf.ReadUBitLong(8);
                break;
            }
            case EventValueType::Bool:
            {
                eventData.bValue = bitbuf.ReadOneBit() != 0;
                break;
            }
            case EventValueType::Local:
            default:
                assert(false);
                break;
            }
        }
        return data;
    }

    void PrintEventData(bf_read& bitbuf, const EventDescriptor& desc)
    {
        char tempStr[MAX_EVENT_BYTES];
        printf("%s:\n", desc.name);
        for (const EventValue& value : desc.values)
        {
            printf("  %s: ", value.name);
            switch(value.type)
            {
            case EventValueType::String:
            {
                const bool ok = bitbuf.ReadString(tempStr, sizeof(tempStr), false, nullptr);
                assert(ok);
                printf("%s\n", tempStr);
                break;
            }
            case EventValueType::Float:
            {
                printf("%f\n", bitbuf.ReadFloat());
                break;
            }
            case EventValueType::Long:
            {
                printf("%i\n", bitbuf.ReadSBitLong(32));
                break;
            }
            case EventValueType::Short:
            {
                printf("%i\n", bitbuf.ReadSBitLong(16));
                break;
            }
            case EventValueType::Byte:
            {
                printf("%u\n", bitbuf.ReadUBitLong(8));
                break;
            }
            case EventValueType::Bool:
            {
                printf("%s\n", (bitbuf.ReadOneBit() != 0) ? "true" : "false");
                break;
            }
            case EventValueType::Local:
            default:
                assert(false);
                break;
            }
        }
    }
}
