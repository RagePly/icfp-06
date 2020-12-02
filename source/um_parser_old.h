#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef UM_PARSER
#define UM_PARSER

#define PARSE_INFO_OK 0
#define PARSE_INFO_INVALID_OPCODE 1
#define PARSE_INFO_EOF -1

#define UM_PARSE_STATUS_OK 0
#define UM_PARSE_STATUS_FNF -1
#define UM_PARSE_STATUS_PARSE_ERR -2

struct registerp {
    unsigned int A;
    unsigned int B;
    unsigned int C;
};

struct byte_quintuple {
    char A;
    char B;
    char C;
    char D;
};

struct parse_info {
  unsigned int opcode;
  int status;
};

struct UMParseOutput {
    unsigned long programLen;
    unsigned int* program;
    int status;
};

// Parsing input
unsigned int concatonate_bytes(struct byte_quintuple);
struct parse_info parse_quintuple(FILE*);

// Parse instruction
struct registerp parse_regp(unsigned int);
unsigned int parse_opcode(unsigned int);
unsigned int parse_ort_pointer(unsigned int);
unsigned int parse_immediate(unsigned int);

// Parse file
off_t getSize(const char *); // return bytes
struct UMParseOutput parseUMZFile(const char*);
void reformatUMPO(struct UMParseOutput);
 
// Debug functions
void printProgramArr(struct UMParseOutput);

#endif