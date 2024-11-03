
#include "rocketmodefixer.h"

#include "base/array.h"
#include "demofile/demotypes.h"
#include "demmessages/dem_datatables.h"
#include "netmessages/netmessages.h"
#include "netmessages/svc_sendtable.h"

#define SPROP_NUMFLAGBITS_NETWORKED 16

#define PROPINFOBITS_NUMPROPS 10
#define PROPINFOBITS_TYPE 5
#define PROPINFOBITS_FLAGS SPROP_NUMFLAGBITS_NETWORKED
#define PROPINFOBITS_STRINGBUFFERLEN 10
#define PROPINFOBITS_NUMBITS 7
#define PROPINFOBITS_RIGHTSHIFT 6
#define PROPINFOBITS_NUMELEMENTS 10

#define SPROP_EXCLUDE (1<<6)

struct SendProp;

enum SendPropType
{
    DPT_Int = 0,
    DPT_Float,
    DPT_Vector,
    DPT_VectorXY,
    DPT_String,
    DPT_Array,
    DPT_DataTable,
    DPT_NUMSendPropTypes
};

struct SendTable
{
    Array<SendProp> mProps;
    int mNumProps;
    char mTableName[256];
};

struct SendProp
{
    SendProp()
    {
        mExcludeDTName[0] = '\0';
        mVarName[0] = '\0';
    }

    SendPropType mType;
    int mBits;
    float mLowValue;
    float mHighValue;
    int mNumElements;
    int mFlags;
    char mExcludeDTName[256];
    char mVarName[256];
};

static bool PeekTableName(bf_read buf, char* tableName, int maxTableNameLength)
{
    return buf.ReadString(tableName, maxTableNameLength);
}

static bool ReadSendTable(bf_read* buf, SendTable& table)
{
    buf->ReadString(table.mTableName, sizeof(table.mTableName));
    const int numProps = buf->ReadUBitLong(PROPINFOBITS_NUMPROPS);
    table.mNumProps = numProps;
    table.mProps.reset(numProps);
    for (int i = 0; i < numProps; ++i)
    {
        SendProp& prop = table.mProps[i];
        prop.mType = static_cast<SendPropType>(buf->ReadUBitLong(PROPINFOBITS_TYPE));
        buf->ReadString(prop.mVarName, sizeof(prop.mVarName));

        prop.mFlags = buf->ReadUBitLong(PROPINFOBITS_FLAGS);

        if (prop.mType == DPT_DataTable || ((prop.mFlags & SPROP_EXCLUDE) != 0))
        {
            buf->ReadString(prop.mExcludeDTName, sizeof(prop.mExcludeDTName));
        }
        else if (prop.mType == DPT_Array)
        {
            prop.mNumElements = buf->ReadUBitLong(PROPINFOBITS_NUMELEMENTS);
        }
        else
        {
            prop.mLowValue = buf->ReadBitFloat();
            prop.mHighValue = buf->ReadBitFloat();
            prop.mBits = buf->ReadUBitLong(PROPINFOBITS_NUMBITS);
        }
    }
    return !buf->IsOverflowed();
}

