/* Copyright (c) 2021, Sijmen J. Mulder. See LICENSE.md.
   Copyright (c) 2023, Joshua Wierenga.
   Using code from Blink and Cosmopolitan
   Copyright (c) 2023, Justine Alexandra Roberts Tunney under ISC license. */

#define _WIN32_WINNT _WIN32_WINNT_WINXP
#include <tchar.h>
#include <Windows.h>
#include "../common/pack.h"
#include "cosmogetopt/getopt.h"

#define FATPACK_VERSION     _T("1.1.0")
#define FATPACK_TUI_VERSION _T("0.1.0")

#define VERSION \
	_T("Fatpack pe fat binary creator ") FATPACK_VERSION _T("\n\
Fatpack TUI ") FATPACK_TUI_VERSION _T("\n\
Copyright (c) 2021 Sijmen J. Mulder\n\
Copyright (c) 2023 Joshua Wierenga\n\
Fatpack comes with absolutely NO WARRANTY of any kind.\n\
You may redistribute copies of Fatpack under the BSD License.\n\
For more information, see the file named LICENSE.md.\n")

#define OPTS _T("hvo:gt")

#define USAGE \
	_T(" [-") OPTS _T("] PROGRAM [ADDITIONAL PROGRAMS...]\n\
Options:\n\
  -h       help\n\
  -v       show version\n\
  -o PATH  'fat' universal binary\n\
  -g       optimise for gui applications\n\
  -t       optimise for tui applications\n")

static void
print(HANDLE h, const _TCHAR *s)
{
	WriteConsole(h, s, (DWORD)_tcsclen(s), NULL, NULL);
}

__declspec(noreturn) static void
printusage(int argc, _TCHAR *argv[], int rc, HANDLE h)
{
	print(h, _T("Usage: "));
	print(h, argc > 0 && argv[0] ? argv[0] : _T("Fatpack"));
	print(h, USAGE);
	exit(rc);
}

__declspec(noreturn) static void
printversion(void)
{
	print(GetStdHandle(STD_OUTPUT_HANDLE), VERSION);
	exit(0);
}

static void
getopts(int argc, _TCHAR *argv[])
{
	int opt;
	
	while ((opt = getopt(argc, argv, OPTS)) != -1) {
		switch (opt) {
		case 'o':
			FLAG_FATBINARY = optarg;
			break;
		case 'g':
			FLAG_GUI = TRUE;
			break;
		case 't':
			FLAG_GUI = FALSE;
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
_tmain(int argc, _TCHAR *argv[])
{
	program_invocation_name = argv[0];

	getopts(argc, argv);
	if (optind == argc) {
		printusage(argc, argv, 48, GetStdHandle(STD_ERROR_HANDLE));
	}

	pack(NULL, argc - optind, argv + optind);

	return 0;
}
