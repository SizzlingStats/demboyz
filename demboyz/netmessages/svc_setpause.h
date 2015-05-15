
#pragma once

#include "nethandlers.h"

namespace NetMsg
{
    struct SVC_SetPause
    {
        bool isPaused;
    };
}

DECLARE_NET_HANDLERS(SVC_SetPause);
