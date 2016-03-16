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


int GLOBAL_LINE = 0;
mime * mimeTypes = NULL;

char * extractTypesAndExtensions(char*line, char*ptr) {
    line = line;
    ptr = ptr;

    int t = strlen(line);

    int j = 0;
    for (j = 0; !isspace(line[j]) && j < t; j++) {
        ptr++;
    }

    line[j] = '\0';


    // extra moving
    ptr += 1;

    int k = 0;
    t = strlen(ptr);
    for (k = 0; isspace(ptr[0]) && k < t; k++) {
        ptr++;
    }

    return ptr;
}

void createExtension(mime * contenttype, char * contentType, char * extension) {
    if (strlen(extension) == 0) {
        return;
    }
    
    
    // need realloc?

    contenttype[GLOBAL_LINE].extension = extension;
    contenttype[GLOBAL_LINE].content_type = contentType;


    contenttype[GLOBAL_LINE + 1].extension = NULL;
    contenttype[GLOBAL_LINE + 1].content_type = NULL;
    // printf("Create extension %s for contenttype %s\n",extension,contentType);
    GLOBAL_LINE++;
}

void parseExtensions(mime * mimetypes, char * contentType, char * extensions) {

    int t = strlen(extensions);
    int i = 0;
    char * ptr = extensions;
    for (i = 0; i < t; i++) {

        int k = 0;
        for (k = 0; !isspace(ptr[0]) && k < t; k++) {
            ptr++;
        }

        extensions[k] = '\0';

        // si on a trouve la derniere extension
        if (k >= t) {
            break;
        }

        ptr++;

        // create extension
        createExtension(mimetypes, contentType, extensions);

        extensions = ptr;
    }


}




char * getContentType(char * extension) {
    int i = 0;
    while (mimeTypes[i].extension != NULL) {
        printf("%s\n", mimeTypes[i].extension);
        
        if (strcmp(mimeTypes[i].extension, extension) == 0) {
            return mimeTypes[i].content_type;
        }
        i++;
    }
    return NULL;
}




void loadMimes() {

    // try to load mimes
    int mi = open("/etc/mime.types", O_RDONLY);
    if (mi == -1) {
        perror("Mimes");
    } else {
        FILE * mimeStream = fdopen(mi, "r");

        // init malloc
        mimeTypes = malloc(500 * sizeof (mime));
        GLOBAL_LINE = 0;

        //...
        mimeTypes[0].extension = NULL;
        mimeTypes[0].content_type = NULL;


        char contentType[500];
        int i = -1;
        int skippedHeaders = 0;

        while (fgets(contentType, 1000, mimeStream)) {
            i++;

            // skip headers
            if (skippedHeaders == 0) {
                if (isspace(contentType[0])) {
                    skippedHeaders = 1;
                } else {
                    continue;
                }
            } else if (strlen(contentType) < 2) {
                continue;
            }


            char* extensions = contentType;
            extensions = extractTypesAndExtensions(contentType, extensions);

            parseExtensions(mimeTypes, contentType, extensions);
            
            int j =0;
            for(j=0; mimeTypes[j].extension != NULL; j++){
                //printf("%s\n", mimeTypes[j].extension);
            }
            
        }

        printf("%s\n", getContentType("png"));

    }

}