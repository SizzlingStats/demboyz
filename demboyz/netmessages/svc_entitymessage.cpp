
#include "svc_entitymessage.h"
#include "sourcesdk/bitbuf.h"
#include "netcontants.h"
#include "netmath.h"

namespace NetHandlers
{
    bool SVC_EntityMessage_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_EntityMessage* data)
    {
        data->entIndex = bitbuf.ReadUBitLong(MAX_EDICT_BITS);
        data->classID = bitbuf.ReadUBitLong(MAX_SERVER_CLASS_BITS);
        data->dataLengthInBits = bitbuf.ReadUBitLong(11);
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        bitbuf.ReadBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_EntityMessage_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_EntityMessage* data)
    {
        bitbuf.WriteUBitLong(data->entIndex, MAX_EDICT_BITS);
        bitbuf.WriteUBitLong(data->classID, MAX_SERVER_CLASS_BITS);
        bitbuf.WriteUBitLong(data->dataLengthInBits, 11);
        bitbuf.WriteBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_EntityMessage_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_EntityMessage* data)
    {
        return true;
    }

    bool SVC_EntityMessage_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_EntityMessage* data)
    {
        return true;
    }

    void SVC_EntityMessage_ToString_Internal(std::ostringstream& out, NetMsg::SVC_EntityMessage* data)
    {
        out << "svc_EntityMessage: entity " << data->entIndex
            << ", class " << data->classID
            << ", bytes " << math::BitsToBytes(data->dataLengthInBits);
    }
}
