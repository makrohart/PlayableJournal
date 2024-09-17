#pragma once
#include "DllExport.h"

#include "format"
#include "functional"
#include "libplatform/libplatform.h"
#include "v8.h"

namespace pj
{
	namespace journal
	{
		class JournalEngine final
		{
		public:
			JournalEngine();
			~JournalEngine();

			int compileAndExecute(const char* scriptPath);

		private:

			// Initialize V8
			void initializeV8();

			void bindJS2Native(v8::Isolate* pIsolate);

			// Executes a string within the current v8 context.
			bool compileAndExecute(v8::Local<v8::String> source, v8::Local<v8::Value> name, bool print_result, bool report_exceptions);

			void reportException(v8::TryCatch* try_catch);

		private:
			std::unique_ptr<v8::Platform> m_upPlatform;
			v8::Isolate::CreateParams m_createParams;
			v8::Isolate* m_pIsolate;
			v8::Global<v8::Context> m_context;
		};

		class JS2Native
		{
			using initFunc = void(*)(v8::Local<v8::ObjectTemplate>);
		public:
			static constexpr int add(initFunc method)
			{
				getMethods().push_back(method);
				return 0;
			}
			static void initAll(v8::Local<v8::ObjectTemplate> global)
			{
				for (auto& method : getMethods())
				{
					method(global);
				}
			}
		private:
			static std::vector<initFunc>& getMethods()
			{
				static std::vector<initFunc> methods;
				return methods;
			}
		};
	}
}