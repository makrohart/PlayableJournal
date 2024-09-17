#pragma once

#include "string"
#include "vector"
#include "v8.h"

namespace pj
{
	namespace utils
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

		// Reads a file into a v8 string.
		inline v8::MaybeLocal<v8::String> ReadFile(v8::Isolate* isolate, const char* name)
		{
#pragma warning(suppress : 4996)
			FILE* file = fopen(name, "rb");
			if (file == nullptr) return {};

			fseek(file, 0, SEEK_END);
			size_t size = ftell(file);
			rewind(file);

			char* chars = new char[size + 1];
			chars[size] = '\0';
			for (size_t i = 0; i < size;) {
				i += fread(&chars[i], 1, size - i, file);
				if (ferror(file)) {
					fclose(file);
					return {};
				}
			}
			fclose(file);
			v8::MaybeLocal<v8::String> result = v8::String::NewFromUtf8(isolate, chars, v8::NewStringType::kNormal, static_cast<int>(size));
			delete[] chars;
			return result;
		}
	}
}