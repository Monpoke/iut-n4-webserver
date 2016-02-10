#include  <stdio.h>
#include  <string.h>
#include "configuration.h"
#include "socket.h"
#include "client.h"
#include <sys/types.h>        
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


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

    file = fdopen(socket_client,"w+");

    int resume = 1;
    int nbLine=0;
    do{

        if(fgets(buffer, sizeof buffer,file) == NULL){
            resume = 0;
        }else{
            removeSpecialCar(buffer);

            nbLine++;

            //fprintf(stdout,buffer);

            processHeaderLine(socket_client,nbLine, buffer);

            if(strlen(buffer)==0){
                resume=0;
            }
        }

    }while(resume==1);


    showWelcome(socket_client);

    exit(0);
}


void processHeaderLine(int socket_client, int nb, char buffer[]){

    if(nb==1){
        // check GET method
        if(strncmp(buffer, "GET",3) == 0){
            const char separator[1] = " ";
            char *token;

            token = strtok(buffer,separator);

            int i = 0;

            for(i=0;i<2;i++){
                token = strtok(NULL,separator);
            }

            int j =0;

            while(token[j] != '/'){
                j++;
            }

            if(token[j+1] != '1' || (token[j+3] != '0' && token[j+3] != '1')){
                callError(socket_client,400);
            }

        }else{
            callError(socket_client,400);
        }
    }

}

void callError(int socket_client, int number){    
    //char src[20];
    char head[500], sentence[200];

    strcpy(head, "HTTP/1.1 ");

    switch(number){
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
    sprintf(nbSentence,"%zu",strlen(sentence));

    strcat(head, "\r\nConnection: close\r\nContent-Length: ");
    strcat(head, nbSentence);
    strcat(head, "\r\n\r\n");

    printf("%s\n",head);

//    const char * errorMessage= "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n";

    char final[600];
    strcpy(final, head);
    strcat(final, sentence);


    if(write(socket_client, final, strlen(final))){

    }

    close(socket_client);

}

void showWelcome(int socket_client){
    
    const char * errorMessage= "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: 8\r\n\r\n200 OK\r\n";

    if(write(socket_client, errorMessage, strlen(errorMessage))){

    }

    close(socket_client);

}

void removeSpecialCar(char *buffer){

    int size = strlen(buffer);

    while(size >0){
        if(buffer[size-1] == '\n' || buffer[size-1] == '\r'){
            buffer[size-1] = '\0';
        }else{
            break;
        }

        size --; 
    }
}
