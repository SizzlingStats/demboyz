
#pragma once

#include "nethandlers.h"

namespace NetMsg
{
    struct SVC_Print
    {
        char text[2048];
    };
}

DECLARE_NET_HANDLERS(SVC_Print);
