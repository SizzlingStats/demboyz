
#pragma once

#include "nethandlers.h"
#include <memory>

namespace NetMsg
{
    struct SVC_Menu
    {
        enum class DialogType : uint16_t
        {
            // just an on screen message
            DIALOG_MSG = 0,
            // an options menu
            DIALOG_MENU,
            // a richtext dialog
            DIALOG_TEXT,
            // an entry box
            DIALOG_ENTRY,
            // Ask the client to connect to a specified IP address.
            // Only the "time" and "title" keys are used.
            DIALOG_ASKCONNECT
        };

        std::unique_ptr<uint8_t[]> menuBinaryKeyValues;
        uint16_t dataLengthInBytes;
        DialogType type;
    };
}

DECLARE_NET_HANDLERS(SVC_Menu);
