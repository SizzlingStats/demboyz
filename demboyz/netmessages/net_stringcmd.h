
#pragma once

#include "nethandlers.h"

namespace NetMsg
{
    struct Net_StringCmd
    {
        char command[1024];
    };
}

DECLARE_NET_HANDLERS(Net_StringCmd);
