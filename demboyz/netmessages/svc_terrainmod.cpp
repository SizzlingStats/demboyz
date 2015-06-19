
#include "svc_terrainmod.h"
#include <cassert>

namespace NetHandlers
{
    bool SVC_TerrainMod_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_TerrainMod* data)
    {
        assert(false);
        return true;
    }

    bool SVC_TerrainMod_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_TerrainMod* data)
    {
        assert(false);
        return true;
    }

    bool SVC_TerrainMod_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_TerrainMod* data)
    {
        assert(false);
        return true;
    }

    bool SVC_TerrainMod_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_TerrainMod* data)
    {
        assert(false);
        return true;
    }

    void SVC_TerrainMod_ToString_Internal(std::ostringstream& out, NetMsg::SVC_TerrainMod* data)
    {
        out << "svc_TerrainMod";
    }
}
