#include "pch.h"
#include "Player.h"

void playable::Player::play(const char* pJournalPath)
{
	m_playableEngine.compileAndExecute(pJournalPath);
}