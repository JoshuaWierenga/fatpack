/* Copyright (c) 2018, Sijmen J. Mulder. See LICENSE.md. */

#define USE_TERM

#include <windows.h>
#include <tchar.h>

int CALLBACK
WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR cmdline, int cmdshow)
{
#ifdef USE_TERM
	const TCHAR* output = _T("Hello from " PLATFORM "!\r\n");

	if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
		AllocConsole();
	}

	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), output,

		(DWORD)_tcslen(output), NULL, NULL);
#else
	MessageBox(NULL, _T("Hello from " PLATFORM "!"), _T("Sample"),
	    MB_OK | MB_ICONINFORMATION);
#endif
	return 0;
}
