#include "um_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

off_t getSize(const char* cstrFileName) {
    struct stat st;
    if (stat(cstrFileName, &st) == 0) {
        return st.st_size;
    } else {
        return -1;
    }
};

struct UM_ParseStatus parseFromFile(const char* cstrFileName) {
    struct UM_ParseStatus parseStatus;
    parseStatus.parsedProgram.program   = (unsigned int*) NULL;
    parseStatus.status                  = UM_PARSED_STATUS_OK;

    off_t fileSize = getSize(cstrFileName);

    if (fileSize == -1) {
        parseStatus.status = UM_PARSED_STATUS_FNF;
        return parseStatus;
    }

    FILE* fFile = fopen(cstrFileName, "rb");
    if (!fFile) {
        parseStatus.status = UM_PARSED_STATUS_UE;
        return parseStatus;
    }

    parseStatus.parsedProgram.length    = (unsigned long)(fileSize/4);
    parseStatus.parsedProgram.program   = malloc((unsigned long)fileSize);
    
    unsigned char* ucBuffer = malloc(4);
    for (unsigned long i = 0 ; i < parseStatus.parsedProgram.length ; i++) {
        if (fread(ucBuffer,1,4,fFile) == EOF) {
            fclose(fFile);
            free((void *)ucBuffer);
            free((void *)parseStatus.parsedProgram.program);

            parseStatus.parsedProgram.program   = (unsigned int*)NULL;
            parseStatus.status                  = UM_PARSED_STATUS_UE;

            return parseStatus;
        }

        unsigned int uiA = 0xFF000000   &   (((unsigned int) ucBuffer[0]) << 24);
        unsigned int uiB = 0xFF0000     &   (((unsigned int) ucBuffer[1]) << 16);
        unsigned int uiC = 0xFF00       &   (((unsigned int) ucBuffer[2]) << 8);
        unsigned int uiD = 0xFF         &   (((unsigned int) ucBuffer[3]));

        parseStatus.parsedProgram.program[i] = uiA | uiB | uiC | uiD;
    }

    fclose(fFile);
    free((void *)ucBuffer);

    return parseStatus;
}

int UM_writeToFile(struct UM_ParsedProgram ppProgram , const char *cstrFileName) {
    FILE* fFile = fopen(cstrFileName, "w");

    if (!fFile) {
        return UM_FILE_WRITE_FNF;
    }

    for (unsigned long i = 0 ; i < ppProgram.length ; i++) {
        fprintf(fFile, "%u\n", ppProgram.program[i]);
    }   

    fclose(fFile);
    
}