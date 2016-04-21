
#include "valve_support.h"

uint32 COM_BufferToBufferDecompress(uint8* dest, uint32 destLen, const uint8* src, uint32 srcLen);
uint32 COM_GetUncompressedSize(const uint8* data, uint32 numBytes);

uint32 COM_GetIdealDestinationCompressionBufferSize_Snappy(uint32 srcLen);
uint32 COM_CompressBuffer_Snappy(const uint8* src, uint32 srcLen, uint8* dest, uint32 destLen);
