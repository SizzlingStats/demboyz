
#pragma once

#include "nethandlers.h"

namespace NetMsg
{
    struct Net_SignonState
    {
        std::uint32_t spawnCount;
        std::uint8_t signonState;
    };
}

DECLARE_NET_HANDLERS(Net_SignonState);
