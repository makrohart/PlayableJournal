#include "pch.h"
#include "JournalistManager.h"

#include "FileJournalist.h"
#include "mutex"

namespace
{
	std::mutex s_journalistManagerMutex;
	pj::journal::JournalistManager* s_pJournalistManager;
}

pj::journal::JournalistManager* pj::journal::JournalistManager::getInstance()
{
	if (!s_pJournalistManager)
	{
		s_journalistManagerMutex.lock();
		if (!s_pJournalistManager)
		{
			s_pJournalistManager = new JournalistManager();
		}
		s_journalistManagerMutex.unlock();
	}
	return s_pJournalistManager;
}

pj::journal::JournalistManager::JournalistManager()
{
	m_journalists.push_back(std::make_unique<FileJournalist>());
}

void pj::journal::JournalistManager::write(const char* pData)
{
	for (auto& journalist : m_journalists)
		journalist->write(pData);
}