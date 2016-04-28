
#include "sourcecontext.h"
#include "netmessages/svc_gameeventlist.h"

SourceGameContext::SourceGameContext():
    protocol(0),
    gameEventList(nullptr)
{
}

SourceGameContext::~SourceGameContext()
{
    protocol = 0;
    delete gameEventList;
    gameEventList = nullptr;
}
