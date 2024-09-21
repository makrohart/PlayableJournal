#pragma once 
#include "DllExport.h"

#include "string"
namespace pj
{
	namespace player
	{
		PLAYABLEJOURNAL_API class PlayerOption
		{
		public:
			PLAYABLEJOURNAL_API void setJournalToPlay(const char* pJournal) { m_journalToPlay = pJournal; };
			PLAYABLEJOURNAL_API void setJournalToRecord(const char* pJournal) { m_journalToRecord = pJournal; };

			const std::string& getJournalToPlay() const { return m_journalToPlay; };
			const std::string& getJournalToRecord() const { return m_journalToRecord; };

		private:
			std::string m_journalToPlay;
			std::string m_journalToRecord;
		};
	}
}