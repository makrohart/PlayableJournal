#pragma once
#include "DllExport.h"

#include "string"
#include "v8.h"
namespace pj
{
	namespace playable
	{
		class PlayableMethod
		{
		public:
			PlayableMethod(const std::string& name, v8::FunctionCallback method) : m_name(name), m_method(method) {}

			const std::string getName() const { return m_name; }
			const v8::FunctionCallback& getMethod() const { return m_method; }

		private:
			std::string m_name;
			v8::FunctionCallback m_method;
		};
	}
}