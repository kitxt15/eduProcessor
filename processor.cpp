#include <stdio.h>
#include <stdlib.h>
#include "processor.h"
#include "video.h"

// ./proccessor example_bytecode.txt
// делает команды оттуда

void init_processor(Processor* cpu);
int load_commands(const char* filename, Command commands[],
                        size_t capacity, size_t *len);
void print_commands(Command* commands_arr, size_t* len);

int main(int argc, char *argv[]) {

    struct Processor cpu = {
        .st_data         = {},
        .st_return       = {},
        .regs            = {0, 0, 0},
        .VerifyProcessor = VerifyProcessor,
    };

    struct Text asm_text = {.index_arr = {}, .text_buffer = {}};


    init_processor(&cpu);
    const char *input_file = argc > 1 ? argv[1] : "example_bytecode.bin";

    Command commands_arr[MAX_LENGTH] = {};
    size_t len = 0;

    if (load_commands(input_file , commands_arr, MAX_LENGTH, &len) != 0 ) {
        ProcessorDestructor(&cpu);
        return -1;
    }

    print_commands(commands_arr, &len);
    int result = StartProcessor(&cpu, commands_arr, len);

    ProcessorDestructor(&cpu);

    render(cpu.ram + 3072);

    return result == 0 ? 0 : 1;
}

void init_processor(Processor* cpu){
    cpu->VerifyProcessor = VerifyProcessor;

    TypeInfo intInfo = {};
    IntInfoInit(&intInfo);

    ProcessorConstructor(cpu, intInfo, intInfo);
}

int load_commands(const char* filename, Command commands[],
                        size_t capacity, size_t* len){
    FILE *fp = fopen(filename, "rb");
        if (fp == nullptr) {
            return -1;
        }
        *len = fread(commands, sizeof(commands[0]), capacity, fp);
        bool failed = ferror(fp);
        fclose(fp);

        return failed ? -1 : 0;
}

void print_commands(Command* commands_arr, size_t* len){
    printf(GREEN "Starting processor...\n" RESET);
    for (size_t i = 0; i < *len; i++) {
        printf("[%zu] opcode=%d arg=%d reg=%d\n", i, commands_arr[i].command, commands_arr[i].arg,
               (int)commands_arr[i].r);
    }
}
