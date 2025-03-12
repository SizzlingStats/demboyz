
#pragma once

#include <cstdint>
#include <cstddef>
#include <cassert>
#include <string.h>
#include <stdlib.h>
#include "vector.h"

using uint64 = std::uint64_t;
using uint32 = std::uint32_t;
using uint16 = std::uint16_t;
using uint8 = std::uint8_t;
using int64 = std::int64_t;
using int32 = std::int32_t;
using int16 = std::int16_t;
using int8 = std::int8_t;
using uint = unsigned int;
using byte = char;

#define PLATFORM_64BITS

#ifdef _WIN32
    #ifdef PLATFORM_64BITS
        typedef __int64 intp;				// intp is an integer that can accomodate a pointer
        typedef unsigned __int64 uintp;		// (ie, sizeof(intp) >= sizeof(int) && sizeof(intp) >= sizeof(void *)
    #else
        typedef __int32 intp;
        typedef unsigned __int32 uintp;
    #endif
#else
    #ifdef PLATFORM_64BITS
        typedef long long			intp;
        typedef unsigned long long	uintp;
    #else
        typedef int					intp;
        typedef unsigned int		uintp;
    #endif
#endif

#define IsPC() true

#define Assert(x) assert(x)
#define AssertMsg(x, ...) assert(x)
#define AssertMsg2(x, ...) assert(x)
#define AssertFatalMsg(x, ...) assert(x)

#define Q_memcpy memcpy

#define VALVE_LITTLE_ENDIAN 1
#define RESTRICT
#define FORCEINLINE inline

// OVERALL Coordinate Size Limits used in COMMON.C MSG_*BitCoord() Routines (and someday the HUD)
#define	COORD_INTEGER_BITS			14
#define COORD_FRACTIONAL_BITS		5
#define COORD_DENOMINATOR			(1<<(COORD_FRACTIONAL_BITS))
#define COORD_RESOLUTION			(1.0/(COORD_DENOMINATOR))

// Special threshold for networking multiplayer origins
#define COORD_INTEGER_BITS_MP		11
#define COORD_FRACTIONAL_BITS_MP_LOWPRECISION 3
#define COORD_DENOMINATOR_LOWPRECISION			(1<<(COORD_FRACTIONAL_BITS_MP_LOWPRECISION))
#define COORD_RESOLUTION_LOWPRECISION			(1.0/(COORD_DENOMINATOR_LOWPRECISION))

#define NORMAL_FRACTIONAL_BITS		11
#define NORMAL_DENOMINATOR			( (1<<(NORMAL_FRACTIONAL_BITS)) - 1 )
#define NORMAL_RESOLUTION			(1.0/(NORMAL_DENOMINATOR))

///////////

#if defined _MSC_VER		// MSVC (What about MinGW and Clang for Windows)

#define DWordSwap(d) ((uint32)(_byteswap_ulong( (unsigned long) d)))

#elif defined __GNUC__		// GCC or Clang

#define DWordSwap(d) __builtin_bswap32(d)

#else						// N/A, native code

#pragma message( "TODO: Using non-intrinsic byteswap functions..." )

template <typename T>
inline T DWordSwapC(T dw)
{
    uint32 temp;

    static_assert(sizeof(T) == sizeof(uint32));

    temp = *((uint32*)&dw) >> 24;
    temp |= ((*((uint32*)&dw) & 0x00FF0000) >> 8);
    temp |= ((*((uint32*)&dw) & 0x0000FF00) << 8);
    temp |= ((*((uint32*)&dw) & 0x000000FF) << 24);

    return *((T*)&temp);
}

#define DWordSwap DWordSwapC

#endif

// If a swapped float passes through the fpu, the bytes may get changed.
// Prevent this by swapping floats as DWORDs.
#define SafeSwapFloat( pOut, pIn )	(*((uint*)pOut) = DWordSwap( *((uint*)pIn) ))

inline long BigLong(long val) { int test = 1; return (*(char*)&test == 1) ? DWordSwap(val) : val; }
inline uint32 LittleDWord(uint32 val) { int test = 1; return (*(char*)&test == 1) ? val : DWordSwap(val); }
inline void LittleFloat(float* pOut, const float* pIn) { int test = 1; (*(char*)&test == 1) ? (*pOut = *pIn) : SafeSwapFloat(pOut, pIn); }

FORCEINLINE uint32 LoadLittleDWord(uint32* base, unsigned int dwordIndex)
{
    return LittleDWord(base[dwordIndex]);
}

FORCEINLINE void StoreLittleDWord(uint32* base, unsigned int dwordIndex, uint32 dword)
{
    base[dwordIndex] = LittleDWord(dword);
}

#define BITS_PER_INT 32

inline int GetBitForBitnum( int bitNum ) 
{ 
    static int bitsForBitnum[] = 
    {
        ( 1 << 0 ),
        ( 1 << 1 ),
        ( 1 << 2 ),
        ( 1 << 3 ),
        ( 1 << 4 ),
        ( 1 << 5 ),
        ( 1 << 6 ),
        ( 1 << 7 ),
        ( 1 << 8 ),
        ( 1 << 9 ),
        ( 1 << 10 ),
        ( 1 << 11 ),
        ( 1 << 12 ),
        ( 1 << 13 ),
        ( 1 << 14 ),
        ( 1 << 15 ),
        ( 1 << 16 ),
        ( 1 << 17 ),
        ( 1 << 18 ),
        ( 1 << 19 ),
        ( 1 << 20 ),
        ( 1 << 21 ),
        ( 1 << 22 ),
        ( 1 << 23 ),
        ( 1 << 24 ),
        ( 1 << 25 ),
        ( 1 << 26 ),
        ( 1 << 27 ),
        ( 1 << 28 ),
        ( 1 << 29 ),
        ( 1 << 30 ),
        ( 1 << 31 ),
    };
    return bitsForBitnum[ (bitNum) & (BITS_PER_INT-1) ]; 
}

#ifndef _WIN32
#define _strnicmp strncasecmp
#endif

inline int V_strnicmp( const char *str1, const char *str2, int n )
{
    const unsigned char *s1 = (const unsigned char*)str1;
    const unsigned char *s2 = (const unsigned char*)str2;
    for ( ; n > 0 && *s1; --n, ++s1, ++s2 )
    {
        if ( *s1 != *s2 )
        {
            // in ascii char set, lowercase = uppercase | 0x20
            unsigned char c1 = *s1 | 0x20;
            unsigned char c2 = *s2 | 0x20;
            if ( c1 != c2 || (unsigned char)(c1 - 'a') > ('z' - 'a') )
            {
                // if non-ascii mismatch, fall back to CRT for locale
                if ( (c1 | c2) >= 0x80 ) return _strnicmp( (const char*)s1, (const char*)s2, n );
                // ascii mismatch. only use the | 0x20 value if alphabetic.
                if ((unsigned char)(c1 - 'a') > ('z' - 'a')) c1 = *s1;
                if ((unsigned char)(c2 - 'a') > ('z' - 'a')) c2 = *s2;
                return c1 > c2 ? 1 : -1;
            }
        }
    }
    return (n > 0 && *s2) ? -1 : 0;
}
