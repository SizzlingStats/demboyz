
#include "dem_datatables.h"
#include "demofile/demofile.h"

namespace DemHandlers
{
    bool Dem_DataTables_FileRead_Internal(DemoFileReader& demofile, DemMsg::Dem_DataTables* data)
    {
        data->data = demofile.ReadRawData(DemMsg::Dem_DataTables::DATA_MAX_LENGTH);
        return demofile.IsOk();
    }

    bool Dem_DataTables_FileWrite_Internal(DemoFileWriter& demofile, DemMsg::Dem_DataTables* data)
    {
        demofile.WriteRawData(data->data.begin(), data->data.length());
        return demofile.IsOk();
    }

    bool Dem_DataTables_JsonRead_Internal(JsonRead& jsonbuf, DemMsg::Dem_DataTables* data)
    {
        return true;
    }

    bool Dem_DataTables_JsonWrite_Internal(JsonWrite& jsonbuf, DemMsg::Dem_DataTables* data)
    {
        return true;
    }
}
