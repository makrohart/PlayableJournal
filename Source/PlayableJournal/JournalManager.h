#pragma once
#include "DllExport.h"

#include "fstream"
#include "JournalEngine.h"
#include "string"

namespace pj
{
	namespace journal
	{
		class JournalManager
		{
		public:
			static JournalManager* getInstance();

			void initialize(const std::string& journalPath, const size_t buffSize = 1024);

			void write(const char* pData);

			void executeJournal(const char* journal);

		public:
			JournalEngine* getJournalEngine() { return &m_journalEngine; }

		private:
			JournalManager();
			~JournalManager();

			JournalManager(const JournalManager&) = delete;
			JournalManager(JournalManager&&) = delete;
			JournalManager& operator=(const JournalManager&) = delete;
			JournalManager& operator=(JournalManager&&) = delete;

		private:
			void openOrCreate();
			void close();
			void write(const char* pData, const size_t writeSize);
			void flush();

		private:
			std::string m_journalPath;
			std::fstream m_journal;

			char* m_pBuff;
			char* m_pBuffIndex;
			size_t m_buffSize;

			JournalEngine m_journalEngine;
		};
	}
}