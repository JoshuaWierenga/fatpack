/* Copyright (c) 2018, Sijmen J. Mulder. See LICENSE.md.
   Copyright (c) 2023, Joshua Wierenga. */

#pragma once

#define LEN(a) (sizeof(a)/sizeof(*(a)))

void warn(const _TCHAR *info);
void warnx(const _TCHAR *message);
__declspec(noreturn) void err(const _TCHAR *info);
__declspec(noreturn) void errx(const _TCHAR *message);

BOOL makeusedefault(HWND window);
#ifdef FatpackGUI
BOOL getlbstring(HWND listbox, int idx, _TCHAR *buf, size_t bufsz);
#endif
