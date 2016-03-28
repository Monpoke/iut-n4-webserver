#include "stats.h"
#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>      
 #include <sys/mman.h>


static web_stats * stats;
//static web_stats * stats_shared;

int init_stats(){
    
    // memory
    stats = mmap(NULL, sizeof(web_stats), PROT_EXEC |PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    stats->served_connections=0;
    stats->served_requests=0;
    stats->ok_200=0;
    stats->ko_400=0;
    stats->ko_403=0;
    stats->ko_404=0;
    return 0;
}

web_stats * get_stats(){
    return stats;
}


/**
Stats
*/
void send_stats(FILE * clientFile, http_request * client_request){
    
    // served request    
    char message[800];
    char buffer[100];
    strcpy(message, "<!doctype html>\n<html><head><title>Stats</title><meta charset='utf-8'></head>");
    strcat(message, "<body><h1>Stats...</h1><table><tr><th>Name</th><th>Value</th></tr>");
    
    sprintf(buffer, "<tr><td>Served requests</td><td>%i</td></tr>", stats->served_requests);
    strcat(message, buffer);
    
    sprintf(buffer, "<tr><td>Served connections</td><td>%i</td></tr>", stats->served_connections);
    strcat(message, buffer);
    
    sprintf(buffer, "<tr><td>200</td><td>%i</td></tr>", stats->ok_200);
    strcat(message, buffer);
    
    sprintf(buffer, "<tr><td>400</td><td>%i</td></tr>", stats->ko_400);
    strcat(message, buffer);
    
    sprintf(buffer, "<tr><td>403</td><td>%i</td></tr>", stats->ko_403);
    strcat(message, buffer);
    
    sprintf(buffer, "<tr><td>404</td><td>%i</td></tr>", stats->ko_404);
    strcat(message, buffer);
    
    strcat(message, "</table></body></html>");

    send_response(clientFile, 200, "OK", message);
}