#pragma once
#include "DllExport.h"

#include "PlayableClass.h"

namespace pj
{
	namespace playable
	{
		PLAYABLEJOURNAL_API class PlayableItems
		{
		public:
			PLAYABLEJOURNAL_API static void add(const PlayableClass& playableClass)
			{
				getPlayableClasses().push_back(playableClass);
			}

			static std::vector<PlayableClass>& getPlayableClasses()
			{
				static std::vector<PlayableClass> playableClasses;
				return playableClasses;
			}
		};
	}
}