// :mode=c++:
/*
encode.h - c++ wrapper for a base64 encoding algorithm

This is part of the libb64 project, and has been placed in the public domain.
For details, see http://sourceforge.net/projects/libb64
*/
#ifndef BASE64_ENCODE_H
#define BASE64_ENCODE_H

namespace base64
{
	extern "C" 
	{
		#include "cencode.h"
	}
} // namespace base64

#endif // BASE64_ENCODE_H

