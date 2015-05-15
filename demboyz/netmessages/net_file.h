
#pragma once

#include "nethandlers.h"

namespace NetMsg
{
    struct Net_File
    {
        std::uint32_t transferID;
        bool isRequest;
        char filename[1024];
    };
}

DECLARE_NET_HANDLERS(Net_File);
