
#include "dem_datatables.h"
#include "demofile/demofile.h"
#include "demofile/demojson.h"

namespace DemHandlers
{
    bool Dem_DataTables_FileRead_Internal(FileRead& demofile, DemMsg::Dem_DataTables* data)
    {
        data->data = demofile.ReadRawData(DemMsg::Dem_DataTables::DATA_MAX_LENGTH);
        return demofile.IsOk();
    }

    bool Dem_DataTables_FileWrite_Internal(FileWrite& demofile, DemMsg::Dem_DataTables* data)
    {
        demofile.WriteRawData(data->data.begin(), data->data.length());
        return demofile.IsOk();
    }

    bool Dem_DataTables_JsonRead_Internal(JsonRead& jsonbuf, DemMsg::Dem_DataTables* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());

        data->data.reset(reader.ReadBytes("data", nullptr, 0));
        reader.ReadBytes("data", data->data.begin(), DemMsg::Dem_DataTables::DATA_MAX_LENGTH);
        return !reader.HasReadError();
    }

    bool Dem_DataTables_JsonWrite_Internal(JsonWrite& jsonbuf, DemMsg::Dem_DataTables* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteBytes("data", data->data.begin(), data->data.length());
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }
}
