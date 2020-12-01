#include "um_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include "um_dissasembler.h"

int main() {
	const char *cstrFileName = "assets/codex.umz";
	struct UMParseOutput umpoParsedProgram = parseUMZFile(cstrFileName);
	if (umpoParsedProgram.status == UM_PARSE_STATUS_OK) {
		// printProgramArr(umpoParsedProgram);
		FILE* output = fopen("./out/diss.txt", "w");
		unsigned int uiBw = 0;
		writeDissasemblyToFile(
			umpoParsedProgram.program,
			umpoParsedProgram.programLen,
			output,
			&uiBw
		);
		fclose(output);
		free(umpoParsedProgram.program);
	} else {
		printf("Failed to parse file.\n");
	}
	
	return 0;
}
