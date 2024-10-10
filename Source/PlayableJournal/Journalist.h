#pragma once
#include "DllExport.h"

#include "fstream"
#include "JournalEngine.h"
#include "string"

namespace pj
{
	namespace journal
	{
		class Journalist final
		{
		public:
			static Journalist* getInstance();

			void write(const char* pData);

		private:
			Journalist();
			~Journalist();

			Journalist(const Journalist&) = delete;
			Journalist(Journalist&&) = delete;
			Journalist& operator=(const Journalist&) = delete;
			Journalist& operator=(Journalist&&) = delete;
			
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