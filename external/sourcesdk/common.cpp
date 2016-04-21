
#include "sourcesdk/common.h"
#include "snappy/snappy.h"
#include <string.h>

static const uint32 INVALID = static_cast<uint32>(-1);
static const uint32 LZSS_ID   = BigLong(('L' << 24) | ('Z' << 16) | ('S' << 8) | ('S'));
static const uint32 SNAPPY_ID = BigLong(('S' << 24) | ('N' << 16) | ('A' << 8) | ('P'));

inline uint32_t U8ToU32(const uint8* from)
{
    union U8ToU32
    {
        uint8 bytes[4];
        uint32 dword;
    };
    return reinterpret_cast<const U8ToU32*>(from)->dword;
}

uint32 COM_BufferToBufferDecompress(uint8* dest, uint32 destLen, const uint8* src, uint32 srcLen)
{
    const uint32 uncompressedLen = COM_GetUncompressedSize(src, srcLen);
    if (uncompressedLen == INVALID)
    {
        if (srcLen <= destLen)
        {
            memcpy(dest, src, srcLen);
        }
    }
    else if (uncompressedLen <= destLen)
    {
        const uint32 id = U8ToU32(src);
        if (id == LZSS_ID)
        {
            assert(false);
        }
        else if (id == SNAPPY_ID)
        {
            snappy::RawUncompress(reinterpret_cast<const char*>(src + 4), srcLen - 4, reinterpret_cast<char*>(dest));
        }
    }
    return (uncompressedLen != INVALID) ? uncompressedLen : 0;
}

uint32 COM_GetUncompressedSize(const uint8* data, uint32 numBytes)
{
    size_t uncompressedLength = INVALID;
    if (numBytes > 4)
    {
        const uint32 magic = U8ToU32(data);
        if (numBytes >= 8 && magic == LZSS_ID)
        {
            return U8ToU32(data + 4);
        }

        if (magic == SNAPPY_ID)
        {
            snappy::GetUncompressedLength(reinterpret_cast<const char*>(data + 4), numBytes - 4, &uncompressedLength);
        }
    }
    return static_cast<uint32>(uncompressedLength);
}

uint32 COM_GetIdealDestinationCompressionBufferSize_Snappy(uint32 srcLen)
{
    return snappy::MaxCompressedLength(srcLen) + sizeof(uint32);
}

/*uint32 COM_CompressBuffer_Snappy(const uint8* src, uint32 srcLen, uint8* dest, uint32 destLen)
{
    return COM_GetIdealDestinationCompressionBufferSize_Snappy(srcLen);
}*/
