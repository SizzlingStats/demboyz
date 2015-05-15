
#pragma once

#include "nethandlers.h"

namespace NetMsg
{
    struct SVC_GetCvarValue
    {
        int32_t cookie;
        char cvarName[256];
    };
}

DECLARE_NET_HANDLERS(SVC_GetCvarValue);
