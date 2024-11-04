#pragma once

#include "..\base\MacroUtils.h"
#include "..\base\StringUtils.h"
#include "MethodInfo.h"
#include "ObjectInfo.h"

/// <summary>
/// Start to weave a class into an aspect by deriving a class as proxy from a target class.
/// </summary>
/// <param name="AspectClass:">Aspect name</param>
/// <param name="Class:">Target class</param>
#define ASPECT_VCLASS_BEGIN(AspectClass, Class)                                                                                     \
namespace aop                                                                                                                       \
{                                                                                                                                   \
	template<>                                                                                                                      \
    struct AspectProxy<Class, AspectClass> : Class, AspectClass                                                                     \
    {                                                                                                                               \
        using Type = Class;                                                                                                         \
		AspectProxy(Type target) : m_target(target), m_objectInfo(*(base::string::splitString(#Class, ':').cend() - 1), m_lastId++) \
		{                                                                                                                           \
		  preConstructor(m_objectInfo);                                                                                             \
		}

/// <summary>
/// Weave a virtual member method into an aspect in the derived class as proxy from a target class.
/// </summary>
/// <param name="ReturnType:">Return type of method</param>
/// <param name="Method:">Method name</param>
/// <param name="ArgType:">One argument type</param>
#define ASPECT_VMETHOD(ReturnType, Method, ArgType, ...)                                                                                \
        virtual ReturnType Method(										                                                                \
                                    ArgType _arg                                                                                        \
                                    FOR_EACH_WITH_STEP(COMMA_ARGTYPE_ARG, STEP_ARG, __arg, __VA_ARGS__)                                 \
                                 ) override                                                                                             \
        {                                                                                                                               \
			auto function = std::bind(&Type::Method, &m_target                                                                          \
                    FOR_EACH_WITH_STEP(COMMA_STD_PLACEHOLDER, STEP_1, 1, ArgType, __VA_ARGS__));                                        \
            return invoke<ReturnType>(&m_objectInfo, aop::MethodInfo(#Method), function,                                                \
			        _arg                                                                                                                \
			        FOR_EACH_WITH_STEP(COMMA_ARG, STEP_ARG, __arg, __VA_ARGS__));                                                       \
        }

/// <summary>
/// End weaving a class into an aspect by deriving a class as proxy from a target class.
/// </summary>
#define ASPECT_VCLASS_END               \
    private:                            \
        Type m_target;                  \
        ObjectInfo m_objectInfo;        \
		inline static int m_lastId = 0; \
    };                                  \
}

/// <summary>
/// Start to weave a class into an aspect by warpping a target class with a proxy class.
/// </summary>
/// <param name="AspectClass:">Aspect name</param>
/// <param name="Class:">Target class</param>
#define ASPECT_CLASS_BEGIN(AspectClass, Class)                                                                                      \
namespace aop                                                                                                                       \
{                                                                                                                                   \
	template<>                                                                                                                      \
    struct AspectProxy<Class, AspectClass> : AspectClass                                                                            \
    {                                                                                                                               \
        using Type = Class;                                                                                                         \
		AspectProxy(Type target) : m_target(target), m_objectInfo(*(base::string::splitString(#Class, ':').cend() - 1), m_lastId++) \
		{                                                                                                                           \
		  preConstructor(m_objectInfo);                                                                                             \
		}

/// <summary>
/// Weave a member method into an aspect by warpping a target class with a proxy class.
/// </summary>
/// <param name="ReturnType:">Return type of method</param>
/// <param name="Method:">Method name</param>
/// <param name="ArgType:">One argument type</param>
#define ASPECT_MMETHOD(ReturnType, Method, ArgType, ...)                                                                                \
        ReturnType Method(                                                                                                              \
                                    ArgType _arg                                                                                        \
                                    FOR_EACH_WITH_STEP(COMMA_ARGTYPE_ARG, STEP_ARG, __arg, __VA_ARGS__)                                 \
                                 )                                                                                                      \
        {                                                                                                                               \
			auto function = std::bind(&Type::Method, &m_target                                                                          \
                    FOR_EACH_WITH_STEP(COMMA_STD_PLACEHOLDER, STEP_1, 1, ArgType, __VA_ARGS__));                                        \
            return invoke<ReturnType>(&m_objectInfo, aop::MethodInfo(#Method), function,                                                \
			        _arg                                                                                                                \
			        FOR_EACH_WITH_STEP(COMMA_ARG, STEP_ARG, __arg, __VA_ARGS__));                                                       \
        }

/// <summary>
/// End weaving a class into an aspect by warpping a target class with a proxy class.
/// </summary>
#define ASPECT_CLASS_END                \
    private:                            \
        Type m_target;                  \
        ObjectInfo m_objectInfo;        \
		inline static int m_lastId = 0; \
    };                                  \
}

/// <summary>
/// Weave a global method into an aspect by warpping a target method with a proxy class.
/// </summary>
/// <param name="ReturnType:">Return type of method</param>
/// <param name="Method:">Method name</param>
/// <param name="ArgType:">One argument type</param>
#define ASPECT_METHOD(AspectClass, ReturnType, NameSpace, Method, ArgType, ...)                               \
namespace aop                                                                                                 \
{                                                                                                             \
	template<>                                                                                                \
    struct AspectProxy<ReturnType(ArgType,  __VA_ARGS__), AspectClass> : AspectClass                          \
    {                                                                                                         \
        static ReturnType Method(                                                                             \
							ArgType _arg                                                                      \
                            FOR_EACH_WITH_STEP(COMMA_ARGTYPE_ARG, STEP_ARG, __arg, __VA_ARGS__)               \
                         )                                                                                    \
        {                                                                                                     \
            return invoke<ReturnType>(nullptr, aop::MethodInfo(#Method), NameSpace::Method,                   \
												_arg                                                          \
												FOR_EACH_WITH_STEP(COMMA_ARG, STEP_ARG, __arg, __VA_ARGS__)); \
        }                                                                                                     \
	};                                                                                                        \
}

namespace aop
{
	/// <summary>
	/// To implement customized aspect, you need to inherit from class Aspect and declare and define preMethod and postMethod 
	/// </summary>
	/// <typeparam name="A">Aspect</typeparam>
	template<typename A>
	struct Aspect
	{
		virtual void preConstructor(const ObjectInfo& objectInfo) = 0;

		template<typename R, typename F, typename... Args>
		static R invoke(const aop::ObjectInfo& objectInfo, const MethodInfo& methodInfo, const F& func, Args&&... args)
		{
			static_assert(std::is_base_of<Aspect, A>::value, "Class Aspect should be inherited!");
			return static_cast<A*>(this)->invoke(func, std::forward<Args>(args)...);
		}
	};

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T">Target class to proxy</typeparam>
	/// <typeparam name="A">Aspect</typeparam>
	template<typename T, typename A>
	struct AspectProxy : T, A
	{
		AspectProxy(const T& target) : m_target(target) {}
	private:
		T m_target;
	};
}