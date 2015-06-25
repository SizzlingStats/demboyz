
#include "svc_sounds.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"
#include "netmath.h"

namespace NetHandlers
{
    bool SVC_Sounds_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_Sounds* data)
    {
        data->reliableSound = bitbuf.ReadOneBit() != 0;
        if (data->reliableSound)
        {
            data->numSounds = 1;
            data->dataLengthInBits = bitbuf.ReadUBitLong(8);
        }
        else
        {
            data->numSounds = bitbuf.ReadUBitLong(8);
            data->dataLengthInBits = bitbuf.ReadUBitLong(16);
        }
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        bitbuf.ReadBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_Sounds_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_Sounds* data)
    {
        if (data->reliableSound)
        {
            bitbuf.WriteOneBit(1);
            bitbuf.WriteUBitLong(data->dataLengthInBits, 8);
        }
        else
        {
            bitbuf.WriteOneBit(0);
            bitbuf.WriteUBitLong(data->numSounds, 8);
            bitbuf.WriteUBitLong(data->dataLengthInBits, 16);
        }
        bitbuf.WriteBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_Sounds_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_Sounds* data)
    {
        return true;
    }

    bool SVC_Sounds_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_Sounds* data)
    {
        jsonbuf.StartObject("svc_sounds");
        jsonbuf.WriteBool("reliableSound", data->reliableSound);
        jsonbuf.WriteUInt32("numSounds", data->numSounds);
        jsonbuf.WriteUInt32("dataLengthInBits", data->dataLengthInBits);
        jsonbuf.WriteBits("data", data->data.get(), data->dataLengthInBits);
        jsonbuf.EndObject();
        return true;
    }

    void SVC_Sounds_ToString_Internal(std::ostringstream& out, NetMsg::SVC_Sounds* data)
    {
        out << "svc_Sounds: number " << static_cast<uint32_t>(data->numSounds)
            << (data->reliableSound ? ", reliable" : "")
            << ", bytes " << math::BitsToBytes(data->dataLengthInBits);
    }
}
