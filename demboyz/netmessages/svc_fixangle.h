
#pragma once

#include "nethandlers.h"

namespace NetMsg
{
    struct SVC_FixAngle
    {
        bool relative;
        float x;
        float y;
        float z;
    };
}

DECLARE_NET_HANDLERS(SVC_FixAngle);
