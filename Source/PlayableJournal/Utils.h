#pragma once
#include "DllExport.h"

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

		inline std::string formatString(const char* pStr) { return pStr; }

		template<typename... Args>
		void formatString(const char* pStrFirst, const char* pStrSecond, Args... args)
		{
			std::string str = std::string(pStrFirst) + std::string(pStrSecond);
			return formatString(str.c_str(), args...);
		}

		template<typename T>
		std::string toString(T value)
		{
			return std::to_string(value);
		}

		inline std::string toString(const char* value)
		{
			return std::string(value);
		}

		inline std::string toString(const char value)
		{
			char str[2]{ 0 };
			str[0] = value;
			return std::string(str);
		}

		/// <summary>
		/// E.g. "std::string str" => ""std::string str""
		/// TODO: add this to regression
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		inline std::string toString(const std::string& value)
		{
			return '"' + value + '"';
		}

		template<typename T, typename U>
		std::string toStringFromArgs(const char* pSeparator, T argFirst, U argSecond)
		{
			std::string strArgFirst = pj::utils::toString(argFirst);
			std::string strArgSecond = pj::utils::toString(argSecond);
			std::string str = std::string(strArgFirst) + std::string(pSeparator) + std::string(strArgSecond);
			return str;
		}

		template<typename T, typename U, typename... Args>
		std::string toStringFromArgs(const char* pSeparator, T argFirst, U argSecond, Args&&... args)
		{
			std::string strArgFirst = toString(argFirst);
			std::string strArgSecond = toString(argSecond);
			std::string str = std::string(strArgFirst) + std::string(pSeparator) + std::string(strArgSecond);
			return toStringFromArgs(pSeparator, str.c_str(), args...);
		}

		inline std::string toLower(std::string str)
		{
			for (char& c : str)
			{
				// cast to unsigned char to avoid negative value
				c = std::tolower(static_cast<unsigned char>(c));
			}
			return str;
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