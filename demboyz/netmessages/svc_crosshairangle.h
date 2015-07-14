
#pragma once

#include "nethandlers.h"
#include "sourcesdk/vector.h"

namespace NetMsg
{
    struct SVC_CrosshairAngle
    {
        QAngle angle;
    };
}

DECLARE_NET_HANDLERS(SVC_CrosshairAngle);
