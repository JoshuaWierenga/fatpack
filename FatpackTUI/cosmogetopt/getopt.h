#ifndef GETOPT_H_
#define GETOPT_H_

_TCHAR *program_invocation_name;

_TCHAR *optarg;
int optind, opterr, optopt, optreset;

int getopt(int, _TCHAR *const[], const _TCHAR*);

#endif /* GETOPT_H_ */
