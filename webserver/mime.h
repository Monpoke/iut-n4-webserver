/* 
 * File:   mime.h
 * Author: bourgeop
 *
 * Created on 9 mars 2016, 15:41
 */

#ifndef MIME_H
#define	MIME_H


typedef struct  {
    char *content_type;
    char *extension;
} mime;


void loadMimes();

char * getContentType(char * extension);
void parseExtensions(mime * mimetypes, char * contentType, char * extensions);
void createExtension(mime * contenttype, char * contentType, char * extension);
char * extractTypesAndExtensions(char*line, char*ptr) ;


#endif	/* MIME_H */

