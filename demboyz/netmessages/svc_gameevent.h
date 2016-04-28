
#pragma once

#include "nethandlers.h"
#include "game/gameevents.h"
#include <memory>

namespace NetMsg
{
    struct SVC_GameEvent
    {
        GameEvents::EventDataMap eventData;
        std::unique_ptr<uint8_t[]> data;
        uint16_t dataLengthInBits;
    };
}

DECLARE_NET_HANDLERS(SVC_GameEvent);
