#include "um_parser.h"
#include "um_dissasembler.h"
#include "um.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	const char *cstrFileName 	= "assets/codex.umz";
	struct UM_ParseStatus st	= parseFromFile(cstrFileName);
	if (st.status == UM_PARSED_STATUS_FNF) {
		printf("File not found: %s\n", cstrFileName);
	} else if (st.status == UM_PARSED_STATUS_UE) {
		printf("An unknown error occured!\n");
	} else {
		printf("File Parsed!\n");
		UM_Main(st.parsedProgram.program,st.parsedProgram.length,NULL, UM_MODE_WRITE_OUTPUT_FILE | UM_MODE_DUMP_STATE_BEFORE_EXIT| UM_MODE_OUTPUT_STATE);
	}
	
	return 0;
}
