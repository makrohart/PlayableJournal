#include "pch.h"
#include "Player.h"
#include "JournalManager.h"

void pj::player::Player::play(const char* pJournalPath)
{
	pj::journal::JournalManager::getInstance()->getJournalEngine()->compileAndExecute(pJournalPath);
}