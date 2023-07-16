/* Copyright (c) 2021, Sijmen J. Mulder. See LICENSE.md. */

#include <tchar.h>
#include <Windows.h>
#include "pack.h"
#include "util.h"
#if defined(FatpackGUI)
#include "../Fatpack/resource.h"
#elif defined(FatpackTUI)
#include "../FatpackTUI/resource.h"
#endif

BOOL
packinit(const _TCHAR* path) {
	HINSTANCE instance;
	HANDLE resinfo;
	HGLOBAL reshandle;
	void* resdata;
	size_t resdatasz;
	HANDLE file;

	instance = GetModuleHandle(NULL);

	resinfo = FindResource(instance, MAKEINTRESOURCE(IDR_LOADER),
		RT_RCDATA);
	if (!resinfo ||
		!(reshandle = LoadResource(instance, resinfo)) ||
		!(resdata = LockResource(reshandle)) ||
		!(resdatasz = SizeofResource(instance, resinfo))) {
		warn(_T("Failed to load embbed launcher resource"));
		return FALSE;
	}

	file = CreateFile(path, GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_TEMPORARY, NULL);
	if (!file) {
		warn(_T("Failed to open temporary file for writing"));
		return FALSE;
	}

	if (!WriteFile(file, resdata, (DWORD)resdatasz, NULL, NULL)) {
		warn(_T("Failed to write data to temporary file"));
		CloseHandle(file);
		return FALSE;
	}

	CloseHandle(file);
	return TRUE;
}

BOOL
packadd(HANDLE resupdate, int i, const TCHAR* path) {
	HANDLE file;
	HANDLE mapping;
	HANDLE view;
	DWORD sz;
	BOOL ok;
	
	file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!file) {
		warn(_T("Failed to open input file for reading"));
		return FALSE;
	}

	sz = GetFileSize(file, NULL);
	if (sz == INVALID_FILE_SIZE) {
		warn(_T("Failed to get input file size"));
		CloseHandle(file);
		return FALSE;
	}

	mapping = CreateFileMapping(file, NULL, PAGE_READONLY, 0, (DWORD)sz,
		NULL);
	if (!mapping) {
		warn(_T("Failed to create memory mapping for input file"));
		CloseHandle(file);
		return FALSE;
	}

	view = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, (DWORD)sz);
	if (!view) {
		warn(_T("Failed to create mapping view of input file"));
		CloseHandle(mapping);
		CloseHandle(file);
		return FALSE;
	}

	ok = UpdateResource(resupdate, RT_RCDATA, MAKEINTRESOURCE(1000 + i),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), view, (DWORD)sz);
	if (!ok) {
		warn(_T("Failed to update resource in output file"));
		UnmapViewOfFile(view);
		CloseHandle(mapping);
		CloseHandle(file);
		return FALSE;
	}

	UnmapViewOfFile(view);
	CloseHandle(mapping);
	CloseHandle(file);
	return TRUE;
}
