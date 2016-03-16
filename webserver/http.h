/* 
 * File:   http.h
 * Author: bourgeop
 *
 * Created on 24 février 2016, 14:07
 */

#ifndef HTTP_H
#define	HTTP_H

typedef enum   {
    HTTP_GET,
    HTTP_UNSUPPORTED
} http_method;

typedef struct  {
    http_method method;
    int bad_request;
    int major_version;
    int minor_version;
    char *url;
    char *absolute_url;
    char * filepath;
    char *extension;
} http_request;


#endif	/* HTTP_H */

