#include "pch.h"
#include "Journal.h"

#include "Journalist.h"
#include "string"
#include "Utils.h"
#include "vector"
#include "chrono"

void pj::journal::INFO(const char* pStr)
{
	const auto time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::stringstream timeStr;
	tm time;
	localtime_s(&time, &time_t);
	timeStr << std::put_time(&time, "%Y-%m-%d %H:%M:%S");

	const std::string commentPrefix = "// " + timeStr.str() + " [INFO] ";

	std::vector<std::string> strs = pj::utils::splitString(pStr, '\n');
	if (strs.empty())
		return;

	for (auto it = strs.begin(); it != strs.cend(); it++)
		*it = commentPrefix + *it;

	auto first = strs.begin();
	*first = "\n" + *first;

	for (const auto& str : strs)
		Journalist::getInstance()->write(str.c_str());
}

PLAYABLEJOURNAL_API void pj::journal::FATAL(const char* pStr)
{
	std::vector<std::string> strs = pj::utils::splitString(pStr, '\n');
	if (strs.empty())
		return;

	for (auto it = strs.begin(); it != strs.cend(); it++)
		*it = "// " + *it;

	auto first = strs.begin();
	*first = "\n" + *first;

	for (const auto& str : strs)
		Journalist::getInstance()->write(str.c_str());

	throw std::exception(pStr);
}

PLAYABLEJOURNAL_API void pj::journal::PLAYABLE(const char* pStr)
{
	std::vector<std::string> strs = pj::utils::splitString(pStr, '\n');
	if (strs.empty())
		return;

	auto first = strs.begin();
	*first = "\n" + *first;

	for (const auto& str : strs)
		Journalist::getInstance()->write(str.c_str());
}