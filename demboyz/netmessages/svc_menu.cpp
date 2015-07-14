
#include "svc_menu.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"

using DialogType = NetMsg::SVC_Menu::DialogType;

#define KEYVALUES_TOKEN_SIZE 1024

static const char* KeyValuesBin_GetName(uint8_t* data, size_t dataLength)
{
    if (dataLength <= 2)
    {
        return nullptr;
    }
    return reinterpret_cast<const char*>(data + 1);
}

namespace NetHandlers
{
    bool SVC_Menu_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_Menu* data)
    {
        data->type = static_cast<DialogType>(bitbuf.ReadShort());
        data->dataLengthInBytes = bitbuf.ReadWord();
        data->menuBinaryKeyValues.reset(new uint8_t[data->dataLengthInBytes]);
        bitbuf.ReadBytes(data->menuBinaryKeyValues.get(), data->dataLengthInBytes);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_Menu_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_Menu* data)
    {
        bitbuf.WriteShort(static_cast<int16_t>(data->type));
        bitbuf.WriteWord(data->dataLengthInBytes);
        bitbuf.WriteBytes(data->menuBinaryKeyValues.get(), data->dataLengthInBytes);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_Menu_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_Menu* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        data->type = static_cast<DialogType>(reader.ReadInt32("dialogType"));
        data->dataLengthInBytes = reader.ReadUInt32("dataLengthInBytes");
        data->menuBinaryKeyValues.reset(new uint8_t[data->dataLengthInBytes]);
        reader.ReadBytes("data", data->menuBinaryKeyValues.get(), data->dataLengthInBytes);
        return !reader.HasReadError();
    }

    bool SVC_Menu_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_Menu* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteInt32("dialogType", static_cast<int16_t>(data->type));
        jsonbuf.WriteUInt32("dataLengthInBytes", data->dataLengthInBytes);
        jsonbuf.WriteBytes("data", data->menuBinaryKeyValues.get(), data->dataLengthInBytes);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void SVC_Menu_ToString_Internal(std::ostringstream& out, NetMsg::SVC_Menu* data)
    {
        // binary keyvalues in form [type][name][value]
        //                          [char][cstr][type]
        const char* name = KeyValuesBin_GetName(data->menuBinaryKeyValues.get(), data->dataLengthInBytes);
        out << "svc_Menu: " << static_cast<int16_t>(data->type)
            << " \"" << (name ? name : "No KeyValues")
            << "\" (len:" << data->dataLengthInBytes << ")";
    }
}
