#include "pch.h"
#include "Journal.h"

#include "chrono"
#include "Journalist.h"
#include "string"
#include "StringUtils.h"
#include "Utils.h"
#include "vector"

namespace
{
	std::string getTimeStamp()
	{
		const auto time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::stringstream timeStr;
		tm time;
		localtime_s(&time, &time_t);
		timeStr << std::put_time(&time, "%Y-%m-%d %H:%M:%S");
		return timeStr.str();
	}
}

void pj::journal::INFO(const char* pStr)
{
	const std::string commentPrefix = "// " + getTimeStamp() + " [INFO] ";

	std::vector<std::string> strs = base::string::splitString(pStr, '\n');
	if (strs.empty())
		return;

	for (auto it = strs.begin(); it != strs.cend(); it++)
		*it = commentPrefix + *it;

	auto first = strs.begin();
	*first = "\n" + *first;

	for (const auto& str : strs)
		Journalist::getInstance()->write(str.c_str());
}

void pj::journal::FATAL(const char* pStr)
{
	const std::string commentPrefix = "// " + getTimeStamp() + " [FATAL] ";

	std::vector<std::string> strs = base::string::splitString(pStr, '\n');
	if (strs.empty())
		return;

	for (auto it = strs.begin(); it != strs.cend(); it++)
		*it = commentPrefix + *it;

	auto first = strs.begin();
	*first = "\n" + *first;

	for (const auto& str : strs)
		Journalist::getInstance()->write(str.c_str());

	throw std::exception(pStr);
}

void pj::journal::PLAYABLE(const char* pStr)
{
	std::vector<std::string> strs = base::string::splitString(pStr, '\n');
	if (strs.empty())
		return;

	auto first = strs.begin();
	*first = "\n" + *first;

	for (const auto& str : strs)
		Journalist::getInstance()->write(str.c_str());
}