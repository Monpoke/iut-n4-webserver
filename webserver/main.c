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
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#include "fileReader.h"
#include "client.h"
#include "mime.h"
#include "stats.h"

int CLIENT_ID = 0;

/**
 * Process signals
 * @param sig
 */
void traitement_signal(int sig) {
    // don't want to print...
    sig = sig;

    int status = 0;

    // kill zombies
    while (waitpid(-1, &status, WNOHANG) != -1) {
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
void createClient(int server, char * document_root) {


    int socket_client;


    // BOUCLE POUR ECOUTER TOUUUUT LES CLIENTS
    while (1) {
        //printf("Waiting for client %d\n", CLIENT_ID);
        socket_client = accept(server, NULL, NULL);
        CLIENT_ID++;

        get_stats()->served_connections++;

        if (socket_client == -1) {
            perror("accept");
            /*  traitement d’erreur  */
            printf("Server error...");
        }


        pid_t pid = fork();

        // Call client
        if (pid == 0) {
            clientLoop(socket_client, document_root);
        } else {
            close(socket_client);
        }



    }

}

/**
 * 
 * @param argc
 * @param argv
 */
char * open_documentroot(int argc, char** argv) {
    // directory
    if (argc != 2) {
        printf("usage: ./sushila DIRECTORY\n");
        exit(1);
    }
    
    
    /**
    * check directory
    */
    struct stat s;
    int err = stat(argv[1], &s);
    
    if(-1 == err) {
        if(ENOENT == errno) {
            /* does not exist */
            perror("Repository error");
            exit(1);
        } else {
            perror("stat");
            exit(1);
        }
    } else {
        if(!S_ISDIR(s.st_mode)) {
            perror("Document root error");
            exit(1);
        } 
    }
    
    return argv[1];
}


/**
 * Main program
 * @return 
 */
int main(int argc, char** argv) {

    
    // document root
    char* docroot = open_documentroot(argc, argv);


    int server = creer_serveur(WEBSERVER_PORT);
    printf("Server launched:\n");

    // Mimes
    loadMimes();
    
    // Load stats
    init_stats();
    
    
    // Signaux
    initialiser_signaux();

    
    /**
     * Get client request
     * @return 
     */
    createClient(server, docroot);

    return 0;

}
