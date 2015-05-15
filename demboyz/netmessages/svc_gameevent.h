
#pragma once

#include "nethandlers.h"
#include <memory>

namespace NetMsg
{
    struct SVC_GameEvent
    {
        std::unique_ptr<uint8_t[]> data;
        uint16_t dataLengthInBits;
    };
}

DECLARE_NET_HANDLERS(SVC_GameEvent);
