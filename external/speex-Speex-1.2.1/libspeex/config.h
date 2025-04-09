
#define inline __inline
#define EXPORT

#define DISABLE_VBR
#define DISABLE_ENCODER
#define DISABLE_FLOAT_API

//#define FLOATING_POINT
#define FIXED_POINT

#ifndef FIXED_POINT

//#define USE_SIMD

// In Visual Studio, _M_IX86_FP=1 means /arch:SSE was used, likewise
// _M_IX86_FP=2 means /arch:SSE2 was used.
// Also, enable both _USE_SSE and _USE_SSE2 if we're compiling for x86-64
#if _M_IX86_FP >= 1 || defined(_M_X64)
#define _USE_SSE
#endif

#if _M_IX86_FP >= 2 || defined(_M_X64)
#define _USE_SSE2
#endif

// Visual Studio support alloca(), but it always align variables to 16-bit
// boundary, while SSE need 128-bit alignment. So we disable alloca() when
// SSE is enabled.
#ifndef _USE_SSE
#  define USE_ALLOCA
#endif
#endif

#ifdef FIXED_POINT
#define USE_KISS_FFT
#else
#define USE_SMALLFT
//#define VORBIS_PSYCHO
#endif
