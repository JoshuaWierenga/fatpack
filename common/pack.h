/* Copyright (c) 2021, Sijmen J. Mulder. See LICENSE.md.
   Copyright (c) 2023, Joshua Wierenga. */

#pragma once

extern _TCHAR *FLAG_FATBINARY;
extern BOOL FLAG_GUI;

BOOL packinit(const _TCHAR *path);
BOOL packadd(HANDLE resupdate, int i, const _TCHAR *path);
void pack(HWND dialog, int count, _TCHAR **programs);
