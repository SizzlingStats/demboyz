
#pragma once

#include "nethandlers.h"

namespace NetMsg
{
    struct Net_Tick
    {
        static const std::uint32_t NET_TICK_SCALEUP = 100000;

        std::int32_t tick;
        std::uint16_t hostFrameTime;
        std::uint16_t hostFrameTimeStdDev;
    };
}

DECLARE_NET_HANDLERS(Net_Tick);
