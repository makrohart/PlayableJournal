#pragma once

namespace aspectable
{
    /// <summary>
    /// Asepctable is a light aspect oriented pattern (aop) framework based on strategic pattern.
    /// Derive the class that is to be weaved to aspect.
    /// </summary>
    /// <typeparam name="...As">Aspects</typeparam>
    template<typename... As>
    struct Aspectable : As...
    {
        template<typename T, typename F, typename... Args>
        auto invoke(F T::* pMMethod, Args&&... args)
        {
            using R = std::invoke_result_t<F T::*, T*, Args...>;

            (..., As::preInvoke(pMMethod, std::forward<Args>(args)...));

            if constexpr (std::is_void_v<R>) {
                (static_cast<T*>(this)->*pMMethod)(std::forward<Args>(args)...);
                (..., As::postInvoke(pMMethod, std::forward<Args>(args)...));
            }
            else {
                R result = (static_cast<T*>(this)->*pMMethod)(std::forward<Args>(args)...);
                (..., As::postInvoke(pMMethod, std::forward<Args>(args)...));
                return result;
            }
        }

        template<typename F, typename... Args>
        static auto invoke(F&& method, Args&&... args)
        {
            using R = std::invoke_result_t<F, Args...>;

            (..., As::preInvoke(method, std::forward<Args>(args)...));

            if constexpr (std::is_void_v<R>) {
                method(std::forward<Args>(args)...);
                (..., As::postInvoke(method, std::forward<Args>(args)...));
            }
            else {
                R result = method(std::forward<Args>(args)...);
                (..., As::postInvoke(method, std::forward<Args>(args)...));
                return result;
            }
        }
    };

    /// <summary>
    /// Example Aspect that need to have two template member methods of "preInvoke" and "postInvoke"
    /// </summary>
    struct Aspect
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
}