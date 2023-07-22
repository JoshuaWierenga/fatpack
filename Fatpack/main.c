/* Copyright (c) 2018, Sijmen J. Mulder. See LICENSE.md.
   Copyright (c) 2023, Joshua Wierenga. */

#define _WIN32_WINNT _WIN32_WINNT_WINXP
#include <windows.h>
#include <tchar.h>
#include "../common/pack.h"
#include "../common/util.h"
#include "resource.h"

BOOL
getlbstring(HWND listbox, int idx, _TCHAR *buf, size_t bufsz)
{
	LRESULT len;
	LRESULT res;

	len = SendMessage(listbox, LB_GETTEXTLEN, (WPARAM)idx, 0);
	if (len == LB_ERR) {
		warnx(_T("Failed to retrieve list box item text ")
		    _T("length."));
		return FALSE;
	}

	if ((size_t)(len + 1) >= bufsz) {
		warnx(_T("The list box item does not fit in the ")
		    _T("allocated buffer."));
		return FALSE;
	}

	res = SendMessage(listbox, LB_GETTEXT, (WPARAM)idx, (LPARAM)buf);
	if (res == LB_ERR) {
		warnx(_T("Failed to get list box entry text."));
		return FALSE;
	}

	return TRUE;
}

static void
addfile(HWND dialog)
{
	_TCHAR path[4096];
	_TCHAR *name;
	OPENFILENAME ofn;
	size_t pathlen, namelen;
	HWND listbox;

	if (!((listbox = GetDlgItem(dialog, IDC_EXELIST)))) {
		warn(_T("Failed to get listbox handle"));
		return;
	}

	path[0] = _T('\0');

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = dialog;
	ofn.lpstrFilter =
	    _T("Executable (*.exe)\0*.exe\0")
	    _T("All Files (*.*)\0*.*\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = path;
	ofn.nMaxFile = LEN(path);
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	if (!GetOpenFileName(&ofn))
		return;

	pathlen = _tcsclen(path);
	name = &path[pathlen+1];

	if (*name) {
		/* Mulitiple selection: dir\0file\0file...
		   `path` is the directory
		   `name` the filename */
		path[pathlen] = _T('\\');
		while (*name) {
			namelen = _tcslen(name);
			memcpy_s(&path[pathlen+1], sizeof(path), name,
			    sizeof(_TCHAR) * (namelen + 1));
			SendMessage(listbox, LB_ADDSTRING, 0, (LPARAM)path);
			name += namelen+1;
		}
	} else {
		/* Single selection, `path` is the full path */
		SendMessage(listbox, LB_ADDSTRING, 0, (LPARAM)path);
	}
}

static void
removefile(HWND dialog)
{
	HWND listbox;
	LRESULT idx;

	if (!((listbox = GetDlgItem(dialog, IDC_EXELIST)))) {
		warn(_T("Failed to get listbox handle"));
		return;
	}

	idx = SendMessage(listbox, LB_GETCURSEL, 0, 0);
	if (idx == LB_ERR)
		return;

	idx = SendMessage(listbox, LB_DELETESTRING, (WPARAM)idx, 0);
	if (idx == LB_ERR) {
		warnx(_T("Failed to remove list box item."));
		return;
	}
}

static void
movefile(HWND dialog, int offset)
{
	HWND listbox;
	LRESULT idx;
	LRESULT count;
	LRESULT res;
	_TCHAR path[4096];

	if (!((listbox = GetDlgItem(dialog, IDC_EXELIST)))) {
		warn(_T("Failed to get list box handle"));
		return;
	}

	idx = SendMessage(listbox, LB_GETCURSEL, 0, 0);
	if (idx == LB_ERR || idx + offset < 0)
		return;

	if (offset > 0) {
		count = SendMessage(listbox, LB_GETCOUNT, 0, 0);
		if (count == LB_ERR) {
			warnx(_T("Failed to get list box item count."));
			return;
		}

		if (idx + offset >= count)
			return;
	}

	if (!getlbstring(listbox, (int)idx, path, LEN(path)))
		return;

	SendMessage(listbox, LB_DELETESTRING, (WPARAM)idx, 0);
	if (idx == LB_ERR) {
		warnx(_T("The entry could not be removed from the list ")
		    _T("box."));
		return;
	}

	res = SendMessage(listbox, LB_INSERTSTRING, (WPARAM)(idx+offset),
	    (LPARAM)path);
	if (res == LB_ERR || res == LB_ERRSPACE) {
		warnx(_T("The entry could not be added to the list box."));
		return;
	}

	SendMessage(listbox, LB_SETCURSEL, (WPARAM)(idx+offset), 0);
}

static INT_PTR CALLBACK
dialogproc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case WM_INITDIALOG:
		CheckDlgButton(wnd, IDC_GUI, BST_CHECKED);
		return TRUE;

	case WM_CLOSE:
		DestroyWindow(wnd);
		return TRUE;

	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;

	case WM_COMMAND:
		switch (wparam) {
		case IDC_ADD:
			addfile(wnd);
			return TRUE;
		case IDC_REMOVE:
			removefile(wnd);
			return TRUE;
		case IDC_MOVEUP:
			movefile(wnd, -1);
			return TRUE;
		case IDC_MOVEDOWN:
			movefile(wnd, 1);
			return TRUE;
		case IDC_GUI:
			FLAG_GUI = TRUE;
			return TRUE;
		case IDC_TUI:
			FLAG_GUI = FALSE;
			return TRUE;
		case IDC_PACK:
			pack(wnd, 0, NULL);
			return TRUE;
		}
		break;
	}

	return FALSE;
}

int CALLBACK
WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR cmdline, int cmdshow)
{
	HWND dialog;
	MSG msg;
	BOOL ret;

	dialog = CreateDialog(instance, MAKEINTRESOURCE(IDD_FATPACK), NULL,
	    dialogproc);
	if (!dialog)
		err(_T("Failed to create main window"));

	if (!makeusedefault(dialog))
		err(_T("Failed to adjust window position"));

	ShowWindow(dialog, cmdshow);

	while ((ret = GetMessage(&msg, NULL, 0, 0) > 0)) {
		if (!IsDialogMessage(dialog, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	if (ret == -1)
		err(_T("GetMessage()"));

	return 0;
}
