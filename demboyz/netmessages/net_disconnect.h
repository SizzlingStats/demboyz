
#pragma once

#include "nethandlers.h"

namespace NetMsg
{
    struct Net_Disconnect
    {
        char message[1024];
    };
}

DECLARE_NET_HANDLERS(Net_Disconnect);
