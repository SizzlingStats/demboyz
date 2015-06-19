
#include "net_signonstate.h"
#include "sourcesdk/bitbuf.h"

namespace NetHandlers
{
    bool Net_SignonState_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::Net_SignonState* data)
    {
        data->signonState = bitbuf.ReadByte();
        data->spawnCount = bitbuf.ReadLong();
        //assert(signonState >= SIGNONSTATE_NONE && signonState <= SIGNONSTATE_CHANGELEVEL);
        return !bitbuf.IsOverflowed();
    }

    bool Net_SignonState_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::Net_SignonState* data)
    {
        bitbuf.WriteByte(data->signonState);
        bitbuf.WriteLong(data->spawnCount);
        return !bitbuf.IsOverflowed();
    }

    bool Net_SignonState_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::Net_SignonState* data)
    {
        return true;
    }

    bool Net_SignonState_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::Net_SignonState* data)
    {
        return true;
    }

    void Net_SignonState_ToString_Internal(std::ostringstream& out, NetMsg::Net_SignonState* data)
    {
        out << "net_SignonState: state " << static_cast<uint32_t>(data->signonState)
            << ", count " << data->spawnCount;
    }
}
