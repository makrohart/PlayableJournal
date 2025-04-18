#pragma once

#include "PlayableMethod.h"
#include <string>
#include <unordered_map>
#include "ValueUtils.h"
#include "v8.h"

namespace playable
{
    struct PlayableType
    {
        using Accessor = std::pair<v8::AccessorGetterCallback, v8::AccessorSetterCallback>;

        PlayableType(const std::string& name) : m_name(name) {}

        template<typename T, typename... Args>
        PlayableType& constructor()
        {
            if (m_methods.find(m_name) != m_methods.cend())
                return *this;

            m_methods[m_name] = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
                v8::Isolate* pIsolate = info.GetIsolate();

                T* pNative = new T(pj::utils::toNativeFromJS<Args>(pIsolate, info[std::index_sequence_for<Args...>()])...);
                info.This()->SetInternalField(0, v8::External::New(pIsolate, pNative));
                };

            return *this;
        }

        template<typename T, typename F>
        PlayableMMethod<T, F>& method(const std::string& name, F T::* pMethod)
        {
            PlayableMMethod<T, F>* result = new PlayableMMethod(this, name, pMethod);
            m_toDeletes.push_back(result);
            return *result;
        }

        PLAYABLEJOURNAL_API void addMethod(const std::string& name, v8::FunctionCallback callback);

        inline const std::unordered_map<std::string, v8::FunctionCallback>& getMethods() const
        {
            return m_methods;
        }

        inline const std::unordered_map<std::string, Accessor>& getAccessors() const
        {
            return m_accessors;
        }

        inline std::string getName() const
        {
            return m_name;
        }

        PLAYABLEJOURNAL_API void end();

    private:
        const std::string m_name;
        std::unordered_map<std::string, v8::FunctionCallback> m_methods;
        std::unordered_map<std::string, Accessor> m_accessors;

        std::vector<PlayableMethod*> m_toDeletes;
    };
}