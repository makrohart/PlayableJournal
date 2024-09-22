#include "pch.h"
#include "Player.h"
#include "Journalist.h"

void pj::player::Player::play(const char* pJournalPath)
{
	pj::journal::Journalist::getInstance()->getJournalEngine()->compileAndExecute(pJournalPath);
}