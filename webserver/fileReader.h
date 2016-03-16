/* 
 * File:   fileReader.h
 * Author: bourgeop
 *
 * Created on 2 mars 2016, 15:07
 */

#ifndef FILEREADER_H
#define	FILEREADER_H

#include "http.h"


char *rewrite_url(char *url);
int check_and_open(const char* url, const char *document_root, http_request *request);
int get_file_size(int fd);

#endif	/* FILEREADER_H */

