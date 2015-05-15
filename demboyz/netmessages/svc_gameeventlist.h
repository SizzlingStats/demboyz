
#pragma once

#include "nethandlers.h"
#include "netcontants.h"
#include <vector>

namespace NetMsg
{
    struct SVC_GameEventList
    {
        struct EventValue
        {
            uint8_t type;
            char name[MAX_EVENT_NAME_LENGTH];
        };
        struct EventDescriptor
        {
            uint16_t id;
            char name[MAX_EVENT_NAME_LENGTH];
            std::vector<EventValue> values;
        };

        uint16_t dataLengthInBits;
        std::vector<EventDescriptor> eventDescriptors;
    };
}

DECLARE_NET_HANDLERS(SVC_GameEventList);
