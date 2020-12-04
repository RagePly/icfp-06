#ifndef ULTIMATE_MACHINE
#define ULTIMATE_MACHINE

#define UM_EXIT_STATUS_OK                       0
#define UM_EXIT_STATUS_INVALID_PROGRAM_POINTER  -1
#define UM_EXIT_STATUS_NO_PROGRAM               -2
#define UM_EXIT_STATUS_PP_OOB                   -3 // program pointer out of bounds
#define UM_EXIT_STATUS_INV_OPCODE               -4
#define UM_EXIT_STATUS_INV_ARR_ID               -5
#define UM_EXIT_STATUS_ARR_OOB                  -6
#define UM_EXIT_STATUS_DIV_BY_ZERO              -7
#define UM_EXIT_STATUS_INV_ARR_SIZE             -8
#define UM_EXIT_STATUS_INV_CHR                  -9
#define UM_EXIT_STATUS_INV_PERMISSION           -10
#define UM_EXIT_STATUS_ARR_NOT_ACTIVE           -11


#define UM_MODE_OUTPUT_STATE            0b1
#define UM_MODE_DEBUG                   0b10
#define UM_MODE_WRITE_OUTPUT_FILE       0b100
#define UM_MODE_PRINT_NUMERIC           0b1000
#define UM_MODE_DUMP_STATE_BEFORE_EXIT  0b10000
#define UM_MODE_DEBUG_ARRAY             0b100000

#define UM_REG_A 0
#define UM_REG_B 1
#define UM_REG_C 2

#define UM_TRUE     1
#define UM_FALSE    0


struct UM_Array {
    unsigned long   len;
    unsigned int*   arrayp;
    unsigned int    isActive;
};

struct UM_RegisterID {
    unsigned int A;
    unsigned int B;
    unsigned int C;
};

// Will deallocate the program after use.
void UM_Main(   
    unsigned int*   uiProgram, 
    unsigned long   ulLength, 
    int            *iExitStatus, 
    int             iMode
);



unsigned int UM_findInactive(
    const struct UM_Array  *pArrays,
    const unsigned int      uiCreatedArrays
);

void printDebug(
    unsigned int *uiReg, 
    unsigned int uiOpcode, 
    struct UM_RegisterID registers
);

#endif