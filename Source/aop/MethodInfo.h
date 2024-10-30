#pragma once

#include "string"

namespace aop
{
	class MethodInfo
	{
	public:
		MethodInfo(std::string methodName) : m_methodName(methodName) {}

		std::string getMethodName() const { return m_methodName; }

	private:
		std::string m_methodName;
	};
}