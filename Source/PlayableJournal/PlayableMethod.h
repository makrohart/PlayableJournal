#pragma once

#include "ValueUtils.h"
#include "reflectable/Type.h"

namespace playable
{
    struct PlayableType;

    struct PlayableMethod
    {
        virtual ~PlayableMethod() {};
    };

    template<typename T, typename F>
    struct PlayableMMethod : PlayableMethod
    {
        template<typename T, typename F, typename... Args>
        using InvokePtrType = std::invoke_result_t<F T::*, T*, Args...>(T::*)(F T::*, Args&&...);

        PlayableMMethod(PlayableType* pType, const std::string& name, F T::* /*pMethod*/) : m_pType(pType), m_name(name) {}

        template<F T::* pMethod, typename... Args>
        PlayableType& playable() {
            v8::FunctionCallback callback = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
                v8::Isolate* pIsolate = info.GetIsolate();
                v8::HandleScope handleScope(pIsolate);
                v8::Local<v8::Object> self = info.Holder();
                v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));
                T* pNative = static_cast<T*>(native->Value());

                reflectable::Type* pType = reflectable::Type::get<T>();
                if (!pType)
                    return;

                const std::string methodName = pType->getMethodName(pMethod);

                using R = std::invoke_result_t<F T::*, T*, Args...>;
                using MethodType = R(Args...);
                InvokePtrType<T, MethodType, Args...> invokePtr = &T::template invoke<T, MethodType, Args...>;

                if constexpr (std::is_void_v<R>)
                {
                    std::invoke(invokePtr, pNative, pMethod,
                        pj::utils::toNativeFromJS<Args>(pIsolate, info[std::index_sequence_for<Args...>()])...);
                }
                else
                {
                    R result = std::invoke(invokePtr, pNative, pMethod,
                        pj::utils::toNativeFromJS<Args>(pIsolate, info[std::index_sequence_for<Args...>()])...);
                    info.GetReturnValue().Set(pj::utils::toJSFromNative(pIsolate, result));
                }
                };

            m_pType->addMethod(m_name, callback);

            return *m_pType;
        }

    private:
        const std::string m_name;
        playable::PlayableType* m_pType;
    };
}