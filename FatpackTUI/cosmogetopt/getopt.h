#ifndef GETOPT_H_
#define GETOPT_H_

extern _TCHAR *program_invocation_name;

extern _TCHAR *optarg;
extern int optind, opterr, optopt, optreset;

int getopt(int, _TCHAR *const[], const _TCHAR*);

#endif /* GETOPT_H_ */
