/* Copyright (c) 2021, Sijmen J. Mulder. See LICENSE.md.
   Copyright (c) 2023, Joshua Wierenga. */

#include <tchar.h>
#include <Windows.h>
#include "pack.h"
#include "util.h"
#if defined(FatpackGUI)
#include "../Fatpack/resource.h"
#elif defined(FatpackTUI)
#include "../FatpackTUI/resource.h"
#endif

_TCHAR *FLAG_FATBINARY = NULL;
BOOL FLAG_GUI = TRUE;

BOOL
packinit(const _TCHAR *path) {
	HINSTANCE instance;
	HANDLE resinfo;
	HGLOBAL reshandle;
	void *resdata;
	size_t resdatasz;
	HANDLE file;
	PIMAGE_DOS_HEADER mzheader;
	WORD newsubsystem;

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

	if (FLAG_GUI) {
		mzheader = (PIMAGE_DOS_HEADER)resdata;
		newsubsystem = IMAGE_SUBSYSTEM_WINDOWS_GUI;
		SetFilePointer(file, mzheader->e_lfanew + (LONG)offsetof(IMAGE_NT_HEADERS32, 
			OptionalHeader.Subsystem), 0, FILE_BEGIN);
		WriteFile(file, &newsubsystem, sizeof newsubsystem, NULL, NULL);
	}

	CloseHandle(file);
	return TRUE;
}

BOOL
packadd(HANDLE resupdate, int i, const _TCHAR *path) {
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

void
pack(HWND dialog, int programc, _TCHAR **programv) {
#if defined(FatpackGUI)
	HINSTANCE instance;
	HWND listbox;
	OPENFILENAME ofn;
	_TCHAR path[4096];
	_TCHAR srcpath[4096];
#elif defined(FatpackTUI)
	_TCHAR *path;
	_TCHAR *srcpath;
#endif
	LRESULT count;
	_TCHAR tmpdir[MAX_PATH + 1];
	_TCHAR tmppath[MAX_PATH + 1];
	HANDLE resupdate;
	int i;

	tmppath[0] = '\0';
#if defined(FatpackGUI)
	instance = GetModuleHandle(NULL);

	if (!(listbox = GetDlgItem(dialog, IDC_EXELIST))) {
		warn(_T("Failed to get list box handle"));
		return;
	}

	count = SendMessage(listbox, LB_GETCOUNT, 0, 0);
	if (count == LB_ERR) {
		warnx(_T("Failed to get list box item count."));
		return;
	}
#elif defined(FatpackTUI)
	count = programc;
#endif

	if (!count) {
		warnx(_T("Add one or more executables to pack into a ")
			_T("'fat' universal binary."));
		return;
	}

#if defined(FatpackGUI)
	path[0] = _T('\0');

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = dialog;
	ofn.hInstance = instance;
	ofn.lpstrFilter =
		_T("Executable (*.exe)\0*.exe\0")
		_T("All Files (*.*)\0*.*\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = path;
	ofn.nMaxFile = LEN(path);
	ofn.lpstrDefExt = _T("exe");
	ofn.Flags = OFN_OVERWRITEPROMPT;

	if (!GetSaveFileName(&ofn))
		return;
#elif defined (FatpackTUI)
	if (FLAG_FATBINARY == NULL) {
		warnx(_T("Path to output 'fat' universal binary required, ")
			_T("specify with -o"));
		return;
	}

	path = FLAG_FATBINARY;
#endif
	if (!GetTempPath(LEN(tmpdir), tmpdir)) {
		warn(_T("Failed to get temporary directory path"));
		return;
	}

	if (!GetTempFileName(tmpdir, _T("Fpk"), 0, tmppath)) {
		warn(_T("Failed to get temporary file path"));
		return;
	}

	if (!packinit(tmppath))
		return;

	if (!(resupdate = BeginUpdateResource(tmppath, FALSE))) {
		warn(_T("Failed to open the temporary file for resource ")
			_T("editing"));
		goto cleanup;
	}

	for (i = 0; i < count; i++) {
#if defined(FatpackGUI)
		if (!getlbstring(listbox, i, srcpath, LEN(srcpath)))
			goto cleanup;
#elif defined(FatpackTUI)
		// TODO: Rewrite relative paths as absolute paths with \\.\ when possible
		if (programv[i][0] == _T('\0'))
			goto cleanup;
		srcpath = programv[i];
#endif
		if (!packadd(resupdate, i, srcpath))
			goto cleanup;
	}

	EndUpdateResource(resupdate, FALSE);
	resupdate = NULL;

	if (!CopyFile(tmppath, path, FALSE))
		warn(_T("Failed to write output file"));

cleanup:
	if (resupdate)
		EndUpdateResource(resupdate, TRUE);
	if (tmppath[0])
		DeleteFile(tmppath);
}