static bool ModifyTFBaseRocketAngRotation(bf_read* buf, SendTable& table)
{
    buf->ReadString(table.mTableName, sizeof(table.mTableName));
    const int numProps = buf->ReadUBitLong(PROPINFOBITS_NUMPROPS);
    table.mNumProps = numProps;
    table.mProps.reset(numProps);
    for (int i = 0; i < numProps; ++i)
    {
        SendProp& prop = table.mProps[i];
        prop.mType = static_cast<SendPropType>(buf->ReadUBitLong(PROPINFOBITS_TYPE));
        buf->ReadString(prop.mVarName, sizeof(prop.mVarName));

        prop.mFlags = buf->ReadUBitLong(PROPINFOBITS_FLAGS);

        if (prop.mType == DPT_DataTable || ((prop.mFlags & SPROP_EXCLUDE) != 0))
        {
            buf->ReadString(prop.mExcludeDTName, sizeof(prop.mExcludeDTName));
        }
        else if (prop.mType == DPT_Array)
        {
            prop.mNumElements = buf->ReadUBitLong(PROPINFOBITS_NUMELEMENTS);
        }
        else
        {
            prop.mLowValue = buf->ReadBitFloat();
            prop.mHighValue = buf->ReadBitFloat();

            const bool angRotationProp = !strcmp(prop.mVarName, "m_angRotation");
            if (angRotationProp)
            {
                static_assert(PROPINFOBITS_NUMBITS == 7);

                // modify the next PROPINFOBITS_NUMBITS bits.
                unsigned char* base = (unsigned char*)buf->GetBasePointer();
                const int curBit = buf->GetNumBitsRead();
                const int curDwordRoundedDown = (curBit >> 5);
                const int offsetBits = (curBit & 31);

                bf_write writeBuf(base + (curDwordRoundedDown * 4), 4, 32);
                writeBuf.SeekToBit(offsetBits);
                writeBuf.WriteUBitLong(13, PROPINFOBITS_NUMBITS);

            }
            prop.mBits = buf->ReadUBitLong(PROPINFOBITS_NUMBITS);
            if (angRotationProp)
            {
                assert(prop.mBits == 13);
                break;
            }
        }
    }
    return !buf->IsOverflowed();
}

//static bool WriteSendTable(bf_write* buf, const SendTable& table)
//{
//    buf->WriteString(table.mTableName);
//    const int numProps = table.mNumProps;
//    buf->WriteUBitLong(numProps, PROPINFOBITS_NUMPROPS);
//    for (int i = 0; i < numProps; ++i)
//    {
//        const SendProp& prop = table.mProps[i];
//        buf->WriteUBitLong(static_cast<unsigned int>(prop.mType), PROPINFOBITS_TYPE);
//        buf->WriteString(prop.mVarName);
//        buf->WriteUBitLong(prop.mFlags, PROPINFOBITS_FLAGS);
//
//        if (prop.mType == DPT_DataTable || ((prop.mFlags & SPROP_EXCLUDE) != 0))
//        {
//            buf->WriteString(prop.mExcludeDTName);
//        }
//        else if (prop.mType == DPT_Array)
//        {
//            buf->WriteUBitLong(prop.mNumElements, PROPINFOBITS_NUMELEMENTS);
//        }
//        else
//        {
//            buf->WriteBitFloat(prop.mLowValue);
//            buf->WriteBitFloat(prop.mHighValue);
//            buf->WriteUBitLong(prop.mBits, PROPINFOBITS_NUMBITS);
//        }
//    }
//    return !buf->IsOverflowed();
//}

RocketModeFixer::RocketModeFixer(FILE* outputFp) :
    DemoWriter(outputFp)
{
}

static bool DataTable_LoadDataTablesFromBuffer(bf_read* pBuf, int nDemoProtocol)
{
    while (pBuf->ReadOneBit() != 0)
    {
        const bool bNeedsDecoder = pBuf->ReadOneBit() != 0;

        SendTable table;
        if (PeekTableName(*pBuf, table.mTableName, sizeof(table.mTableName)) && !strcmp(table.mTableName, "DT_TFBaseRocket"))
        {
            return ModifyTFBaseRocketAngRotation(pBuf, table);
        }

        if (!ReadSendTable(pBuf, table))
        {
            return false;
        }
    }
    return true;
}

void RocketModeFixer::StartCommandPacket(const CommandPacket& packet)
{
    if (packet.cmd == dem_datatables)
    {
        DemMsg::Dem_DataTables* datatables = reinterpret_cast<DemMsg::Dem_DataTables*>(packet.data);
        bf_read buf(datatables->data.begin(), DemMsg::Dem_DataTables::DATA_MAX_LENGTH);
        const bool bNoOverflow = DataTable_LoadDataTablesFromBuffer(&buf, 0);
        assert(bNoOverflow);
    }
    DemoWriter::StartCommandPacket(packet);
}

IDemoWriter* IDemoWriter::CreateRocketModeFixerDemoWriter(void* outputFp)
{
    return new RocketModeFixer(reinterpret_cast<FILE*>(outputFp));
}
