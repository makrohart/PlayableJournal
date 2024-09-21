#include "pch.h"
#include "JournalManager.h"

#include "mutex"

namespace
{
	std::mutex s_journalManagerMutex;
	pj::journal::JournalManager* s_pJournalManager;
}

pj::journal::JournalManager* pj::journal::JournalManager::getInstance()
{
	if (!s_pJournalManager)
	{
		s_journalManagerMutex.lock();
		if (!s_pJournalManager)
		{
			s_pJournalManager = new JournalManager();
		}
		s_journalManagerMutex.unlock();
	}
	return s_pJournalManager;
}

void pj::journal::JournalManager::initialize(const std::string& journalPath, const size_t buffSize)
{
	m_journalPath = journalPath;
	m_buffSize = buffSize;
	// 0 as place holder
	m_pBuff = new char[buffSize];
	memset(m_pBuff, 0, m_buffSize);
	m_pBuffIndex = m_pBuff;

	openOrCreate();
}

pj::journal::JournalManager::JournalManager()
{
	initialize("Journal.js");
}

pj::journal::JournalManager::~JournalManager()
{
	if (m_pBuff)
	{
		close();
		delete m_pBuff;
		m_pBuffIndex = nullptr;
	}
}

void pj::journal::JournalManager::openOrCreate()
{
	if (m_journal && m_journal.is_open())
		return;
	else
		m_journal.open(m_journalPath, std::fstream::out | std::fstream::app);

	if (!m_journal)
		m_journal.open(m_journalPath, std::fstream::out | std::fstream::app);
}

void pj::journal::JournalManager::close()
{
	if (m_journal)
	{
		m_journal.flush();
		m_journal.close();
	}

}

void pj::journal::JournalManager::write(const char* pData)
{
	if (!pData)
		throw std::invalid_argument("Invalid argument with type const char*");

	write(pData, strlen(pData));
	flush();
}

void pj::journal::JournalManager::write(const char* pData, const size_t writeSize)
{
	const size_t writableSize = m_buffSize - (m_pBuffIndex - m_pBuff);

	if (writeSize < writableSize)
	{
		memcpy(m_pBuffIndex, pData, writeSize);
		m_pBuffIndex += writeSize;
	}
	else
	{
		memcpy(m_pBuffIndex, pData, writableSize);
		m_pBuffIndex += writableSize;
		flush();
		write(pData + writableSize, writeSize - writableSize);
	}
}

void pj::journal::JournalManager::flush()
{
	m_journal << m_pBuff;
	m_journal.flush();
	memset(m_pBuff, 0, m_buffSize);
	m_pBuffIndex = m_pBuff;
}