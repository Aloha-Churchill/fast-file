#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <omp.h>
#include <dirent.h>
#include "file_function.h"

#define BUFSIZE 4096
#define FILENAMESIZE 128

void connect()
{
    printf("Connected to C extension...\n");
}

void read_file(const char* filename, char* output_buf) {
    FILE *fp = fopen(filename, "r");
    char c;
    if(fp != NULL) {
        size_t newLen = fread(output_buf, sizeof(char), BUFSIZE, fp);

        if ( ferror( fp ) != 0 ) {
            printf("Filename: %s\n", filename);
            fputs("Error reading file\n", stderr);
        }
        fclose(fp);
    }
    else {
        printf("could not open file");
    }
}

void concat_files(const char* dirname, const char* write_filename) {
    struct dirent *dp;
    DIR *dfd;
    if((dfd = opendir(dirname)) == NULL) {
        printf("Could not open directory\n");
    }

    #pragma omp parallel for
    for(int i=0; i<12500; i++) {
        dp = readdir(dfd);
        if(dp == NULL) {
            printf("error\n");
        }
        char fullpathname[BUFSIZE];
        bzero(fullpathname, BUFSIZE);
        strcpy(fullpathname, dirname);
        strcat(fullpathname, "/");
        strcat(fullpathname, dp->d_name);

        // now want to paste contents to shared file like csv or something
        char contents[BUFSIZE];
        bzero(contents, BUFSIZE);
        read_file(fullpathname, contents);

        #pragma omp critical 
        {
            FILE* fp = fopen(write_filename, "a");
            if(fp == NULL) {
                printf("error in opening file\n");
            }
            fprintf(fp, "%s\n", contents);
            fclose(fp);
        }

    }
    printf("done\n");

}
