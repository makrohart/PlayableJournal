#pragma once
#include "DllExport.h"

#include "JournalEngine.h"
#include "PlayerOption.h"
namespace pj
{
	namespace player
	{
		PLAYABLEJOURNAL_API class Player
		{
		public:
			PLAYABLEJOURNAL_API Player() {};
			PLAYABLEJOURNAL_API void play(const char* pJournalPath);
		};
	}
}