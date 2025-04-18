#pragma once

#include "aspectable/Aspectable.h"
#include <string>

struct TestAspect_1
{
	template<typename T, typename F, typename... Args>
	void preInvoke(F T::* /*pMMethod*/, Args&&... /*args*/)
	{
	}

	template<typename T, typename F, typename... Args>
	void postInvoke(F T::* /*pMMethod*/, Args&&... /*args*/)
	{
	}

	template<typename F, typename... Args>
	static void preInvoke(F&& /*method*/, Args&&... /*args*/)
	{
	}

	template<typename F, typename... Args>
	static void postInvoke(F&& /*method*/, Args&&... /*args*/)
	{
	}
};

struct TestAspect_2
{
	template<typename T, typename F, typename... Args>
	void preInvoke(F T::* /*pMMethod*/, Args&&... /*args*/)
	{
	}

	template<typename T, typename F, typename... Args>
	void postInvoke(F T::* /*pMMethod*/, Args&&... /*args*/)
	{
	}

	template<typename F, typename... Args>
	static void preInvoke(F&& /*method*/, Args&&... /*args*/)
	{
	}

	template<typename F, typename... Args>
	static void postInvoke(F&& /*method*/, Args&&... /*args*/)
	{
	}
};

class TestAspectableClass : public aspectable::Aspectable<TestAspect_1, TestAspect_2>
{
public:

	void voidMethodInt(const int intValue) {}

	std::string stringMethodIntString(const int intValue, const std::string& stringValue)
	{
		std::string retStr;
		for (int ii = intValue; ii >= 0; ii--)
		{
			retStr += stringValue;
		}
		return retStr;
	}

	double doubleMethodInt(const int intValue)
	{
		return intValue;
	}
};

namespace testAspectableMethods
{
	std::string stringMethodIntString(const int intValue, const std::string& stringValue)
	{
		std::string retStr;
		for (int ii = intValue; ii >= 0; ii--)
		{
			retStr += stringValue;
		}
		return retStr;
	}
}

