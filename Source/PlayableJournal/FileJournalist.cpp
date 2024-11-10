#include "pch.h"
#include "FileJournalist.h"
#include "filesystem"

namespace
{
	const char journalFilePrefix[] = "Journal";
	const char journalExtention[] = ".js";
}

pj::journal::FileJournalist::FileJournalist()
{
	std::string journalPath = std::string(journalFilePrefix) + std::string(journalExtention);
	int postfix = 1;
	while (std::filesystem::exists(journalPath.c_str()))
	{
		std::ostringstream oss;
		oss << journalFilePrefix << "." << std::setw(4) << std::setfill('0') << postfix++ << journalExtention;
		journalPath = oss.str();
	}

	initialize(journalPath.c_str());
}

pj::journal::FileJournalist::~FileJournalist()
{
	if (m_pBuff)
	{
		close();
		delete m_pBuff;
		m_pBuffIndex = nullptr;
	}
}

pj::journal::FileJournalist::FileJournalist(FileJournalist&& other)
{
	m_journalPath = other.m_journalPath;
	m_journal = std::move(other.m_journal);
	
	m_pBuff = other.m_pBuff;
	other.m_pBuff = nullptr;
	m_pBuffIndex = other.m_pBuffIndex;
	other.m_pBuffIndex = nullptr;
	m_buffSize = other.m_buffSize;
}

pj::journal::FileJournalist& pj::journal::FileJournalist::operator=(FileJournalist&& other)
{
	m_journalPath = other.m_journalPath;
	m_journal = std::move(other.m_journal);

	m_pBuff = other.m_pBuff;
	other.m_pBuff = nullptr;
	m_pBuffIndex = other.m_pBuffIndex;
	other.m_pBuffIndex = nullptr;
	m_buffSize = other.m_buffSize;
	return *this;
}

void pj::journal::FileJournalist::initialize(const char* pJournalPath, const size_t buffSize)
{
	m_journalPath = pJournalPath;
	m_buffSize = buffSize;
	// 0 as place holder
	m_pBuff = new char[buffSize];
	memset(m_pBuff, 0, m_buffSize);
	m_pBuffIndex = m_pBuff;

	openOrCreate();
}

void pj::journal::FileJournalist::openOrCreate()
{
	if (m_journal && m_journal.is_open())
		return;
	else
		m_journal.open(m_journalPath, std::fstream::out | std::fstream::app);

	if (!m_journal)
		m_journal.open(m_journalPath, std::fstream::out | std::fstream::app);
}

void pj::journal::FileJournalist::write(const char* pData)
{
	if (!pData)
		throw std::invalid_argument("Invalid argument with type const char*");

	write(pData, strlen(pData));
	flush();
}

void pj::journal::FileJournalist::write(const char* pData, const size_t writeSize)
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

void pj::journal::FileJournalist::close()
{
	if (m_journal)
	{
		m_journal.flush();
		m_journal.close();
	}
}

void pj::journal::FileJournalist::flush()
{
	m_journal << m_pBuff;
	m_journal.flush();
	memset(m_pBuff, 0, m_buffSize);
	m_pBuffIndex = m_pBuff;
}