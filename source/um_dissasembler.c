#include "um_dissasembler.h"
#include <stdio.h>

void writeDissasemblyToFile(unsigned int* uipUMProgram,
                unsigned long ulProgramLen,
                FILE* fpOutputStream,
                unsigned int* uiBytesWritten) {

    *uiBytesWritten = 0;

    if (!fpOutputStream || ulProgramLen == 0) {
        return;
    }

    const char* cstrpOpcodes[16] = {
        "ifmv\0",   //conditional move
        "read\0",   // read array
        "put \0",   // write array
        "add \0",   // addition
        "mul \0",   // multiplication
        "div \0",   // divide
        "nand\0",   // not-and
        "halt\0",   // halt program
        "allc\0",   // allocate array
        "clra\0",   // clear array
        "chro\0",   // character output
        "chri\0",   // character input
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
    
    for (unsigned long ulPP = 0; ulPP < ulProgramLen; ulPP++) {
        unsigned int uiInstruction = uipUMProgram[ulPP];
        unsigned int uiOpcode = 0xF & (uiInstruction >> 28);
        const char* cstrOpcode = cstrpOpcodes[uiOpcode];
        
        // Find numerical length of pp
        unsigned int uiNrHexDigits = 1;
        unsigned long uiExp = 16;
        while (ulPP / uiExp != 0) {
            uiExp *= 16;
            uiNrHexDigits++;
        }
        char cstrLeadingZeroes[9] = "00000000\0";
        cstrLeadingZeroes[8 - uiNrHexDigits] = '\0';

        char cstrBuffer[45];
        if (uiOpcode == 0x7) {
            sprintf(cstrBuffer,"%s%lX    %s\n",cstrLeadingZeroes, ulPP,cstrOpcode);
        } else if (uiOpcode == 0xD) {
            unsigned int uiImmediate = 0x1ffffff & uiInstruction;
            const char* uiAreg = cstrpRegisters[0x7 & (uiInstruction >> 25)];
            sprintf(cstrBuffer,"%s%lX    %s    %s, %d\n",cstrLeadingZeroes, ulPP,cstrOpcode,uiAreg, uiImmediate);
        } else {
            if (uiOpcode > 0xD || 0xffffe00 & uiInstruction) {
                sprintf(cstrBuffer,"%s%lX    NOP\n",cstrLeadingZeroes, ulPP);
            } else {
                const char *uiRegC = cstrpRegisters[0x7 & uiInstruction];
                const char *uiRegB = cstrpRegisters[0x7 & (uiInstruction >> 3)];
                const char *uiRegA = cstrpRegisters[0x7 & (uiInstruction >> 6)];
                if (uiOpcode == 0x8 || uiOpcode == 0xC) {
                    sprintf(cstrBuffer,"%s%lX    %s    %s, %s\n",
                            cstrLeadingZeroes, 
                            ulPP,
                            cstrOpcode, 
                            uiRegB, 
                            uiRegC
                    );
                } else if (uiOpcode == 0x9 || uiOpcode == 0xA || uiOpcode == 0xB) {
                    sprintf(cstrBuffer,"%s%lX    %s    %s\n", 
                            cstrLeadingZeroes, 
                            ulPP,
                            cstrOpcode, 
                            uiRegC
                    );
                } else {
                    sprintf(cstrBuffer,"%s%lX    %s    %s, %s, %s\n",
                            cstrLeadingZeroes, 
                            ulPP,
                            cstrOpcode, 
                            uiRegA, 
                            uiRegB, 
                            uiRegC
                    );
                }
            }
        }

        // See length of message
        unsigned int uiLength = 0;
        while (cstrBuffer[uiLength] != '\0') {uiLength++;};

        // write to file
        fwrite(cstrBuffer, sizeof cstrBuffer[0], uiLength, fpOutputStream);
        *uiBytesWritten += uiLength;
    }
}