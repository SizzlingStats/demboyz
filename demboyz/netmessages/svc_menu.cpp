
#include "svc_menu.h"
#include "bitbuf.h"

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
    bool SVC_Menu_BitRead_Internal(bf_read& bitbuf, SourceGameContext& context, NetMsg::SVC_Menu* data)
    {
        data->type = static_cast<DialogType>(bitbuf.ReadShort());
        data->dataLengthInBytes = bitbuf.ReadWord();
        data->menuBinaryKeyValues.reset(new uint8_t[data->dataLengthInBytes]);
        bitbuf.ReadBytes(data->menuBinaryKeyValues.get(), data->dataLengthInBytes);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_Menu_BitWrite_Internal(bf_write& bitbuf, SourceGameContext& context, NetMsg::SVC_Menu* data)
    {
        bitbuf.WriteShort(static_cast<uint16_t>(data->type));
        bitbuf.WriteWord(data->dataLengthInBytes);
        bitbuf.WriteBytes(data->menuBinaryKeyValues.get(), data->dataLengthInBytes);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_Menu_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_Menu* data)
    {
        return true;
    }

    bool SVC_Menu_JsonWrite_Internal(JsonWrite& jsonbuf, SourceGameContext& context, NetMsg::SVC_Menu* data)
    {
        return true;
    }

    void SVC_Menu_ToString_Internal(std::ostringstream& out, NetMsg::SVC_Menu* data)
    {
        // binary keyvalues in form [type][name][value]
        //                          [char][cstr][type]
        const char* name = KeyValuesBin_GetName(data->menuBinaryKeyValues.get(), data->dataLengthInBytes);
        out << "svc_Menu: " << static_cast<uint16_t>(data->type)
            << " \"" << (name ? name : "No KeyValues")
            << "\" (len:" << data->dataLengthInBytes << ")";
    }
}
