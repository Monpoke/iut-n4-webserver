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

    /* On peut  maintenant  dialoguer  avec le  client  */
    const char *message_bienvenue = "Welcome ON Sushila SERVER!\n\n";

    /**
     * Sends a message every second.
     */
    if (write(socket_client, message_bienvenue, strlen(message_bienvenue)) != -1) {
        printf("Sending welcome message to %d...\n", ID);
    } else {
        perror("Error welcome message");
    }

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

            fprintf(stdout,buffer);

            processHeaderLine(socket_client,nbLine, buffer);

        }

    }while(resume==1);

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
                callError(socket_client,405);
            }

        }else{
            callError(socket_client,400);
        }
    }

}

void callError(int socket_client, int number){
    number+=1;
    
    const char * errorMessage= "HTTP/1.1 400 Bad Request\nConnection: close\nContent-Length: 17\n400 Bad request\n";

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
