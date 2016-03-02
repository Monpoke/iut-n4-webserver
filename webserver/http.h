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
    int major_version;
    int minor_version;
    char *url;
} http_request;


#endif	/* HTTP_H */
