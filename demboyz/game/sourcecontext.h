
#pragma once

#include <cstdint>

namespace NetMsg
{
    struct SVC_GameEventList;
}

struct SourceGameContext
{
    SourceGameContext();
    ~SourceGameContext();

    int16_t protocol;
    NetMsg::SVC_GameEventList* gameEventList;
};
