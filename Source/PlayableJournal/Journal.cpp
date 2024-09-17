#include "pch.h"
#include "Journal.h"

#include "JournalManager.h"
#include "string"
#include "Utils.h"
#include "vector"

void pj::journal::INFO(const char* pStr)
{
	std::vector<std::string> strs = pj::utils::splitString(pStr, '\n');
	if (strs.empty())
		return;

	for (auto it = strs.begin(); it != strs.cend(); it++)
		*it = "// " + *it;

	auto first = strs.begin();
	*first = "\n" + *first;

	for (const auto& str : strs)
		JournalManager::getInstance()->write(str.c_str());
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
		JournalManager::getInstance()->write(str.c_str());

	throw std::exception(pStr);
}
