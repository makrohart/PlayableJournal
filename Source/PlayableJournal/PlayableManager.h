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

			PLAYABLEJOURNAL_API void add(const PlayableMethod& method)
			{
				m_methods.push_back(method);
			}

			std::vector<PlayableClass>& getPlayableClasses()
			{
				return m_playableClasses;
			}

			std::vector<PlayableMethod>& getPlayableMethods()
			{
				return m_methods;
			}

		private:
			std::vector<PlayableClass> m_playableClasses;
			std::vector<PlayableMethod> m_methods;
		};
	}
}