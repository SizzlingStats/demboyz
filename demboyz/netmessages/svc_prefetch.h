
#pragma once

#include "nethandlers.h"

namespace NetMsg
{
    struct SVC_Prefetch
    {
        enum Types
        {
            SOUND = 0
        };
        uint16_t type;
        uint16_t soundIndex;
    };
}

DECLARE_NET_HANDLERS(SVC_Prefetch);
