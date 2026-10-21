#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "colors.h"

#include "asm.h"
// assembler text to byte code + registers + metkas
// вызов: ./asm example.txt -o example_bytecode.txt
//


int main(int argc, char* argv[]){

    struct Line index_arr[MAX_LENGTH] = {};
    struct Label labels[MAX_LENGTH] = {};

    int opt = 0;
    char flag[3] = "-o";
    char* text_buffer = {};

    const char* input_file =  "example.txt";
    const char* output_file = "example_bytecode.bin";

    if (argv[1]){
        input_file = argc >= 1 && strcmp(argv[1], flag)!= 0 ? argv[1] :  "example.txt";
        output_file = "example_bytecode.txt";
        int output_pos = FindFlagPos(argc, argv);
        if (output_pos != -1){
            output_file = argv[output_pos + 1];
        }
    }

    printf("IN file is: <%s> \n", input_file);
    printf("OUT file is: <%s> \n", output_file);


    if (ReadFile(input_file, index_arr, &text_buffer) != 0){
        return -1;
    }


    size_t len = CountLenOfLines(index_arr);

    if (len > 1){
        RemoveCommentsAndSkips(index_arr);
    }

    // FillArgPtr(index_arr, len);
    WriteFile(output_file, index_arr, labels);

    return 0;
}
