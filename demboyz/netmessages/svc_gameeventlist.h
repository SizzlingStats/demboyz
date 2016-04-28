
#pragma once

#include "nethandlers.h"
#include "netcontants.h"
#include "game/gameevents.h"
#include <vector>

namespace NetMsg
{
    struct SVC_GameEventList
    {
        std::vector<GameEvents::EventDescriptor> eventDescriptors;
        uint32_t dataLengthInBits;
    };
}

DECLARE_NET_HANDLERS(SVC_GameEventList);
