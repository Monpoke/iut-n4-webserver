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
    char buffer[255];
    int nbRead = 0;
    do {

        nbRead = read(socket_client, buffer, 255);

        // just end
        buffer[nbRead] = '\0';
        printf("Received: %s\n", buffer);

        
        /**
         * REPLY MESSAGE
         * @param ID
         * @param socket_client
         */
        if (write(socket_client, buffer, nbRead) != -1) {
            printf("Sending welcome message to %d...\n", ID);
        } else {
            perror("Error welcome message");
        }
        
    } while (nbRead>0);

    exit(0);
}
