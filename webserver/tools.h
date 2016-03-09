/* 
 * File:   tools.h
 * Author: bourgeop
 *
 * Created on 24 février 2016, 15:57
 */

#ifndef TOOLS_H
#define	TOOLS_H
#include <sys/types.h>

char *strdup(const char *src) ;
void explode(const char *src, const char *tokens, char ***list, size_t *len);
char *replace_str(char *str, char *orig, char *rep);

#endif	/* TOOLS_H */

