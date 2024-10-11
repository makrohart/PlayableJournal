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
	namespace journal
	{
		class JournalEngine final
		{
		public:
			inline JournalEngine()
			{
				initializeV8();

				// Currently, we only use one isolate.
				// We may consider use multiple for testing in isolation in multiple threads.
				m_createParams.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
				m_pIsolate = v8::Isolate::New(m_createParams);
			}

			inline ~JournalEngine()
			{
				m_pIsolate->Dispose();
				v8::V8::Dispose();
				v8::V8::DisposePlatform();
				delete m_createParams.array_buffer_allocator;
			}

			inline int compileAndExecute(const char* scriptPath)
			{
				v8::Isolate::Scope isolate_scope(m_pIsolate);
				v8::Global<v8::Context> context = bindJS2Native(m_pIsolate);
				bool success;
				{
					// Enter the execution environment before evaluating any code.
					v8::HandleScope handle_scope(m_pIsolate);
					v8::Context::Scope context_scope(context.Get(m_pIsolate));
					v8::Local<v8::String> file_name = v8::String::NewFromUtf8(m_pIsolate, scriptPath).ToLocalChecked();
					v8::Local<v8::String> source;
					if (!pj::utils::ReadFile(m_pIsolate, scriptPath).ToLocal(&source))
						pj::journal::FATAL(std::format("Error reading '%s'\n", scriptPath).c_str());
					success = compileAndExecute(source, file_name, false, true);
				}
				// It is important not to pump the message loop when there are v8::Local
				// handles on the stack, as this may trigger a stackless GC.
				while (v8::platform::PumpMessageLoop(m_upPlatform.get(), m_pIsolate)) continue;
				if (!success) return 1;
			}

		private:

			// Initialize V8
			inline void initializeV8()
			{
				// icu_data_file, natives_blob.bin and snapshot_blob.bin 
				// locate under the same directory with build directory.
				v8::V8::InitializeICUDefaultLocation("");
				v8::V8::InitializeExternalStartupData("");

				m_upPlatform = v8::platform::NewDefaultPlatform();

				v8::V8::InitializePlatform(m_upPlatform.get());
				if (!v8::V8::Initialize())
					pj::journal::FATAL("Failed to initialize v8");
			}

			inline v8::Global<v8::Context> bindJS2Native(v8::Isolate* pIsolate)
			{
				v8::HandleScope handle_scope(pIsolate);

				// Create a template for the global object.
				v8::Local<v8::ObjectTemplate> globalTemplate = v8::ObjectTemplate::New(pIsolate);

				pj::playable::PlayableManager* pPlayableManager = pj::playable::PlayableManager::getInstance();

				// Bind gloable method
				for (const auto& playableMethod : pPlayableManager->getPlayableMethods())
				{
					v8::Local<v8::FunctionTemplate> functionTemplate = v8::FunctionTemplate::New(pIsolate, playableMethod.getMethod());
					globalTemplate->Set(pIsolate, playableMethod.getName().c_str(), functionTemplate);
				}

				// Bind Class
				for (const auto& playableClass : pPlayableManager->getPlayableClasses())
				{
					const char* className = playableClass.getName().c_str();
					// Bind constructor
					v8::Local<v8::FunctionTemplate> classTemplate = v8::FunctionTemplate::New(pIsolate, playableClass.getConstructor());
					classTemplate->SetClassName(v8::String::NewFromUtf8(pIsolate, className).ToLocalChecked());
					globalTemplate->Set(pIsolate, className, classTemplate);
					v8::Local<v8::ObjectTemplate> classPrototype = classTemplate->PrototypeTemplate();

					// Bind methods
					for (auto& method : playableClass.getMethods())
						classPrototype->Set(pIsolate, method.getName().c_str(), v8::FunctionTemplate::New(pIsolate, method.getMethod()));

					v8::Local<v8::ObjectTemplate> classInstance = classTemplate->InstanceTemplate();
					classInstance->SetInternalFieldCount(1);

					// Bind getters and setters
					for (auto& accesser : playableClass.getAccessers())
						classInstance->SetAccessor(v8::String::NewFromUtf8(pIsolate, accesser.getName().c_str()).ToLocalChecked(), accesser.getGetter(), accesser.getSetter());
				}

				v8::Local<v8::Context> context = v8::Context::New(pIsolate, nullptr, globalTemplate);
				return v8::Global<v8::Context>(pIsolate, context);
			}

			// Executes a string within the current v8 context.
			inline bool compileAndExecute(v8::Local<v8::String> source, v8::Local<v8::Value> name, bool print_result, bool report_exceptions)
			{
				v8::HandleScope handle_scope(m_pIsolate);
				v8::TryCatch try_catch(m_pIsolate);
				v8::ScriptOrigin origin(m_pIsolate, name);
				v8::Local<v8::Context> context(m_pIsolate->GetCurrentContext());
				v8::Local<v8::Script> script;
				if (!v8::Script::Compile(context, source, &origin).ToLocal(&script))
				{
					// Print errors that happened during compilation.
					if (report_exceptions)
						reportException(&try_catch);
					return false;
				}
				else
				{
					v8::Local<v8::Value> result;
					if (!script->Run(context).ToLocal(&result))
					{
						assert(try_catch.HasCaught());
						// Print errors that happened during execution.
						if (report_exceptions)
							reportException(&try_catch);
						return false;
					}
					else
					{
						assert(!try_catch.HasCaught());
						if (print_result && !result->IsUndefined())
						{
							// If all went well and the result wasn't undefined then print
							// the returned value.
							v8::String::Utf8Value str(m_pIsolate, result);
							const char* cstr = *str;
							pj::journal::FATAL("execute result: ", cstr, "\n");
						}
						return true;
					}
				}
			}

			inline void reportException(v8::TryCatch* try_catch)
			{
				v8::HandleScope handle_scope(m_pIsolate);
				v8::String::Utf8Value exception(m_pIsolate, try_catch->Exception());
				const char* exception_string = *exception;
				v8::Local<v8::Message> message = try_catch->Message();
				if (message.IsEmpty())
				{
					// V8 didn't provide any extra information about this error; just
					// print the exception.
					pj::journal::FATAL(exception_string);
				}
				else
				{
					// Print (filename):(line number): (message).
					v8::String::Utf8Value filename(m_pIsolate, message->GetScriptOrigin().ResourceName());
					v8::Local<v8::Context> context(m_pIsolate->GetCurrentContext());
					const char* filename_string = *filename;
					const int linenum = message->GetLineNumber(context).FromJust();
					pj::journal::FATAL(std::format("{}:{}: {}", filename_string, linenum, exception_string).c_str());
					// Print line of source code.
					v8::String::Utf8Value sourceline(m_pIsolate, message->GetSourceLine(context).ToLocalChecked());
					const char* sourceline_string = *sourceline;
					pj::journal::FATAL(sourceline_string);
					// Print wavy underline (GetUnderline is deprecated).
					int start = message->GetStartColumn(context).FromJust();
					for (int i = 0; i < start; i++)
					{
						pj::journal::FATAL(" ");
					}
					int end = message->GetEndColumn(context).FromJust();
					for (int i = start; i < end; i++)
					{
						pj::journal::FATAL("^");
					}
					pj::journal::FATAL("\n");
					v8::Local<v8::Value> stack_trace_string;
					if (try_catch->StackTrace(context).ToLocal(&stack_trace_string) &&
						stack_trace_string->IsString() &&
						stack_trace_string.As<v8::String>()->Length() > 0)
					{
						v8::String::Utf8Value stack_trace(m_pIsolate, stack_trace_string);
						const char* err = *stack_trace;
						pj::journal::FATAL(err);
					}
				}
			}

		private:
			std::unique_ptr<v8::Platform> m_upPlatform;
			v8::Isolate::CreateParams m_createParams;
			v8::Isolate* m_pIsolate;
		};
	}
}