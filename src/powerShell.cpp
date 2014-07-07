// myaddon.cpp : Defines the exported functions for the DLL application.
//
#using "../dll/System.Management.Automation.dll"
#include "stdafx.h"
#pragma comment(lib, "node")
#include <node.h>
#include <v8.h>
#include <Windows.h>
#include <msclr\marshal_cppstd.h>
#include <string.h>



using namespace v8;
using namespace System;
using namespace System::Collections::ObjectModel;
using namespace System::Diagnostics;
using namespace System::Collections;
using namespace msclr::interop;
using namespace System::Management::Automation;


Handle<Value> parseJson(Handle<Value> jsonString) {
    HandleScope scope;

    Handle<Context> context = Context::GetCurrent();
    Handle<v8::Object> global = context->Global();

    Handle<v8::Object> JSON = global->Get(v8::String::New("JSON"))->ToObject();
    Handle<Function> JSON_parse = Handle<Function>::Cast(JSON->Get(v8::String::New("parse")));

    // return JSON.parse.apply(JSON, jsonString);
    return scope.Close(JSON_parse->Call(JSON, 1, &jsonString));
}

Handle<Value> getProcess(const Arguments& args) {
    HandleScope scope;
	Arguments args2 = args;
	Handle<Value> b = args2[0];
	Handle<v8::Object> val = v8::Object::New();
	std::string str;

	System::String^ _tmp = "";

	if(b->IsInt32())
		_tmp="-Id "+b->Int32Value();

	PowerShell^ _ps = PowerShell::Create();
	//get-wmiobject Win32_PerfFormattedData_PerfProc_Process | ? { $_.name -eq 'powershell' }
    _ps->AddScript("Get-Process "+_tmp+" | Select-Object name,processname,id,fileversion,productversion,company,path,description | ConvertTo-Json");
    auto results = _ps->Invoke();

	Handle<v8::Array> _array = v8::Array::New(results->Count);
    for (int i = 0; i < results->Count; i++)
    {
        System::String^ objectStr = results[i]->ToString();
       
		str = (marshal_as<std::string>(objectStr));
    }


    return scope.Close(parseJson(v8::String::New(str.c_str())));
}

// (gwmi -Query ("Select processid from Win32_Process  WHERE parentprocessid = 6188")) | Select-Object processid | ConvertTo - Json
Handle<Value> getChildProcess(const Arguments& args) {
	HandleScope scope;
	Handle<Value> b = args[0];
	Handle<v8::Object> val = v8::Object::New();
	std::string str;

	PowerShell^ _ps = PowerShell::Create();
	//get-wmiobject Win32_PerfFormattedData_PerfProc_Process | ? { $_.name -eq 'powershell' }
	
	_ps->AddScript("(gwmi -Query (\"Select processid from Win32_Process  WHERE parentprocessid = " + b->Int32Value() + "\")).processid");// | Select - Object processid | ConvertTo - Json");
	auto results = _ps->Invoke();
	Handle<v8::Array> _array = v8::Array::New(results->Count);
	for (int i = 0; i < results->Count; i++)
	{
		try{
			System::String^ objectStr = results[i]->ToString();
			str = (marshal_as<std::string>(objectStr));
			_array->Set(i, parseJson(v8::String::New(str.c_str())));
		}
		catch (System::Exception^ error){}
	}


	return scope.Close(_array);
}

Handle<Value> getProcessCPUUsage(const Arguments& args) {
	HandleScope scope;

	Handle<v8::Object> rObject = v8::Object::New();
	array<Process^>^ pArray = Process::GetProcesses();

	long long totalProcTime = 0;
	for (size_t i = 0; i < pArray->Length; i++)
	{
		Process^ proc = pArray[i];
		Handle<v8::Object> iObject = v8::Object::New();
		iObject->Set(v8::String::NewSymbol("ProcessName"), v8::String::New(marshal_as<std::string>( proc->ProcessName).c_str()));
		rObject->Set(v8::Number::New(proc->Id), iObject);
		try{
			long long _ticks = proc->TotalProcessorTime.Ticks;
			totalProcTime += _ticks;
			//	Console::WriteLine(100.0 / totalProcTime*proc->TotalProcessorTime.Ticks + " Process Usage " + proc->ProcessName);
			iObject->Set(v8::String::NewSymbol("CPUticks"), v8::Number::New(proc->TotalProcessorTime.Ticks));
		}
		catch (System::Exception^ error){
			//Console::WriteLine("Error with proc " + proc->ProcessName);
			iObject->Set(v8::String::NewSymbol("CPUticks"), Null());
			//Console::WriteLine(error);
		}
		rObject->Set(v8::String::NewSymbol("totalTicks"), v8::Number::New(totalProcTime));
	}

	return scope.Close(rObject);
}

void init (Handle<v8::Object> target) {
    HandleScope scope;
    target->Set(v8::String::NewSymbol("getProcess"), FunctionTemplate::New(getProcess)->GetFunction());
	target->Set(v8::String::NewSymbol("getChildProcess"), FunctionTemplate::New(getChildProcess)->GetFunction());
	target->Set(v8::String::NewSymbol("getCPUUsage"), FunctionTemplate::New(getProcessCPUUsage)->GetFunction());
}
NODE_MODULE(PowerShell, init);