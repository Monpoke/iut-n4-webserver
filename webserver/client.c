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

    int nbLine = 0;


    /**
     * DECLARE REQUESTS AND METHODS
     */
    http_request client_request;


    int firstDataReceived = 0;
    while (1) {


        fgets_or_exit(contentLineClient, sizeof contentLineClient, file);
        removeSpecialCar(contentLineClient);

        /**
         * If client sends 
         */
        if (strlen(contentLineClient) > 0) {

            /**
             * REQUEST-LINE
             */
            if (nbLine == 1) {
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



    send_response(socket_client, 200, "OK", "Bonjour\r\n");





    // showWelcome(socket_client);

    exit(0);
}

void processHeaderLine(int socket_client, int lineNumber, char buffer[], http_request *req) {

    socket_client = socket_client;

    if (lineNumber == 1) {

        printf("PARSE REQUEST:\n");
        parse_http_request(buffer, req);

        printf("REQ.URL: %s\n\n", req->url);
    }
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


        return 0;

    }

    /**
     * Find supported method
     */
    if (strncmp(list[0], "GET", 3) && strlen(list[0]) == 3) {
        request->method = HTTP_GET;
    } else {
        returnCode = 0;
    }


    /**
     * HTTP URL
     */
    request->url = list[1];

    /**
     * Version 
     */
    if (strlen(list[2]) == 8 && (strncmp(list[2], "HTTP/1.0", 8) || strncmp(list[2], "HTTP/1.1", 8))) {
        request->major_version = 1;
        char *miniVersion = list[2];
        request->minor_version = miniVersion[7] - '0';
    } else {
        returnCode = 0;
    }



    return returnCode;
}

/**
 * Call error
 * @param socket_client
 * @param number
 */
void callError(int socket_client, int number) {
    //char src[20];
    char head[500], sentence[200];

    strcpy(head, "HTTP/1.1 ");

    switch (number) {
        case 404:
            strcat(head, "404 Not Found");
            strcpy(sentence, "404 Not Found");
            break;

        default:
            strcat(head, "400 Bad Request");
            strcpy(sentence, "400 Bad request");
            break;
    }


    strcat(sentence, "\r\n");

    char nbSentence[10];
    sprintf(nbSentence, "%zu", strlen(sentence));

    strcat(head, "\r\nConnection: close\r\nContent-Length: ");
    strcat(head, nbSentence);
    strcat(head, "\r\n\r\n");

    char final[600];
    strcpy(final, head);
    strcat(final, sentence);

    if (write(socket_client, final, strlen(final))) {

    }

    close(socket_client);
    exit(0);

}

void showWelcome(int socket_client) {

    const char * errorMessage = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: 8\r\n\r\n200 OK\r\n";

    if (write(socket_client, errorMessage, strlen(errorMessage))) {

    }

    close(socket_client);

}

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









