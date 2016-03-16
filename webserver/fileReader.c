#include  <stdio.h>
#include  <string.h>
#include "configuration.h"
#include "socket.h"
#include <sys/types.h>          /* See NOTES */
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

#include "fileReader.h"
#include "tools.h"

/**
 * Delete query string
 * @param url
 * @return 
 */
char *rewrite_url(char *url) {
    printf("URL: %s\n", url);

    //url=strdup(url);


    if (strcmp(url, "/") == 0) {
        return "index.html";
    }

    // char absolute[255];
    // strcpy(absolute, url);

    char *cut = strchr(url, '?');

    if (cut != NULL) {
        int index = cut - url;
        url[index] = '\0';
    }

    return url;

}

/**
 * Opens a file
 * @param url
 * @param document_root
 * @return 
 */
int check_and_open(const char* url, const char *document_root) {

    printf("REquesturl: %s\n\n",url);
    
    //document_root+=url;

    char totalPath[500];
    strcpy(totalPath, document_root);
    strcat(totalPath, url);

    /**
     * SECURITY CONCERNS
     */
    // replace ".." and "//"
    char* newPath = replace_str(totalPath, "//", "/");
    newPath = replace_str(newPath, "..", "");
    newPath = replace_str(newPath, "//", "/");

    printf("Open file: %s\n", newPath);

    int d = 0;
    d = fopen(newPath,"r");

    DIR * dir = opendir(newPath);

    if (dir != NULL || errno == "EACCES"){
        perror("No right to go");
        return -2;
    }else if(d == -1){
        perror("Can't access file");
        return -1;
    }else{
        return d;
    }

}

/**
 * Returns file size 
 * @param fd
 * @return 
 */
int get_file_size(int fd) {

    struct stat fileStat;
    if (fstat(fd, &fileStat) < 0)
        return 0;

    printf("truc: %zu\n\n", fileStat.st_size);

    return fileStat.st_size;
}



