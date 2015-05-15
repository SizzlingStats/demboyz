
#pragma once

#include "nethandlers.h"

namespace NetMsg
{
    struct SVC_SetView
    {
        uint16_t entIndex;
    };
}

DECLARE_NET_HANDLERS(SVC_SetView);
