#include "pch.h"
#include "Player.h"

void pj::player::Player::play(const char* pJournalPath)
{
	m_playableEngine.compileAndExecute(pJournalPath);
}