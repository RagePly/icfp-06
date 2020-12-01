#include "um_parser.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/types.h>
#include <stdlib.h>



unsigned int concatonate_bytes(struct byte_quintuple bq) {
  return ((((unsigned int) bq.A) << 24) & 0xFF000000) |
  ((((unsigned int) bq.B) << 16) & 0xFF0000) |
  ((((unsigned int) bq.C) << 8) & 0xFF00) |
  (((unsigned int) bq.D) & 0xFF);
};

struct parse_info parse_quintuple(FILE* f) {
  char b[4];
  int info = PARSE_INFO_OK;
  for (unsigned int i = 0; i < 4 ; i++) {
    char c = fgetc(f);
    if (c==EOF) {
      info = PARSE_INFO_EOF;
      break;
    }
    b[i] = c;
  }
  struct byte_quintuple bq = {b[0],b[1],b[2],b[3]};
  unsigned int opcode = concatonate_bytes(bq);
  if (parse_opcode(opcode) == 0xE || parse_opcode(opcode) == 0xF) {
    info = PARSE_INFO_INVALID_OPCODE;
  }
  struct parse_info pf = {opcode, info};

  return pf;
};

unsigned int parse_opcode(unsigned int instruction) {
  return 0xF & (instruction >> 28);
};

struct registerp parse_regp(unsigned int instruction) {
  struct registerp reg = {
    0b111 & instruction,
    0b111 & (instruction >> 3),
    0b111 & (instruction >> 6)
  };

  return reg;
};
unsigned int parse_ort_pointer(unsigned int instruction) {
  return 0b111 & (instruction >> 25);
};
unsigned int parse_immediate(unsigned int instruction) {
  return 0x1FFFFFF & instruction;
};

off_t getSize(const char* cstrFileName) {
    struct stat st;
    if (stat(cstrFileName, &st) == 0) {
        return st.st_size;
    } else {
        return -1;
    }
};

struct UMParseOutput parseUMZFile(const char* cstrFileName) {
    struct UMParseOutput umpoParse;
    umpoParse.program == (unsigned int*)NULL;
    
    // Check filesize:
    off_t fileSize = getSize(cstrFileName);

    if (fileSize == -1) {
        // File not found error
        umpoParse.status = UM_PARSE_STATUS_FNF;
        return umpoParse;
    }

    unsigned long ulNrOpcodes = ((unsigned long) fileSize) / 4;
    umpoParse.program = (unsigned int*) malloc(ulNrOpcodes*4);
    umpoParse.programLen = ulNrOpcodes;

    FILE* fFile = fopen(cstrFileName, "rb");
    printf("Number of bytes to read: %ld\n", ((size_t)ulNrOpcodes)*4 );

    size_t retCode = fread(umpoParse.program, 4, (size_t)ulNrOpcodes,fFile);
    if (retCode == 0) {
        // Close the file stream
        fclose(fFile);
        // Deallocate
        free((void *)umpoParse.program);
        umpoParse.status = UM_PARSE_STATUS_PARSE_ERR;
        umpoParse.program = NULL;
        return umpoParse;
    }

    //for (unsigned long i = 0; i < ulNrOpcodes ; i++) {
    //    struct parse_info piParse = parse_quintuple(fFile);
    //    if (piParse.status != PARSE_INFO_OK) {
    //        // Close the file stream
    //        fclose(fFile);
    //        // Deallocate
    //        free((void *)umpoParse.program);
    //        umpoParse.status = UM_PARSE_STATUS_PARSE_ERR;
    //        umpoParse.program = NULL;
    //        return umpoParse;
    //    }
//
    //    umpoParse.program[i] = piParse.opcode;
    //}

    fclose(fFile);
    umpoParse.status = UM_PARSE_STATUS_OK;
    reformatUMPO(umpoParse);
    return umpoParse;
};

void reformatUMPO(struct UMParseOutput umpo) {
    for (unsigned long i = 0; i < umpo.programLen; i++) {
        unsigned int uiTmp = umpo.program[i];
        umpo.program[i] = 0xFF & (uiTmp >> 24) |
                        0xFF00 & (uiTmp >> 8) |
                        0xFF0000 & (uiTmp << 8) |
                        0xFF000000 & (uiTmp << 24);
    }
}


void printProgramArr(struct UMParseOutput umpo){
    printf("Length = %ld\nProgram content:\n",umpo.programLen);
    for (unsigned long i = 0; i < umpo.programLen; i++) {
        printf("%ld: %X\n", i,umpo.program[i]);
    }
    printf("\n");
};
