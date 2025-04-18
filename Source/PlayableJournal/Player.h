#pragma once
#include "DllExport.h"

#include "PlayableEngine.h"

namespace playable
{
	class PLAYABLEJOURNAL_API Player
	{
	public:
		Player() = default;
		~Player() = default;

		void play(const char* pJournalPath);

	private:
		PlayableEngine m_playableEngine;
	};
}