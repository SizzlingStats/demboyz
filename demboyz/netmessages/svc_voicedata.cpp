
#include "svc_voicedata.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"
#include "netmath.h"

namespace NetHandlers
{
    bool SVC_VoiceData_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_VoiceData* data)
    {
        data->fromClientIndex = bitbuf.ReadByte();
        data->proximity = !!bitbuf.ReadByte();
        data->dataLengthInBits = bitbuf.ReadWord();
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        bitbuf.ReadBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_VoiceData_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_VoiceData* data)
    {
        bitbuf.WriteByte(data->fromClientIndex);
        bitbuf.WriteByte(data->proximity);
        bitbuf.WriteWord(data->dataLengthInBits);
        bitbuf.WriteBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_VoiceData_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_VoiceData* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        data->fromClientIndex = reader.ReadUInt32("fromClientIndex");
        data->proximity = reader.ReadBool("proximity");
        data->dataLengthInBits = reader.ReadUInt32("dataLengthInBits");
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        reader.ReadBits("data", data->data.get(), data->dataLengthInBits);
        return !reader.HasReadError();
    }

    bool SVC_VoiceData_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_VoiceData* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteUInt32("fromClientIndex", data->fromClientIndex);
        jsonbuf.WriteBool("proximity", data->proximity);
        jsonbuf.WriteUInt32("dataLengthInBits", data->dataLengthInBits);
        jsonbuf.WriteBits("data", data->data.get(), data->dataLengthInBits);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void SVC_VoiceData_ToString_Internal(std::ostringstream& out, NetMsg::SVC_VoiceData* data)
    {
        out << "svc_VoiceData: client " << static_cast<uint32_t>(data->fromClientIndex)
            << ", bytes " << math::BitsToBytes(data->dataLengthInBits);
    }
}
