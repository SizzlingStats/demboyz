
#pragma once

#include "demhandlers.h"
#include <string>

namespace DemMsg
{
    struct Dem_ConsoleCmd
    {
        static const int COMMAND_MAX_LENGTH = 1024;
        std::string command;
    };
}

DECLARE_DEM_HANDLERS(Dem_ConsoleCmd);
