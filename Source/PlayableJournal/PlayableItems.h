#pragma once
#include "DllExport.h"

#include "PlayableClass.h"
#include "Singleton.h"

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

		class PlayableManager : pj::base::Singleton<PlayableManager>
		{
		public:
			void add(const PlayableClass& playableClass)
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