#pragma once
#include <string>
namespace reflectable
{
	struct Method
	{
		virtual std::string getName() const = 0;

		uintptr_t end()
		{
			return (uintptr_t)this;
		}
	};

	template<typename T, typename F>
	struct MMethod : Method
	{
		MMethod(const std::string& name, F T::* pMember) : m_name(name), m_pMMethod(pMember) {}

		std::string getName() const override { return m_name; }

		inline F T::* getMMethod() const { return m_pMMethod; }
	private:
		const std::string m_name;
		F T::* m_pMMethod;
	};

	template<typename F>
	struct GMethod : Method
	{
		GMethod(const std::string& name, F&& method) : m_name(name), m_method(method) {}

		std::string getName() const override { return m_name; }

		inline F getMethod() const { return m_method; }
	private:
		const std::string m_name;
		F m_method;
	};
}