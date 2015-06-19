// :mode=c++:
/*
decode.h - c++ wrapper for a base64 decoding algorithm

This is part of the libb64 project, and has been placed in the public domain.
For details, see http://sourceforge.net/projects/libb64
*/
#ifndef BASE64_DECODE_H
#define BASE64_DECODE_H

namespace base64
{
	extern "C"
	{
		#include "cdecode.h"
	}
} // namespace base64

#endif // BASE64_DECODE_H

