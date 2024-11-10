#include "pch.h"
#include "Journal.h"

#include "chrono"
#include "JournalistManager.h"
#include "string"
#include "StringUtils.h"
#include "Utils.h"
#include "vector"

namespace
{
	enum class LogLevel
	{
		INFO = 0,
		WARN,
		FATAL,
		PLAYABLE
	};

	std::string getTimeStamp()
	{
		const auto time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::stringstream timeStr;
		tm time;
		localtime_s(&time, &time_t);
		timeStr << std::put_time(&time, "%Y-%m-%d %H:%M:%S");
		return timeStr.str();
	}

	void writeToJournal(const LogLevel logLevel, const char* pStr)
	{
		std::string commentPrefix = "";
		switch (logLevel)
		{
		case (LogLevel::INFO):
			commentPrefix += "// " + getTimeStamp() + " [INFO] ";
			break;
		case(LogLevel::WARN):
			commentPrefix += "// " + getTimeStamp() + " [WARN] ";
			break;
		case(LogLevel::FATAL):
			commentPrefix += "// " + getTimeStamp() + " [FATAL] ";
			break;
		case(LogLevel::PLAYABLE):
		default:
			break;
		}

		std::vector<std::string> strs = base::string::splitString(pStr, "\n");
		if (strs.empty())
			return;

		for (auto it = strs.begin(); it != strs.cend(); it++)
			*it = commentPrefix + *it;

		auto first = strs.begin();
		*first = "\n" + *first;

		for (const auto& str : strs)
			pj::journal::JournalistManager::getInstance()->write(str.c_str());
	}
}

void pj::journal::INFO(const char* pStr)
{
	writeToJournal(LogLevel::INFO, pStr);
}

void pj::journal::WARN(const char* pStr)
{
	writeToJournal(LogLevel::WARN, pStr);
}

void pj::journal::FATAL(const char* pStr)
{
	writeToJournal(LogLevel::FATAL, pStr);

	throw std::exception(pStr);
}

void pj::journal::PLAYABLE(const char* pStr)
{
	writeToJournal(LogLevel::PLAYABLE, pStr);
}