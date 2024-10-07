#pragma once

#include "string"
#include "vector"
#include "v8.h"

namespace pj
{
    namespace utils
    {
        template<typename T>
        v8::Local<v8::Value> toJSFromNative(v8::Isolate* pIsolate, const T& value);

        template<>
        v8::Local<v8::Value> toJSFromNative(v8::Isolate* pIsolate, const std::string& value)
        {
            return v8::String::NewFromUtf8(pIsolate, value.c_str()).ToLocalChecked();
        }

        template<typename T>
        v8::Local<v8::Value> toJSFromNative(v8::Isolate* pIsolate, const std::vector<T>& values)
        {
            v8::Local<v8::Context> context = pIsolate->GetCurrentContext();
            v8::Local<v8::Array> v8Values = v8::Array::New(pIsolate, values.size());
            for (size_t ii = 0; ii != values.size(); ++ii)
            {
                v8::Local<v8::Value> elem = toJSFromNative(pIsolate, values[ii]);   
                v8Values->Set(context, ii, elem).Check();
            }
            return v8Values;
        }

        template<typename T>
        T toNativeFromJS(v8::Isolate* pIsolate, v8::Local<v8::Value> value);

        template<>
        int toNativeFromJS<int>(v8::Isolate* pIsolate, v8::Local<v8::Value> value)
        {
            return value.As<v8::Int32>()->Value();
        }

        template<>
        const char* toNativeFromJS<const char*>(v8::Isolate* pIsolate, v8::Local<v8::Value> value)
        {
            v8::Local<v8::String> v8Str = value.As<v8::String>();
            const int size = v8Str->Utf8Length(pIsolate);
            char* pStr = new char[size + 1] {0};
            v8Str->WriteUtf8(pIsolate, pStr, size);
            return pStr;
        }

        template<>
        const char toNativeFromJS<const char>(v8::Isolate* pIsolate, v8::Local<v8::Value> value)
        {
            v8::Local<v8::String> v8Str = value.As<v8::String>();
            char* pStr = new char[2] {0};
            v8Str->WriteUtf8(pIsolate, pStr, 1);
            return *pStr;
        }

        template<>
        std::string toNativeFromJS<std::string>(v8::Isolate* pIsolate, v8::Local<v8::Value> value)
        {
            v8::Local<v8::String> v8Str = value.As<v8::String>();
            const int size = v8Str->Utf8Length(pIsolate);
            char* pStr = new char[size + 1] {0};
            v8Str->WriteUtf8(pIsolate, pStr, size);
            std::string str = std::string(pStr);
            delete pStr;
            return str;
        }
    }
}