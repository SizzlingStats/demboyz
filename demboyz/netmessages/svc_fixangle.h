
#pragma once

#include "nethandlers.h"
#include "sourcesdk/vector.h"

namespace NetMsg
{
    struct SVC_FixAngle
    {
        bool relative;
        QAngle angle;
    };
}

DECLARE_NET_HANDLERS(SVC_FixAngle);
