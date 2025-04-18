#pragma once

#include "aspectable/Aspectable.h"
#include <format>
#include "reflectable/GMethodManager.h"
#include "Journal.h"
#include "reflectable/Reflectable.h"
#include <string>
#include "Utils.h"

namespace pj
{
	namespace playable
	{
		struct PlayableAspect
		{
			template<typename T, typename F, typename... Args>
			void preInvoke(F T::* pMMethod, Args&&... args)
			{
				std::string argsStr = "";
				if constexpr (sizeof...(Args) != 0)
					argsStr = pj::utils::toStringFromArgs(", ", std::forward<Args>(args)...);

				reflectable::Type* pType = reflectable::Type::get<T>();
				if (!pType)
					return;
				const std::string className = pType->getName();
				auto pObject = static_cast<T*>(this);
				const int id = pObject->getId();
				const std::string instanceName = pj::utils::toLower(className) + "_" + std::to_string(id);
				
				// If constructor is called
				if (const auto methodName = pType->getMethodName(pMMethod); methodName == "")
				{
					pj::journal::PLAYABLE(instanceName.c_str(), " = new ", className.c_str(), "()", ";");
				}
				else
				{
					pj::journal::PLAYABLE(instanceName.c_str(), ".", methodName.c_str(), "(", argsStr.c_str(), ");");
				}			
			}

			template<typename T, typename F, typename... Args>
			void postInvoke(F T::* /*pMMethod*/, Args&&... /*args*/)
			{
			}

			template<typename F, typename... Args>
			static void preInvoke(F&& method, Args&&... args)
			{
				std::string argsStr = "";
				if constexpr (sizeof...(Args) != 0)
					argsStr = pj::utils::toStringFromArgs(", ", std::forward<Args>(args)...);

				const std::string methodName = reflectable::GMethodManager::getInstance()->getMethodName(method);
				pj::journal::PLAYABLE(methodName.c_str(), "(", argsStr.c_str(), ");");
			}

			template<typename F, typename... Args>
			static void postInvoke(F&& /*method*/, Args&&... /*args*/)
			{
			}
		};

	}
}