#pragma once
#include "DllExport.h"

#include "JournalEngine.h"

namespace pj
{
	namespace player
	{
		class Player
		{
		public:
			inline Player() = default;

			inline void play(const char* pJournalPath)
			{
				m_journalEngine.compileAndExecute(pJournalPath);
			}

		private:
			pj::journal::JournalEngine m_journalEngine;
		};
	}
}