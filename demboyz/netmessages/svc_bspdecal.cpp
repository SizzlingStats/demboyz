
#include "svc_bspdecal.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"
#include "demofile/demojson.h"
#include "netcontants.h"

namespace NetHandlers
{
    bool SVC_BSPDecal_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_BSPDecal* data)
    {
        bitbuf.ReadBitVec3Coord(data->position);
        data->decalTextureIndex = bitbuf.ReadUBitLong(MAX_DECAL_INDEX_BITS);
        if (bitbuf.ReadOneBit() != 0)
        {
            data->entIndex = bitbuf.ReadUBitLong(MAX_EDICT_BITS);
            data->modelIndex = bitbuf.ReadUBitLong(SP_MODEL_INDEX_BITS);
        }
        else
        {
            data->entIndex = 0;
            data->modelIndex = 0;
        }
        data->lowPriority = bitbuf.ReadOneBit() != 0;
        return !bitbuf.IsOverflowed();
    }

    bool SVC_BSPDecal_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_BSPDecal* data)
    {
        bitbuf.WriteBitVec3Coord(data->position);
        bitbuf.WriteUBitLong(data->decalTextureIndex, MAX_DECAL_INDEX_BITS);
        if (data->entIndex != 0)
        {
            bitbuf.WriteOneBit(1);
            bitbuf.WriteUBitLong(data->entIndex, MAX_EDICT_BITS);
            bitbuf.WriteUBitLong(data->modelIndex, SP_MODEL_INDEX_BITS);
        }
        else
        {
            bitbuf.WriteOneBit(0);
        }
        bitbuf.WriteOneBit(data->lowPriority);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_BSPDecal_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_BSPDecal* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        DemoJsonReader::ReadVector(reader, "position", data->position);
        data->decalTextureIndex = reader.ReadUInt32("decalTextureIndex");
        data->entIndex = reader.ReadUInt32("entIndex");
        data->modelIndex = reader.ReadUInt32("modelIndex");
        data->lowPriority = reader.ReadBool("lowPriority");
        return !reader.HasReadError();
    }

    bool SVC_BSPDecal_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_BSPDecal* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        DemoJsonWriter::WriteVector(jsonbuf, "position", data->position);
        jsonbuf.WriteUInt32("decalTextureIndex", data->decalTextureIndex);
        jsonbuf.WriteUInt32("entIndex", data->entIndex);
        jsonbuf.WriteUInt32("modelIndex", data->modelIndex);
        jsonbuf.WriteBool("lowPriority", data->lowPriority);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void SVC_BSPDecal_ToString_Internal(std::ostringstream& out, NetMsg::SVC_BSPDecal* data)
    {
        out << "svc_BSPDecal: tex " << data->decalTextureIndex
            << ", ent " << data->entIndex
            << ", mod " << data->modelIndex
            << " lowpriority " << data->lowPriority;
    }
}
