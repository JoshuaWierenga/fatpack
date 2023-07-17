/* Copyright (c) 2021, Sijmen J. Mulder. See LICENSE.md. */

extern _TCHAR* FLAG_FATBINARY;
extern BOOL FLAG_GUI;

BOOL packinit(const _TCHAR* path);
BOOL packadd(HANDLE resupdate, int i, const TCHAR* path);
void pack(HWND dialog, int count, _TCHAR** programs);
