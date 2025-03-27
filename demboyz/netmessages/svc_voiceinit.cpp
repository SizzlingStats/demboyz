
#include "svc_voiceinit.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"

namespace NetHandlers
{
    bool SVC_VoiceInit_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_VoiceInit* data)
    {
        bitbuf.ReadString(data->voiceCodec, sizeof(data->voiceCodec));
        data->quality = bitbuf.ReadByte();
        if(data->quality == NetMsg::SVC_VoiceInit::QUALITY_HAS_SAMPLE_RATE)
        {
            data->sampleRate = bitbuf.ReadShort();
        }
        else
        {
            data->sampleRate = 0;
        }
        return !bitbuf.IsOverflowed();
    }

    bool SVC_VoiceInit_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_VoiceInit* data)
    {
        bitbuf.WriteString(data->voiceCodec);
        bitbuf.WriteByte(data->quality);
        if(data->quality == NetMsg::SVC_VoiceInit::QUALITY_HAS_SAMPLE_RATE)
        {
            bitbuf.WriteShort(data->sampleRate);
        }
        return !bitbuf.IsOverflowed();
    }

    bool SVC_VoiceInit_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_VoiceInit* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        reader.ReadString("voiceCodec", data->voiceCodec, sizeof(data->voiceCodec));
        data->quality = reader.ReadUInt32("quality");
        if (data->quality == NetMsg::SVC_VoiceInit::QUALITY_HAS_SAMPLE_RATE)
        {
            data->sampleRate = reader.ReadInt32("sampleRate");
        }
        else
        {
            data->sampleRate = 0;
        }
        return !reader.HasReadError();
    }

    bool SVC_VoiceInit_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_VoiceInit* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteString("voiceCodec", data->voiceCodec);
        jsonbuf.WriteUInt32("quality", data->quality);
        if (data->quality == NetMsg::SVC_VoiceInit::QUALITY_HAS_SAMPLE_RATE)
        {
            jsonbuf.WriteInt32("sampleRate", data->sampleRate);
        }
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void SVC_VoiceInit_ToString_Internal(std::ostringstream& out, NetMsg::SVC_VoiceInit* data)
    {
        out << "svc_VoiceInit: codec \"" << data->voiceCodec;
        if(data->quality == NetMsg::SVC_VoiceInit::QUALITY_HAS_SAMPLE_RATE)
        {
            out << "\", sample rate " << static_cast<uint32_t>(data->sampleRate);
        }
        else
        {
            out << "\", qualitty " << static_cast<uint32_t>(data->quality);
        }
    }
}
