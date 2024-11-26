#include "header.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    const char *inputFilePath = argv[1];
    const char *completeCodePath = "Complete_Code.as";

    Kdam_Assembler(inputFilePath);
    twoPasses();

    return 0;
}


