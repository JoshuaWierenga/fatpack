/* Copyright (c) 2021 Sijmen J. Mulder. See LICENSE.md.
   Copyright (c) 2023 Joshua Wierenga.
   Using code from Blink and Cosmopolitan
   Copyright (c) 2023 Justine Alexandra Roberts Tunney under ISC license. */

#include <tchar.h>
#include <Windows.h>
#include "../common/pack.h"
#include "../common/util.h"
#include "cosmogetopt/getopt.h"

#define FATPACK_VERSION     "1.1.0"
#define FATPACK_TUI_VERSION "0.1.0"

#define VERSION \
	_T("Fatpack pe fat binary creator " FATPACK_VERSION "\n\
Fatpack TUI " FATPACK_TUI_VERSION "\n\
Copyright (c) 2021 Sijmen J. Mulder\n\
Copyright (c) 2023 Joshua Wierenga\n\
Fatpack comes with absolutely NO WARRANTY of any kind.\n\
You may redistribute copies of Fatpack under the BSD License.\n\
For more information, see the file named LICENSE.md.\n")

#define OPTS _T("hvo:")

static const _TCHAR USAGE[] = _T(
" [-" OPTS "] PROGRAM [ADDITIONAL PROGRAMS...]\n"
"Options:\n"
"  -h       help\n"
"  -v       show version\n"
"  -o PATH  'fat' universal binary\n"
);

static _TCHAR *fatbinarypath = NULL;

static void
print(HANDLE h, const _TCHAR* s) {
	WriteConsole(h, s, (DWORD)_tcsclen(s), NULL, NULL);
}

void
warnx(const _TCHAR* message) {
	print(GetStdHandle(STD_ERROR_HANDLE), message);
}

// TODO: Check how this looks on 
void
warn(const _TCHAR* info) {
	DWORD num;
	DWORD dw;
	TCHAR buf[4096];
	TCHAR* errstr;

	num = GetLastError();

	dw = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM, 0, num, 0, (LPTSTR)&errstr, 0, NULL);
	if (dw) {
		_sntprintf_s(buf, LEN(buf), _TRUNCATE,
			_T("%s:\n%sError: 0x%X\n"), info, errstr, num);
		LocalFree(errstr);
	} else {
		_sntprintf_s(buf, LEN(buf), _TRUNCATE,
			_T("%s:\nError 0x%X\n"), info, num);
	}

	warnx(buf);
}

static void
pack(int count, _TCHAR **programs) {
	_TCHAR tmpdir[MAX_PATH + 1];
	_TCHAR tmppath[MAX_PATH + 1];
	HANDLE resupdate = NULL;
	int i;

	tmppath[0] = '\0';

	if (!count) {
		warnx(_T("Add one or more executables to pack into a ")
			_T("'fat' universal binary."));
		return;
	}

	if (fatbinarypath == NULL) {
		warnx(_T("Path to output 'fat' universal binary required, ")
			_T("specify with - o"));
		return;
	}

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
		// TODO: Rewrite relative paths as absolute paths with \\.\ when possible
		if (programs[i][0] == _T('\0'))
			goto cleanup;
		if (!packadd(resupdate, i, programs[i]))
			goto cleanup;
	}

	EndUpdateResource(resupdate, FALSE);
	resupdate = NULL;

	if (!CopyFile(tmppath, fatbinarypath, FALSE))
		warn(_T("Failed to write output file"));

cleanup:
	if (resupdate)
		EndUpdateResource(resupdate, TRUE);
	if (tmppath[0])
		DeleteFile(tmppath);
}

static void
printusage(int argc, _TCHAR* argv[], int rc, HANDLE h) {
	print(h, _T("Usage: "));
	print(h, argc > 0 && argv[0] ? argv[0] : _T("Fatpack"));
	print(h, USAGE);
	exit(rc);
}

static void
printversion(void) {
	print(GetStdHandle(STD_OUTPUT_HANDLE), VERSION);
	exit(0);
}

void
getopts(int argc, _TCHAR* argv[])
{
	int opt;
	
	while ((opt = getopt(argc, argv, OPTS)) != -1) {
		switch (opt) {
		case 'o':
			fatbinarypath = optarg;
			break;
		case 'v':
			printversion();
		case 'h':
			printusage(argc, argv, 0, GetStdHandle(STD_OUTPUT_HANDLE));
		default:
			printusage(argc, argv, 48, GetStdHandle(STD_ERROR_HANDLE));
		}
	}
}

int
_tmain(int argc, _TCHAR* argv[])
{
	program_invocation_name = argv[0];

	getopts(argc, argv);
	if (optind == argc) {
		printusage(argc, argv, 48, GetStdHandle(STD_ERROR_HANDLE));
	}

	pack(argc - optind, argv + optind);

	return 0;
}
