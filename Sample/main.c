/* Copyright (c) 2018, Sijmen J. Mulder. See LICENSE.md.
   Copyright (c) 2023, Joshua Wierenga. */

#define USE_TERM

#define _WIN32_WINNT _WIN32_WINNT_WINXP
#include <windows.h>
#include <tchar.h>

int CALLBACK
WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR cmdline, int cmdshow)
{
#ifdef USE_TERM
	const _TCHAR *output = _T("Hello from ") _T(PLATFORM) _T("!\r\n");

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
