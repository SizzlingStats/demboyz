
#pragma once

#include "nethandlers.h"
#include "vector.h"

namespace NetMsg
{
    struct SVC_BSPDecal
    {
        Vector position;
        uint16_t decalTextureIndex;
        uint16_t entIndex;
        uint16_t modelIndex;
        bool lowPriority;
    };
}

DECLARE_NET_HANDLERS(SVC_BSPDecal);
