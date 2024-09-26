#pragma once
#include "DllExport.h"

#include "PlayableClass.h"
#include "Singleton.h"

namespace pj
{
	namespace playable
	{
		class PlayableManager final
		{
			DECLARE_SINGLETON_DEFAULT(PlayableManager)

		public:
			PLAYABLEJOURNAL_API void add(const PlayableClass& playableClass)
			{
				m_playableClasses.push_back(playableClass);
			}

			std::vector<PlayableClass>& getPlayableClasses()
			{
				return m_playableClasses;
			}

		private:
			std::vector<PlayableClass> m_playableClasses;
		};
	}
}