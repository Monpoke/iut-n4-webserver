#include  <stdio.h>
#include  <string.h>
#include "configuration.h"
#include "socket.h"
#include <sys/types.h>          /* See NOTES */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>

/**
 * Init signals
 */
void initialiser_signaux(void) {
    // ignore signal SIGPIPE
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("signal");
    }

}

/**
 * Main program
 * @return 
 */
int main() {
    printf("Server launched:\n");


    int server = creer_serveur(WEBSERVER_PORT);

    // Signals
    initialiser_signaux();
    
    /**
     * Get client request
     * @return 
     */
    int socket_client;
    while (1) {
        printf("Waiting for client\n");
        socket_client = accept(server, NULL, NULL);
        if (socket_client == -1) {
            perror("accept");
            /*  traitement d’erreur  */
            printf("Server error...");
            exit(1);
        }
        /* On peut  maintenant  dialoguer  avec le  client  */
        const char *message_bienvenue = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin condimentum mollis eros in fringilla.\
        Suspendisse potenti. Proin at eleifend urna. Morbi scelerisque, libero et hendrerit elementum, neque arcu rutrum sem,\
         a dictum lacus lorem placerat velit. Pellentesque nisl lacus, volutpat sed quam sed, lacinia fringilla lorem.\
        In vitae congue tortor, quis luctus nisl. Proin sollicitudin tellus turpis, at tempor velit porttitor ut. \
        Maecenas consectetur at metus a iaculis. Donec suscipit nunc sit amet lorem aliquet eleifend.\
         Praesent id tellus tortor. Integer sapien elit, tincidunt fermentum mauris non, lacinia vestibulum libero. \
        Mauris faucibus tellus eu metus posuere ultrices. Vivamus ac enim placerat, mollis massa sed, pharetra lacus. \
        n risus magna, aliquet in sollicitudin nec, finibus non enim.\n\n";

        /**
         * Sends a message every second
         */
        while (write(socket_client, message_bienvenue, strlen(message_bienvenue)) != -1) {
            printf("Sending welcome message...\n");
            sleep(1);
        }

    }

    return 0;

}


