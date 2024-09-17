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
#pragma comment(lib, "debug\\compression_utils_portable.lib")
#pragma comment(lib, "debug\\icui18n.lib")
#pragma comment(lib, "debug\\icuuc.lib")
#pragma comment(lib, "debug\\mksnapshot.lib")
#pragma comment(lib, "debug\\v8_libbase.lib")
#pragma comment(lib, "debug\\v8_libplatform.lib")
#pragma comment(lib, "debug\\v8_monolith.lib")
#pragma comment(lib, "debug\\zlib.lib")
#else
#pragma comment(lib, "release\\compression_utils_portable.lib")
#pragma comment(lib, "release\\icui18n.lib")
#pragma comment(lib, "release\\icuuc.lib")
#pragma comment(lib, "release\\mksnapshot.lib")
#pragma comment(lib, "release\\v8_libbase.lib")
#pragma comment(lib, "release\\v8_libplatform.lib")
#pragma comment(lib, "release\\v8_monolith.lib")
#pragma comment(lib, "release\\zlib.lib")
#endif

#pragma comment(lib, "DbgHelp.lib")
#pragma comment(lib, "winmm.lib")

#endif //PCH_H
