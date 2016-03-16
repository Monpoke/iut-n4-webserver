#include  <stdio.h>
#include  <string.h>
#include "configuration.h"
#include "socket.h"
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
#include "fileReader.h"
#include "mime.h"

void clientLoop(int socket_client, char * document_root) {

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


        /**
         * If client sends 
         */
        if (strlen(contentLineClient) > 0) {

            /**
             * REQUEST-LINE
             */
            if (nbLine == 1) {
                BAD_REQUEST = parse_http_request(contentLineClient, &client_request);
            } else {
                // PROCESS OTHER HEADERS IF NEEDED
            }


            firstDataReceived = 1;
            nbLine++;


            fprintf(stdout, contentLineClient);

            fprintf(stdout, "\n");
        } else if (firstDataReceived == 1) {
            break;
        }

    }

    // request
    printf("\nREQUESTED URL: %s\n", client_request.absolute_url);


    fprintf(stdout, "\n");

    /**
     * Sends response 
     */
    if (BAD_REQUEST == 0) {
        send_response(clientFile, 400, "Bad Request", "Bad Request\r\n");
    } else if (!(client_request.major_version == 1 && (client_request.minor_version >= 0 || client_request.minor_version <= 1))) {
        send_response(clientFile, 505, "HTTP Version not supported", "HTTP Version not supported\r\n");
    } else if (client_request.method == HTTP_UNSUPPORTED) {
        send_response(clientFile, 405, "Method Not Allowed", "Method Not Allowed\r\n");
    } else {

        // check file
        int filepath = check_and_open(client_request.absolute_url, document_root, &client_request);
        if (filepath == -1) {
            send_response(clientFile, 404, "Not Found", "<h1>My bad! 404 error!</h1> <p>Sorry, this page doesn't exists...</p>\r\n");
        } else if(filepath == -2) {
            send_response(clientFile, 403, "Forbidden", "<h1>403 - Forbidden</h1> <p>Sorry! Access denied :(</p>\r\n");
        }else{
            send_file(clientFile, filepath,&client_request);
        }
    }




    fprintf(stdout, "Client closed connection.\n\n");
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
    request->absolute_url = rewrite_url(list[1]);


    /**
     * Version 
     */
    if (strlen(list[2]) == 8 && strncmp(list[2], "HTTP/", 5) == 0) {
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

/**
 * 
 * @param client
 * @param file
 */
void send_file(FILE *client, int file, http_request * client_request) {
    file = file;
    int size = get_file_size(file);

    FILE *fileStream;
    fileStream = fdopen(file, "r");

    // send status
    send_status(client, 200, "OK");
    fprintf(client, "Connection: close\r\n");

    send_contenttype(client, client_request);

    // content length
    fprintf(client, "Content-length: %d\r\n", size);
    fprintf(client, "\r\n");

    char part[255];
    while (fgets(part, 255, fileStream)) {
        fprintf(client, "%s", part);
    }

    // sends
    fflush(client);
}

void send_contenttype(FILE * client, http_request * client_request) {
    // find mimetype
    char * contenttype = getContentType(client_request->extension);

    fprintf(client, "Content-type: %s\r\n", contenttype);
}