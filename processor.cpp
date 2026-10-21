#include <stdio.h>
#include <stdlib.h>
#include "processor.h"
// ./proccessor example_bytecode.txt
// делает команды оттуда

void init();


int main(int argc, char* argv[]){

    struct Processor cpu = {
       .st_data = {},
       .st_return = {},
       .regs = {0,0,0},
       .VerifyProcessor = VerifyProcessor,
    };

    struct Text asm_text = {
        .index_arr = {},
        .text_buffer = {}
    };

    struct TypeInfo IntInfo = {};
    IntInfoInit(&IntInfo);

    ProcessorConstructor(&cpu, IntInfo, IntInfo);

    Command commands_arr[MAX_LENGTH] = {};

    const char* input_file = argc > 1 ? argv[1] : "example_bytecode.txt";

    if (ReadFile(input_file, asm_text.index_arr, &asm_text.text_buffer) != 0){
        return -1;
    }
    size_t len = CountLenOfLines(asm_text.index_arr);

    ParseCommands(asm_text.index_arr, commands_arr);


    printf("Commands:\n");
    for (int i = 0; i < len; i++){
        printf("%d\n", commands_arr[i].command);
    }

    printf("Args:\n");
    for (int i = 0; i < len; i++){
        printf("%d\n", commands_arr[i].arg);
    }

    printf( GREEN "Starting processor...\n" RESET);
    StartProcessor(&cpu, commands_arr, len);


    ProcessorDestructor(&cpu);

    return 0;
}
