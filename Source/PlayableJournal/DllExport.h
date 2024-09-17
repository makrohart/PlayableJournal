#pragma once

#ifdef PLAYABLEJOURNAL_EXPORTS
#define PLAYABLEJOURNAL_API __declspec(dllexport)
#else
#define PLAYABLEJOURNAL_API __declspec(dllimport)
#endif