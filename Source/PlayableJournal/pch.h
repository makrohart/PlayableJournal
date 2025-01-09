// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#ifdef _DEBUG
#pragma comment(lib, "Debug\\v8.dll.lib")
#pragma comment(lib, "Debug\\v8_libbase.dll.lib")
#pragma comment(lib, "Debug\\v8_libplatform.dll.lib")
#else
#pragma comment(lib, "Release\\v8.dll.lib")
#pragma comment(lib, "Release\\v8_libbase.dll.lib")
#pragma comment(lib, "Release\\v8_libplatform.dll.lib")
#endif

// This is for static link of v8 library
//#pragma comment(lib, "DbgHelp.lib")
//#pragma comment(lib, "winmm.lib")

#endif //PCH_H
