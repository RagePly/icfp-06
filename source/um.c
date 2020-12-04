#include "um.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// TEMPLATE ===============================================================
// ========================================================================


void UM_Main
(
    unsigned int   *uiProgram,
    unsigned long   ulLength, 
    int            *iExitStatus, 
    int             iMode
) 
{
    int iPlaceHolder;
    if (iExitStatus == NULL) {
        iExitStatus = &iPlaceHolder;
    }

    *iExitStatus = UM_EXIT_STATUS_OK;
    // Simple validation of the input =========================================
    if (iMode & UM_MODE_OUTPUT_STATE) {
        printf("Validating input...");
    }

    if (uiProgram == NULL) {
        *iExitStatus = UM_EXIT_STATUS_INVALID_PROGRAM_POINTER;
        return;
    }

    if (ulLength == 0) {
        *iExitStatus = UM_EXIT_STATUS_NO_PROGRAM;
        return;
    }

    if (iMode & UM_MODE_OUTPUT_STATE) {
        printf("Done!\n");
    }
    // ========================================================================


    // Setting up memory ======================================================

    if (iMode & UM_MODE_OUTPUT_STATE) {
        printf("Initializing memory...");
    }

    unsigned int uiReg[8]           = {0,0,0,0,0,0,0,0};
    unsigned int uiNumOfArrays      = 1000;
    unsigned int uiCreatedArrays    = 1;
    struct UM_Array *pArrays        = calloc(1000,sizeof (struct UM_Array));
    pArrays[0].arrayp               = uiProgram;
    pArrays[0].len                  = ulLength;
    pArrays[0].isActive             = UM_TRUE;
    unsigned long ulPP              = 0;

    if (iMode & UM_MODE_OUTPUT_STATE) {
        printf("Done!\n");
    }
    if (iMode & UM_MODE_DEBUG) printf("Init state: Program len: %lu \n", pArrays[0].len);
    
    
    // ========================================================================

    const char* cstrpOpcodes[16] = {
        "ifmv\0",   //conditional move
        "read\0",   // read array
        "put \0",   // write array
        "add \0",   // addition
        "mul \0",   // multiplication
        "div \0",   // divide
        "nand\0",   // not-and
        "halt\0",   // halt program
        "aloc\0",   // allocate array
        "clr \0",   // clear array
        "out \0",   // character output
        "in  \0",   // character input
        "load\0",   // load program
        "set \0",   // set immediate
        "NOP \0",   // Invalid operation
        "NOP \0",   // Invalid operation
    };

    const char* cstrpRegisters[] = {
        "rega\0",
        "regb\0",
        "regc\0",
        "regd\0",
        "rege\0",
        "regf\0",
        "regg\0",
        "regh\0"
    };

    unsigned int            uInstruction;
    unsigned int            uiOpcode;
    struct UM_RegisterID    regID;
    unsigned int            uiImmediateVal;
    unsigned int            uiArrayIndex;
    unsigned char           ucInputBuffer;

    if (iMode & UM_MODE_OUTPUT_STATE) printf("Running...\nOUTPUT:\n");

    while (1) {
        // Operation parsing ==================================================
        if (ulPP >= pArrays[0].len) {
            *iExitStatus = UM_EXIT_STATUS_PP_OOB;
            break;
        }

        uInstruction    = pArrays[0].arrayp[ulPP];
        uiOpcode        = 0xF & (uInstruction >> 28);
        if (iMode & UM_MODE_DEBUG) {printf("%lX:    %s",ulPP,cstrpOpcodes[uiOpcode]);}
        ulPP++;

        // ====================================================================

        // Execution ==========================================================
        if              (uiOpcode > 0xD) {
            *iExitStatus = UM_EXIT_STATUS_INV_OPCODE;
            break;
        } else if       (uiOpcode == 0xD) {
            regID.A           = 0b111     & (uInstruction >> 25);
            uiImmediateVal    = 0x1ffffff & uInstruction;
            uiReg[regID.A]    = uiImmediateVal;
            if (iMode & UM_MODE_DEBUG) {printf("    %s, %u",cstrpRegisters[regID.A], uiImmediateVal);}
        } else {
            regID.A   = 0b111 & (uInstruction >> 6);
            regID.B   = 0b111 & (uInstruction >> 3);
            regID.C   = 0b111 & (uInstruction);
            if (iMode & UM_MODE_DEBUG) {printf("    %s, %s, %s",
                            cstrpRegisters[regID.A],
                            cstrpRegisters[regID.B],
                            cstrpRegisters[regID.C]);
                            }


            if          (uiOpcode == 0x0) { // Conditional Move
                if (uiReg[regID.C]) {
                    uiReg[regID.A] = uiReg[regID.B];
                } 
            } else if   (uiOpcode == 0x1) { // Array index
                if (uiReg[regID.B] >= uiCreatedArrays){
                    *iExitStatus = UM_EXIT_STATUS_INV_ARR_ID;
                    break;
                } else if (pArrays[uiReg[regID.B]].isActive == UM_FALSE) {
                    *iExitStatus = UM_EXIT_STATUS_ARR_NOT_ACTIVE;
                    break;
                } else if (pArrays[uiReg[regID.B]].len <= uiReg[regID.C]) {
                    printf("Array id: %u, Array len: %lu, Index: %u\n",uiReg[regID.B], pArrays[uiReg[regID.B]].len,uiReg[regID.C]);
                    *iExitStatus = UM_EXIT_STATUS_ARR_OOB;
                    break;
                } else {
                    uiReg[regID.A] = pArrays[uiReg[regID.B]].arrayp[uiReg[regID.C]];
                }
            } else if   (uiOpcode == 0x2) { // Array ammendment
                if (uiReg[regID.A] >= uiCreatedArrays) {
                    *iExitStatus = UM_EXIT_STATUS_INV_ARR_ID;
                    break;
                } else if (pArrays[uiReg[regID.A]].isActive == UM_FALSE){
                    *iExitStatus = UM_EXIT_STATUS_ARR_NOT_ACTIVE;
                    break;
                } else if (pArrays[uiReg[regID.A]].len <= uiReg[regID.B]) {
                    *iExitStatus = UM_EXIT_STATUS_ARR_OOB;
                    break;
                } else {
                    pArrays[uiReg[regID.A]].arrayp[uiReg[regID.B]] = uiReg[regID.C];
                }
            } else if   (uiOpcode == 0x3) {
                uiReg[regID.A] = uiReg[regID.B] + uiReg[regID.C];
            } else if   (uiOpcode == 0x4) {
                uiReg[regID.A] = uiReg[regID.B] * uiReg[regID.C];
            } else if   (uiOpcode == 0x5) {
                if (uiReg[regID.C] == 0) {
                    *iExitStatus = UM_EXIT_STATUS_DIV_BY_ZERO;
                    break;
                }
                uiReg[regID.A] = uiReg[regID.B] / uiReg[regID.C];
            } else if   (uiOpcode == 0x6) {
                uiReg[regID.A] = ~(uiReg[regID.B] & uiReg[regID.C]);
            } else if   (uiOpcode == 0x7) {
                break;
            } else if   (uiOpcode == 0x8) {
                uiArrayIndex = UM_findInactive(pArrays, uiCreatedArrays);
                if (uiArrayIndex == 0) { // no created array has been made inactive
                    pArrays[uiCreatedArrays].arrayp     = malloc(4*uiReg[regID.C]);
                    pArrays[uiCreatedArrays].len        = uiReg[regID.C];
                    pArrays[uiCreatedArrays].isActive   = UM_TRUE;
                    uiReg[regID.B] = uiCreatedArrays;
                    if (iMode & UM_MODE_DEBUG_ARRAY) printf("DEBUG<ARRAY>: Created array: id: %u, len: %u\n", uiCreatedArrays, uiReg[regID.C]);
                    uiCreatedArrays++;
                } else { // found allready created, inactive array
                    pArrays[uiArrayIndex].arrayp        = malloc(4*uiReg[regID.C]);
                    pArrays[uiArrayIndex].len           = uiReg[regID.C];
                    pArrays[uiArrayIndex].isActive      = UM_TRUE;
                    uiReg[regID.B] = uiArrayIndex;
                    if (iMode & UM_MODE_DEBUG_ARRAY) printf("DEBUG<ARRAY>: Realloc array: id: %u, len: %u\n", uiArrayIndex, uiReg[regID.C]);
                }
            } else if   (uiOpcode == 0x9) {
                if (uiReg[regID.C] == 0) {
                    *iExitStatus = UM_EXIT_STATUS_INV_ARR_ID;
                    break; 
                }
                
                if (pArrays[uiReg[regID.C]].isActive == UM_TRUE) {
                    free(pArrays[uiReg[regID.C]].arrayp);
                    pArrays[uiReg[regID.C]].len       = 0;
                    pArrays[uiReg[regID.C]].isActive  = UM_FALSE;
                    if (iMode & UM_MODE_DEBUG_ARRAY) printf("DEBUG<ARRAY>: Killed array: id: %u\n", uiReg[regID.C]);
                } else {
                    continue;
                    *iExitStatus = UM_EXIT_STATUS_INV_PERMISSION;
                    break;
                }
            } else if   (uiOpcode == 0xA) {
                if (uiReg[regID.C] >= 0 || uiReg[regID.C] <= 255) {
                    if (iMode & UM_MODE_WRITE_OUTPUT_FILE) {
                        if (iMode & UM_MODE_PRINT_NUMERIC) {
                            printf("%u", uiReg[regID.C]);
                        } else {
                            printf("%c",(unsigned char) uiReg[regID.C]);
                        }
                    }
                } else {
                    *iExitStatus = UM_EXIT_STATUS_INV_CHR;
                    break;
                }
            } else if   (uiOpcode == 0xB) {
                printf("Input: ");

                scanf(" %c",&ucInputBuffer);
                uiReg[regID.C] = (unsigned int) ucInputBuffer;
            } else if   (uiOpcode == 0xC) {
                if (uiReg[regID.B] == 0) {
                    if (uiReg[regID.C] >= pArrays[0].len) {
                        *iExitStatus = UM_EXIT_STATUS_PP_OOB;
                        break;
                    } else {
                        ulPP = (unsigned long) uiReg[regID.C];
                    }
                } else {
                    if (pArrays[uiReg[regID.B]].isActive == UM_FALSE) {
                        *iExitStatus = UM_EXIT_STATUS_ARR_NOT_ACTIVE;
                        break;
                    } else if (uiReg[regID.C] >= pArrays[uiReg[regID.B]].len) {
                        *iExitStatus = UM_EXIT_STATUS_ARR_OOB;
                        break;
                    } else {
                        free(pArrays[0].arrayp); // release old array
                        pArrays[0].arrayp   = malloc(pArrays[uiReg[regID.B]].len*4);
                        pArrays[0].len      = pArrays[uiReg[regID.B]].len;
                        memcpy(pArrays[0].arrayp, pArrays[uiReg[regID.B]].arrayp, pArrays[uiReg[regID.B]].len*4);
                        ulPP                = (unsigned long) uiReg[regID.C];
                    }
                }
            }
        }
        if (iMode & UM_MODE_DEBUG) printf("    %u, %u, %u, %u, %u, %u, %u, %u\n",uiReg[0],uiReg[1],uiReg[2],uiReg[3],uiReg[4],uiReg[5],uiReg[6],uiReg[7]);
    }
    if (iMode & UM_MODE_OUTPUT_STATE) printf("\nDone, exit status: %d\n", *iExitStatus);

    if (iMode & UM_MODE_DUMP_STATE_BEFORE_EXIT) {
        printf("Dumping state at exit\n");
        printf("Program pointer: %lu, last command: %s\n", ulPP- 1, cstrpOpcodes[uiOpcode]);
        printf("Created and active arrays:\n");

        const char cstrActive[]     = "Active";
        const char cstrInactive[]   = "Inactive";
        const char* cstrPrint;
        for (unsigned int i = 0; i < uiCreatedArrays ; i++) {
            if (pArrays[i].isActive == UM_TRUE) {
                cstrPrint = cstrActive;
            } else {
                cstrPrint = cstrInactive;
            }

            printf("ID: %u, Len: %lu, State: %s\n", i, pArrays[i].len, cstrPrint);
        }
    }
    // Clear up memory ========================================================
    if (iMode & UM_MODE_OUTPUT_STATE) {
        printf("Cleaning up...");
    }

    for (unsigned int i = 0; i < uiNumOfArrays ; i++) {
        if (pArrays[i].isActive == UM_TRUE) {
            free(pArrays[i].arrayp);
        }
    }

    free(pArrays);

    if (iMode & UM_MODE_OUTPUT_STATE) {
        printf("Done!\n");
    }
    // ========================================================================
    return;
}


unsigned int UM_findInactive(
    const struct UM_Array  *pArrays,
    const unsigned int      uiCreatedArrays
) {
    for (unsigned int i = 1; i < uiCreatedArrays ; i++) {
        if (pArrays[i].isActive == UM_FALSE) return i;
    }

    return 0; // Will only be returned IF no unactive is found
};