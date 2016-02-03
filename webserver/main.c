#include  <stdio.h>
#include  <string.h>
#include "configuration.h"
#include "socket.h"
#include <sys/types.h>          /* See NOTES */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "client.h"

int CLIENT_ID = 0;

/**
 * Process signals
 * @param sig
 */
void traitement_signal(int sig) {
    printf("Signal received: %d\n", sig);

    int status = 0;

    // kill zombies
    while (waitpid(-1, &status, WNOHANG)!=-1) {
    }

}

/**
 * Init signals
 */
void initialiser_signaux(void) {
    // ignore signal SIGPIPE
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("signal");
    }
    
    struct sigaction sa;
    sa.sa_handler = traitement_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction(SIGCHLD)");
    }
}

/**
 * Listens to clients
 * @param server
 */
void createClient(int server) {


    int socket_client;


    // BOUCLE POUR ECOUTER TOUUUUT LES CLIENTS
    while (1) {
        printf("Waiting for client %d\n", CLIENT_ID);
        socket_client = accept(server, NULL, NULL);
        CLIENT_ID++;

        if (socket_client == -1) {
            perror("accept");
            /*  traitement d’erreur  */
            printf("Server error...");
        }

        pid_t pid = fork();

        // Call client
        if (pid == 0) {
            clientLoop(CLIENT_ID, socket_client);
        } else {


            close(socket_client);
        }



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
    createClient(server);

    return 0;

}


