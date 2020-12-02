#include "um_parser.h"
#include "um_dissasembler.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	const char *cstrFileName 	= "assets/sandmark.umz";
	const char *cstrOutputFile	= "out/unformatedOutput.txt";
	const char *cstrDissasembly	= "out/dissOutput.txt";
	struct UM_ParseStatus st	= parseFromFile(cstrFileName);
	if (st.status == UM_PARSED_STATUS_FNF) {
		printf("File not found: %s\n", cstrFileName);
	} else if (st.status == UM_PARSED_STATUS_UE) {
		printf("An unknown error occured!\n");
	} else {
		printf("File Parsed!\n");
		UM_writeToFile(st.parsedProgram, cstrOutputFile);
		FILE* fDiss = fopen(cstrDissasembly,"w");
		writeDissasemblyToFile(st.parsedProgram.program,st.parsedProgram.length,fDiss,NULL);
		free((void *)st.parsedProgram.program);
	}
	
	return 0;
}
