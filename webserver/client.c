#include  <stdio.h>
#include  <string.h>
#include "configuration.h"
#include "socket.h"
#include <sys/types.h>        
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "tools.h"
#include  "http.h"
#include "client.h"

void clientLoop(int ID, int socket_client) {
    ID = ID;


    /**
     * TMP BUFFER
     */
    char contentLineClient[4096];
    FILE *clientFile;
    clientFile = fdopen(socket_client, "w+");

    int nbLine = 1;


    /**
     * DECLARE REQUESTS AND METHODS
     */
    http_request client_request;
    client_request.bad_request = 0;

    int BAD_REQUEST;


    int firstDataReceived = 0;
    while (1) {


        fgets_or_exit(contentLineClient, sizeof contentLineClient, clientFile);
        removeSpecialCar(contentLineClient);

        printf("RECEIVED: %s\n", contentLineClient);


        /**
         * If client sends 
         */
        if (strlen(contentLineClient) > 0) {

            /**
             * REQUEST-LINE
             */
            if (nbLine == 1) {
                printf("PARSE HTTP REQUESTTTTT\n");
                BAD_REQUEST = parse_http_request(contentLineClient, &client_request);
            } else {
                // PROCESS OTHER HEADERS IF NEEDED
            }


            firstDataReceived = 1;
            nbLine++;


            fprintf(stdout, contentLineClient);

        } else if (firstDataReceived == 1) {
            printf("ALERT ALERT, BREAK\n\n");
            break;
        }

    }


    /**
     * Sends response 
     */
     if (BAD_REQUEST == 0) {
        send_response(clientFile, 400, "Bad Request", "Bad Request\r\n");
    } else if (!(client_request.major_version == 1 && (client_request.minor_version >= 0 || client_request.minor_version <= 1))) {
        send_response(clientFile, 505, "HTTP Version not supported", "HTTP Version not supported\r\n");
    } 
    else if (client_request.method == HTTP_UNSUPPORTED) {
        send_response(clientFile, 405, "Method Not Allowed", "Method Not Allowed\r\n");
    } else if (strcmp(client_request.url, "/") == 0) {
        send_response(clientFile, 200, "OK", "<h1>Hello World!</h1> <p>My first content</p>\r\n");
    } else {
        send_response(clientFile, 404, "Not Found", "<h1>My bad!</h1> <p>Sorry, this page doesn't exists...</p>\r\n");
    }




    // showWelcome(socket_client);

    exit(0);
}

/**
 * Parse the HTTP Request
 * @param request_line
 * @param request
 * @return 
 */
int parse_http_request(const char *request_line, http_request *request) {
    request_line = request_line;
    request = request;

    /**
     * 1 if correct. 
     */
    int returnCode = 1;


    // Default method
    request->method = HTTP_UNSUPPORTED;


    char **list;
    size_t i, len;
    explode(request_line, " ", &list, &len);

    if (len != 3) {
        /* free list */
        for (i = 0; i < len; ++i)
            free(list[i]);
        free(list);

        printf("COUCOU, TU BUGUES\n\n");
        
        return 0;

    }

    /**
     * Find supported method
     */
    if (strcmp(list[0], "GET") == 0) {
        request->method = HTTP_GET;
    }



    /**
     * HTTP URL
     */
    request->url = list[1];

    /**
     * Version 
     */
    if (strlen(list[2]) == 8 && strncmp(list[2], "HTTP/", 5) == 0/*&& (strcmp(list[2], "HTTP/1.0") == 0 || strcmp(list[2], "HTTP/1.1") == 0)*/) {
        char *miniVersion = list[2];
        request->major_version = miniVersion[5] - '0';

        // minor version
        request->minor_version = miniVersion[7] - '0';
    } else {
        // badd HTTP VERSION
        request->bad_request = 1;
        returnCode = 0;
    }


    return returnCode;
}

/**
 * Clear special characters
 * @param buffer
 */
void removeSpecialCar(char *buffer) {

    int size = strlen(buffer);

    while (size > 0) {
        if (buffer[size - 1] == '\n' || buffer[size - 1] == '\r') {
            buffer[size - 1] = '\0';
        } else {
            break;
        }

        size--;
    }
}

/**
 * 
 * @param buffer
 * @param size
 * @param stream
 * @return 
 */
char *fgets_or_exit(char *buffer, int size, FILE *stream) {

    if (fgets(buffer, size, stream) == NULL) {
        exit(0);
    } else {
        return buffer;
    }
}

void send_status(FILE *client, int code, const char * reason_phrase) {
    fprintf(client, "HTTP/1.1 %d %s\r\n", code, reason_phrase);
}

/**
 * Sends response to client
 * @param client
 * @param code
 * @param reason_phrase
 * @param message_body
 */
void send_response(FILE *client, int code, const char *reason_phrase, const char * message_body) {

    // send status
    send_status(client, code, reason_phrase);

    // connection closed
    fprintf(client, "Connection: close\r\n");

    // content length
    fprintf(client, "Content-length: %zu\r\n", strlen(message_body));
    ;

    fprintf(client, "\r\n");

    // message body
    fprintf(client, "%s", message_body);

    // sends
    fflush(client);

}









