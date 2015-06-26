
#include "dem_usercmd.h"
#include "demofile/demofile.h"
#include "demofile/demojson.h"

namespace DemHandlers
{
    bool Dem_UserCmd_FileRead_Internal(FileRead& demofile, DemMsg::Dem_UserCmd* data)
    {
        data->commandData = demofile.ReadUserCmd(data->commandNum, DemMsg::Dem_UserCmd::COMMANDDATA_MAX_LENGTH);
        return demofile.IsOk();
    }

    bool Dem_UserCmd_FileWrite_Internal(FileWrite& demofile, DemMsg::Dem_UserCmd* data)
    {
        demofile.WriteUserCmd(data->commandNum, data->commandData.begin(), data->commandData.length());
        return demofile.IsOk();
    }

    bool Dem_UserCmd_JsonRead_Internal(JsonRead& jsonbuf, DemMsg::Dem_UserCmd* data)
    {
        return true;
    }

    bool Dem_UserCmd_JsonWrite_Internal(JsonWrite& jsonbuf, DemMsg::Dem_UserCmd* data)
    {
        DemoJsonWriter::WriteUserCmd(jsonbuf, data->commandNum,
                                     data->commandData.begin(), data->commandData.length());
        return true;
    }
}
