#pragma once
#include "DllExport.h"

#include "cassert"
#include "format"
#include "functional"
#include "Journal.h"
#include "libplatform/libplatform.h"
#include "PlayableManager.h"
#include "Utils.h"
#include "v8.h"

namespace pj
{
	namespace playable
	{
		class PlayableEngine final
		{
		public:
			PlayableEngine();

			~PlayableEngine();

			int compileAndExecute(const char* scriptPath);

		private:

			// Initialize V8
			void initializeV8();

			v8::Global<v8::Context> bindJS2Native(v8::Isolate* pIsolate);

			// Executes a string within the current v8 context.
			bool compileAndExecute(v8::Local<v8::String> source, v8::Local<v8::Value> name, bool print_result, bool report_exceptions);

			void reportException(v8::TryCatch* try_catch);

		private:
			std::unique_ptr<v8::Platform> m_upPlatform;
			v8::Isolate::CreateParams m_createParams;
			v8::Isolate* m_pIsolate;
		};
	}
}