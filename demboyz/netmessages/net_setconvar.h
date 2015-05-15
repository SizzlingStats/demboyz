
#pragma once

#include "nethandlers.h"
#include <vector>

namespace NetMsg
{
    struct Net_SetConVar
    {
        typedef struct cvar_s
        {
            char name[MAX_OSPATH];
            char value[MAX_OSPATH];
        } cvar_t;

        std::vector<cvar_t> cvars;
    };
}

DECLARE_NET_HANDLERS(Net_SetConVar);
