#pragma once

#include "memory"
#include "IJournalist.h"
#include "vector"

namespace pj
{
	namespace journal
	{
		class JournalistManager final
		{
		public:
			static JournalistManager* getInstance();

			void write(const char* pData);

		private:
			JournalistManager();
			~JournalistManager() {};

			JournalistManager(const JournalistManager&) = delete;
			JournalistManager(JournalistManager&&) = delete;
			JournalistManager& operator=(const JournalistManager&) = delete;
			JournalistManager& operator=(JournalistManager&&) = delete;

		private:
			std::vector<std::unique_ptr<IJournalist>> m_journalists;
		};
	}
}