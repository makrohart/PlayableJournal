#pragma once
#include "DllExport.h"

#include "PlayableEngine.h"

namespace pj
{
	namespace player
	{
		class PLAYABLEJOURNAL_API Player
		{
		public:
			Player() = default;
			~Player() = default;

			void play(const char* pJournalPath);

		private:
			pj::playable::PlayableEngine m_playableEngine;
		};
	}
}