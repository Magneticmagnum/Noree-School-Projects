#ifndef _MAKEARGV_H
#define _MAKEARGV_H

int makeargv(const char *s, const char *delimiters, char ***argvp);
void freemakeargv(char **argv);

#endif /* _MAKEARGV_H */

