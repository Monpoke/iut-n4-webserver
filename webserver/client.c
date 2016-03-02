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
    FILE *file;
    file = fdopen(socket_client, "w+");

    int nbLine = 1;


    /**
     * DECLARE REQUESTS AND METHODS
     */
    http_request client_request;
    client_request.bad_request=0;


    int firstDataReceived = 0;
    while (1) {


        fgets_or_exit(contentLineClient, sizeof contentLineClient, file);
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
                parse_http_request(contentLineClient, &client_request);
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
    if (client_request.bad_request != 0) {
        send_response(socket_client, 400, "Bad Request", "Bad Request\r\n");
    } 
    else if (client_request.method == HTTP_UNSUPPORTED) {
        send_response(socket_client, 405, "Method Not Allowed", "Method Not Allowed\r\n");
    } 
    else if(strcmp(client_request.url, "/") == 0){
        send_response(socket_client, 200, "OK", "<h1>Hello World!</h1> <p>My first content</p>\r\n");
    } else {
        send_response(socket_client, 404, "Not Found", "<h1>My bad!</h1> <p>Sorry, this page doesn't exists...</p>\r\n");
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

        request->bad_request=1;
        
        return 0;

    }

    /**
     * Find supported method
     */
    if (strcmp(list[0], "GET") == 0) {
        request->method = HTTP_GET;
    } else {
        returnCode = 0;
    }


    /**
     * HTTP URL
     */
    printf("I FOUND URL %s\n\n", list[1]);
    
    request->url = list[1];

    /**
     * Version 
     */
    if (strlen(list[2]) == 8 && (strcmp(list[2], "HTTP/1.0") == 0 || strcmp(list[2], "HTTP/1.1") == 0)) {
        request->major_version = 1;
        char *miniVersion = list[2];
        request->minor_version = miniVersion[7] - '0';
    } else {
        // badd HTTP VERSION
        request->bad_request=1;
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

/**
 * Sends response to client
 * @param client
 * @param code
 * @param reason_phrase
 * @param message_body
 */
void send_response(int client, int code, const char *reason_phrase, const char * message_body) {

    //send_status(client)
    
    // http version
    write(client, "HTTP/1.1 ", 9);

    // convert int code to string
    char codeString[3];
    sprintf(codeString, "%d ", code);

    write(client, codeString, strlen(codeString));


    // reason phrase
    write(client, reason_phrase, strlen(reason_phrase));

    // connection closed
    write(client, "\r\nConnection: close", 19);

    // content length
    char totalLength[10];
    sprintf(totalLength, "%zu", strlen(message_body));

    // Content length
    write(client, "\r\nContent-length: ", 18);
    write(client, totalLength, strlen(totalLength));

    write(client, "\r\n\r\n", 4);

    // message body
    write(client, message_body, strlen(message_body));


}









