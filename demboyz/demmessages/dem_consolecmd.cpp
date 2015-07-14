
#include "dem_consolecmd.h"
#include "demofile/demofile.h"
#include "base/jsonfile.h"

namespace DemHandlers
{
    bool Dem_ConsoleCmd_FileRead_Internal(FileRead& demofile, DemMsg::Dem_ConsoleCmd* data)
    {
        char command[DemMsg::Dem_ConsoleCmd::COMMAND_MAX_LENGTH];
        demofile.ReadRawData(reinterpret_cast<uint8_t*>(command), sizeof(command));
        data->command.assign(command);
        return demofile.IsOk();
    }

    bool Dem_ConsoleCmd_FileWrite_Internal(FileWrite& demofile, DemMsg::Dem_ConsoleCmd* data)
    {
        const uint8_t* command = reinterpret_cast<const uint8_t*>(data->command.data());
        demofile.WriteRawData(command, data->command.length() + 1);
        return demofile.IsOk();
    }

    bool Dem_ConsoleCmd_JsonRead_Internal(JsonRead& jsonbuf, DemMsg::Dem_ConsoleCmd* data)
    {
        return true;
    }

    bool Dem_ConsoleCmd_JsonWrite_Internal(JsonWrite& jsonbuf, DemMsg::Dem_ConsoleCmd* data)
    {
        jsonbuf.WriteString("command", data->command);
        return true;
    }
}
