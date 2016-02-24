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
#include  "http.h"
#include "client.h"

char *strdup(const char *src) {
    char *tmp = malloc(strlen(src) + 1);
    if (tmp)
        strcpy(tmp, src);
    return tmp;
}

void explode(const char *src, const char *tokens, char ***list, size_t *len) {
    if (src == NULL || list == NULL || len == NULL)
        return;

    char *str, *copy, **_list = NULL, **tmp;
    *list = NULL;
    *len = 0;

    copy = strdup(src);
    if (copy == NULL)
        return;

    str = strtok(copy, tokens);
    if (str == NULL)
        goto free_and_exit;

    _list = realloc(NULL, sizeof *_list);
    if (_list == NULL)
        goto free_and_exit;

    _list[*len] = strdup(str);
    if (_list[*len] == NULL)
        goto free_and_exit;
    (*len)++;


    while ((str = strtok(NULL, tokens))) {
        tmp = realloc(_list, (sizeof *_list) * (*len + 1));
        if (tmp == NULL)
            goto free_and_exit;

        _list = tmp;

        _list[*len] = strdup(str);
        if (_list[*len] == NULL)
            goto free_and_exit;
        (*len)++;
    }


free_and_exit:
    *list = _list;
    free(copy);
}

void clientLoop(int ID, int socket_client) {
    ID = ID;
    /* On peut  maintenant  dialoguer  avec le  client  */
    // const char *message_bienvenue = "Welcome ON Sushila SERVER!\n\n";

    /**
     * Sends a message every second.
     */
    /*  if (write(socket_client, message_bienvenue, strlen(message_bienvenue)) != -1) {
          printf("Sending welcome message to %d...\n", ID);
      } else {
          perror("Error welcome message");
      }
     */
    /**
     * TMP BUFFER
     * @param ID
     * @param socket_client
     */
    char buffer[4096];

    //  char header[500];

    /*char reqForme[3];
    reqForme[0] = 'G';
    reqForme[1] = 'E';
    reqForme[2] = 'T';
     */
    FILE *file;

    file = fdopen(socket_client, "w+");

    int nbLine = 0;




    int firstDataReceived = 0;
    while (1) {


        fgets_or_exit(buffer, sizeof buffer, file);
        removeSpecialCar(buffer);

        if (strlen(buffer) > 0) {
            firstDataReceived = 1;
            nbLine++;

            fprintf(stdout, buffer);
            printf(" and calling process hl %d\n", nbLine);

            processHeaderLine(socket_client, nbLine, buffer);
        } else if (firstDataReceived == 1) {
            printf("ALERT ALERT, BREAK\n\n");
            break;
        }

    }







    showWelcome(socket_client);

    exit(0);
}

void processHeaderLine(int socket_client, int nb, char buffer[]) {









    socket_client = socket_client;

    if (nb == 1) {
        http_request req;

        printf("PARSE REQUEST:\n");
        parse_http_request(buffer, &req);

        printf("REQ.URL: %s\n\n", req.url);
    }
}

int parse_http_request(const char *request_line, http_request *request) {
    request_line = request_line;
    request = request;

    /**
     * 1 if correct. 
     */
    int returnCode = 1;
    
    
    request->method = HTTP_UNSUPPORTED;


    // Default method

    printf("COUCOU JHE SUIS LA\n\n");


    char **list;
    size_t i, len;
    explode(request_line, " ", &list, &len);
    for (i = 0; i < len; ++i)
        printf("%zu: %s\n", i + 1, list[i]);

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
    if(strncmp(list[0], "GET", 3) && strlen(list[0]) == 3){
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
    if(strlen(list[2]) == 8 && (strncmp(list[2], "HTTP/1.0", 8) || strncmp(list[2], "HTTP/1.1", 8))  ){
        request->major_version = 1;
        char *miniVersion = list[2];
        request->minor_version = miniVersion[7] - '0';
    }
     else {
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



