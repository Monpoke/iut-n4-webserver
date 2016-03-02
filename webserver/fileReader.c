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
   
   
    if(strcmp(url, "/") == 0){
        return "index.html";
    }
    
   // char absolute[255];
   // strcpy(absolute, url);
    
    char *cut = strchr(url, '?');
    
    if(cut != NULL){
        int index = cut - url;
        url[index] = '\0';
    }
        
    return url;
    
}