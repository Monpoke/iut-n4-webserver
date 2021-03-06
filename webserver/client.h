/* 
 * File:   client.h
 * Author: bourgeop
 *
 * Created on 3 février 2016, 15:25
 */


#ifndef CLIENT_H
#define	CLIENT_H

#include "http.h"

void clientLoop(int socket_client, char *docroot);
void processHeaderLine(int socket,int lineNumber, char buffer[],http_request *req);
void callError(int socket,int nb);
void removeSpecialCar(char *buffer);
void showWelcome(int socket_client);
char *fgets_or_exit(char *buffer, int size, FILE *stream);
int parse_http_request(const char *request_line, http_request *request);
void send_response(FILE* client, int code, const char *reason_phrase, const char * message_body);
void send_status(FILE *client, int code, const char * reason_phrase);
void send_file(FILE *client, int file, http_request * client_request);
void send_contenttype(FILE * client, http_request * client_request);
void send_stats(FILE * clientFile, http_request * client_request);

#endif	/* CLIENT_H */

