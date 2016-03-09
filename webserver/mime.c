#include  <stdio.h>
#include  <string.h>
#include "configuration.h"
#include "socket.h"
#include <sys/types.h>
#include <ctype.h>/* See NOTES */
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "fileReader.h"
#include "mime.h"

char * copyWord(char*line, char*ptr) {
    line = line;
    ptr = ptr;

    int j = 0;
    for (j = 0; isspace(ptr[j]); j++) {
        ptr++;
    }
    line[j] = '\0';

    // affichage du caractere de break
    //printf("-> %c\n", (ptr[j]));



    return ptr;
}

void loadMimes() {

    // try to load mimes
    int mi = open("/etc/mime.types", O_RDONLY);
    if (mi == -1) {
        perror("Mimes");
    } else {
        FILE * mimeStream = fdopen(mi, "r");

        mime typesMime[1000];
        char line[500];
        int i = -1;
        int skippedHeaders = 0;

        while (fgets(line, 1000, mimeStream)) {
            i++;

            // skip headers
            if (skippedHeaders == 0) {
                if (isspace(line[0])) {
                    skippedHeaders = 1;
                } else {
                    continue;
                }
            }


            printf("%s",line);


            //  printf("%s", line);
            mime typeActuel;

            char * ptr = line;
           // ptr = copyWord(line, ptr);
                
            int j=0;
            while(!isspace(ptr[j])){
                ptr++;
                j++;
            }
            
            // content-type
            char contenttype[255];
            strcpy(contenttype, ptr);
            
            printf("->\n%s\n",contenttype);
            



            //printf("%s", ptr);

            if (i == 21)
                break;

            typesMime[i] = typeActuel;
        }



        typesMime[0] = typesMime[0];
    }

}