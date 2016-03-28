#ifndef STATS_H
#define STATS_H

#include <stdio.h>
#include "http.h"

typedef struct {
    int served_connections;
    int served_requests;
    int ok_200;
    int ko_400;
    int ko_403;
    int ko_404;
    
}web_stats;

web_stats * get_stats();
int init_stats();
void send_stats(FILE * clientFile, http_request * client_request);
#endif