#ifndef UM_PARSER
#define UM_PARSER

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#define UM_PARSED_STATUS_OK 0
#define UM_PARSED_STATUS_UE 1 // Unknown error occured
#define UM_PARSED_STATUS_FNF 2 // File Not found

#define UM_FILE_WRITE_OK 0
#define UM_FILE_WRITE_UE 1 // Unknown error occured
#define UM_FILE_WRITE_FNF 2 // File Not found

struct UM_ParsedProgram {
    unsigned int* program;
    unsigned long length;
};

struct UM_ParseStatus {
    struct UM_ParsedProgram parsedProgram;
    unsigned int status;
};

off_t getSize(const char *); // return bytes
struct UM_ParseStatus parseFromFile(const char*);
int UM_writeToFile(struct UM_ParsedProgram, const char*);

#endif