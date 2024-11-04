#pragma once

#include "string"
#include "vector"

namespace base
{
	namespace string
	{
		inline std::vector<std::string> splitString(const char* str, const char separator)
		{
			std::vector<std::string> strs;
			int subStrLen = 1;
			const char* subStr = str;
			while (true)
			{
				if (*str == separator)
				{
					strs.emplace_back(subStr, subStrLen);
					subStr = ++str;
					subStrLen = 1;
					continue;
				}

				if (*str == '\0')
				{
					strs.emplace_back(subStr, subStrLen);
					break;
				}

				str++;
				subStrLen++;
			}

			return strs;
		}
	}
}