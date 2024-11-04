#pragma once

#include "cstring"
#include "string"
#include "vector"

namespace base
{
	namespace string
	{
		inline std::vector<std::string> splitString(const char* str, const char* separator)
		{
			const size_t separatorLen = std::strlen(separator);
			size_t subStrLen = 0;

			std::vector<std::string> strs;
			const char* it = str;	
			while (true)
			{
				const char* pRestStr = it + subStrLen;
				if (size_t restStrLen = std::strlen(pRestStr); restStrLen < separatorLen)
				{
					if (subStrLen += restStrLen; subStrLen != 0)
					{
						strs.emplace_back(it, subStrLen);
					}
					break;
				}

				const int result = strncmp(pRestStr, separator, separatorLen);
				if (result != 0)
				{
					subStrLen++;
				}
				else
				{
					if (subStrLen != 0)
					{
						strs.emplace_back(it, subStrLen);
						// Reset
						it += subStrLen;
						subStrLen = 0;				
					}

					it += separatorLen;
					subStrLen = 0;
				}
			}

			return strs;
		}
	}
}