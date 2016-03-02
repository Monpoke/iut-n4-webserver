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


/**
 * Delete query string
 * @param url
 * @return 
 */
char *rewrite_url(char *url) {
   
    if(strcmp(url, "/") == 0){
        return "index.html";
    }
    
    char str[(int)strlen(url)];
    
    strcpy(str,url);
    
    const char s[2] = "?";
    char *token;
     
    token = strtok(str,s);
    
    return token; 
}