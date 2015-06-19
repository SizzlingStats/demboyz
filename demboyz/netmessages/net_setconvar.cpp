
#include "net_setconvar.h"
#include "sourcesdk/bitbuf.h"

namespace NetHandlers
{
    using cvar_t = NetMsg::Net_SetConVar::cvar_t;

    bool Net_SetConVar_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::Net_SetConVar* data)
    {
        data->cvars.resize(bitbuf.ReadByte());
        for (cvar_t& cvar : data->cvars)
        {
            bitbuf.ReadString(cvar.name, sizeof(cvar.name));
            bitbuf.ReadString(cvar.value, sizeof(cvar.value));
        }
        return !bitbuf.IsOverflowed();
    }

    bool Net_SetConVar_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::Net_SetConVar* data)
    {
        bitbuf.WriteByte(data->cvars.size());
        for (cvar_t& cvar : data->cvars)
        {
            bitbuf.WriteString(cvar.name);
            bitbuf.WriteString(cvar.value);
        }
        return !bitbuf.IsOverflowed();
    }

    bool Net_SetConVar_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::Net_SetConVar* data)
    {
        return true;
    }

    bool Net_SetConVar_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::Net_SetConVar* data)
    {
        return true;
    }

    void Net_SetConVar_ToString_Internal(std::ostringstream& out, NetMsg::Net_SetConVar* data)
    {
        cvar_t cvar = data->cvars[0];
        out << "net_SetConVar: " << data->cvars.size() << " cvars, \"" << cvar.name  << "\"=\"" << cvar.value << '"';
    }
}
