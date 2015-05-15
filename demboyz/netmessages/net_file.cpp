
#include "net_file.h"
#include "bitbuf.h"

namespace NetHandlers
{
    bool Net_File_BitRead_Internal(bf_read& bitbuf, SourceGameContext& context, NetMsg::Net_File* data)
    {
        data->transferID = bitbuf.ReadUBitLong(32);
        bitbuf.ReadString(data->filename, sizeof(data->filename));
        data->isRequest = bitbuf.ReadOneBit() != 0;
        return !bitbuf.IsOverflowed();
    }

    bool Net_File_BitWrite_Internal(bf_write& bitbuf, SourceGameContext& context, NetMsg::Net_File* data)
    {
        bitbuf.WriteUBitLong(data->transferID, 32);
        bitbuf.WriteString(data->filename);
        bitbuf.WriteOneBit(data->isRequest);
        return !bitbuf.IsOverflowed();
    }

    bool Net_File_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::Net_File* data)
    {
        return true;
    }

    bool Net_File_JsonWrite_Internal(JsonWrite& jsonbuf, SourceGameContext& context, NetMsg::Net_File* data)
    {
        return true;
    }

    void Net_File_ToString_Internal(std::ostringstream& out, NetMsg::Net_File* data)
    {
        // nothing
    }
}
