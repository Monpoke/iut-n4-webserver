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


#endif	/* MIME_H */

