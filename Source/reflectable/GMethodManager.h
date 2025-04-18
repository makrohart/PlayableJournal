#pragma once

#include "Method.h"
#include <vector>
#include <string>
namespace reflectable
{
	struct GMethodManager
	{
		inline static GMethodManager* getInstance()
		{
			static GMethodManager* s_instance = nullptr;
			if (!s_instance)
				s_instance = new GMethodManager();

			return s_instance;
		}

		inline void add(const Method* pMethod)
		{
			m_methods.push_back(pMethod);
		}

		template<typename F>
		inline static std::string getMethodName(F&& method)
		{
			for (const Method* pMethod : m_methods)
			{
				const GMethod<F>* pGMethod = reinterpret_cast<const GMethod<F>*>(pMethod);
				if (pGMethod && pGMethod->getMethod() == method)
					return pGMethod->getName();
			}
			return "";
		}
	
	private: 
		GMethodManager() = default;
		~GMethodManager() = default; 
		GMethodManager(const GMethodManager&) = delete; 
		GMethodManager(GMethodManager&&) = delete; 
		GMethodManager& operator=(const GMethodManager&) = delete;
		GMethodManager& operator=(GMethodManager&&) = delete;

		std::vector<const Method*> m_methods;
	};
}