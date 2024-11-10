#pragma once

#include "fstream"
#include "IJournalist.h"
#include "string"

namespace pj
{
	namespace journal
	{
		class FileJournalist final : public IJournalist
		{
		public:

			FileJournalist();	
			~FileJournalist();
			FileJournalist(FileJournalist&& other);
			FileJournalist& operator=(FileJournalist&& other);
			// Disable copy constructor and assignment as it should only be stored in singleton JournalistManager.
			// No copy behaviour should occur.
			FileJournalist(const FileJournalist&) = delete;
			FileJournalist& operator=(const FileJournalist&) = delete;

			virtual void write(const char* pData) override;

		private:

			void initialize(const char* pJournalPath, const size_t buffSize = 1024);

			void openOrCreate();
			void write(const char* pData, const size_t writeSize);
			void close();
			void flush();

		private:
			std::string m_journalPath;
			std::fstream m_journal;

			char* m_pBuff;
			char* m_pBuffIndex;
			size_t m_buffSize;
		};
	}
}