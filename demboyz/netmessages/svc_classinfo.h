
#pragma once

#include "nethandlers.h"
#include <vector>

namespace NetMsg
{
    struct SVC_ClassInfo
    {
        typedef struct class_s
        {
            uint32_t classID;
            char className[256];
            char dataTableName[256];
        } class_t;

        int16_t numServerClasses;
        bool createOnClient;
        std::vector<class_t> serverClasses;
    };
}

DECLARE_NET_HANDLERS(SVC_ClassInfo);
