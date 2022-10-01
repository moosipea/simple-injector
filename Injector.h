#pragma once

#include <iostream>
#include <windows.h>
#include <thread>
#include <libloaderapi.h>
#include <TlHelp32.h>

// Whether to display errors in the console or as a MessageBox
//#define MESSAGEBOX_ERRORS

namespace Injector
{
	DWORD GetProcessIdFromExecutable(const char* executableName);
	DWORD GetProcessIdFromTitle(const char* windowTitle);
	bool Inject(DWORD pid, const char* dll);

	void l_Error(const char* error);
	bool l_FileExists(const char* path);
}
