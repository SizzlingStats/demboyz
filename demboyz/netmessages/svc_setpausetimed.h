
#pragma once

#include "nethandlers.h"

namespace NetMsg
{
    struct SVC_SetPauseTimed
    {
        bool isPaused;
        float time;
    };
}

DECLARE_NET_HANDLERS(SVC_SetPauseTimed);
