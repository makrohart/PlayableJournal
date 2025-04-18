#pragma once

#include "Field.h"
#include <memory>
#include "Method.h"
#include <string>
#include "Constructor.h"
#include <unordered_map>
#include <vector>

namespace reflectable
{
    template<typename C, typename F>
    int computeOffset(F C::* pMember)
    {
        return reinterpret_cast<const char*>(&((C*)nullptr->*pMember)) - (char*)nullptr;
    }

	struct Type
	{
        Type(const std::string& name) : m_name(name), m_pParent(nullptr)
        {}

        Type& inherit(const Type* pParent)
        {
            m_pParent = pParent;
            return *this;
        }

        template<typename... Args>
        Type& constructor()
        {
            m_constructors.push_back(std::make_unique<CConstructor<Args...>>());
            return *this;
        }

        template<typename T, typename F>
        Type& method(const std::string& name, F T::* pMMethod)
        {
            if (m_mmethods.find(name) == m_mmethods.cend())
                m_mmethods.insert(std::make_pair(name, std::make_unique<MMethod<T, F>>(name, pMMethod)));

            return *this;
        }

        template<typename T, typename F>
        Type& field(const std::string& name, F T::* pMField)
        {
            m_mfields.push_back(Field{ name, computeOffset(pMField)});
            return *this;
        }

        uintptr_t end()
        {
            return (uintptr_t)this;
        }

        template<typename T>
        static Type* get()
        {
            return (Type*)T::s_reflectable;
        }

        inline std::string getName() const { return m_name; }

        inline Method* getMethod(const std::string& name) const
        {
            auto pair = m_mmethods.find(name);
            if (pair == m_mmethods.cend())
                return nullptr;

            return pair->second.get();
        }

        template<typename T, typename F>
        inline std::string getMethodName(F T::* pMMethod)
        {
            for (const auto& [methodName, pMethod] : m_mmethods)
            {
                MMethod<T, F>* pMFunc = reinterpret_cast<MMethod<T, F>*>(pMethod.get());
                if (pMFunc && (pMFunc->getMMethod() == pMMethod))
                {
                    return methodName;
                }
            }

            return "";
        }
    private:
		const std::string m_name;
        std::vector<std::unique_ptr<Constructor>> m_constructors;
		std::vector<Field> m_mfields;
        std::unordered_map<std::string, std::unique_ptr<Method>> m_mmethods;

		const Type* m_pParent;
	};
}