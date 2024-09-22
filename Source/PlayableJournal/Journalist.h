#pragma once
#include "DllExport.h"

#include "fstream"
#include "JournalEngine.h"
#include "string"

namespace pj
{
	namespace journal
	{
		PLAYABLEJOURNAL_API class Journalist
		{
		public:
			PLAYABLEJOURNAL_API static Journalist* getInstance();

			void initialize(const std::string& journalPath, const size_t buffSize = 1024);

			void write(const char* pData);

			pj::journal::JournalEngine* getJournalEngine() { return &m_journalEngine; }

		private:
			Journalist();
			~Journalist();

			Journalist(const Journalist&) = delete;
			Journalist(Journalist&&) = delete;
			Journalist& operator=(const Journalist&) = delete;
			Journalist& operator=(Journalist&&) = delete;
			
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

			pj::journal::JournalEngine m_journalEngine;
		};
	}
}