#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define DEBUG 3
#include "../eduStack/stack.h"


Stack_t st = {};


void PrintInt(const void* ptr);
int VerifyInt(const void* ptr, const struct TypeInfo);

// void Push(const Stack_t* st);
// void Pop(const Stack_t* st, const int* ptr);
void Out(const Stack_t* st);
void Add(Stack_t* st);
void Sub(Stack_t* st);
void Mul(Stack_t* st);
void Div(Stack_t* st);

int DoCommand(char* command, int arg);



int main(){

    //stack initialization
    struct TypeInfo IntInfo = {};
    IntInfo.size = sizeof(int);
    IntInfo.PrintElementOfType = PrintInt;
    IntInfo.VerifyType = VerifyInt;
    IntInfo.poizonFillByte = 0xDE;

    StackConstructor(&st, IntInfo, 100);

    char command[20] = {};

    int arg = 0;
    size_t count_of_input_vars = scanf("%s", command);

    //TODO PROCESS STRUCT

    while(strcmp(command, "HLT") != 0){

        arg = 0;

        if (strncmp(command, "PUSH", 4) == 0){
            if (scanf("%d", &arg) != 1){
                printf("error arg is not a number");
                scanf("%s", command);
                continue;
            }
        }

        DoCommand(command, arg);
        printf("%s %d\n", command, arg);
        scanf("%s", command);
    }


    StackDeconstructor(&st);
}



void PrintInt(const void* ptr){
    printf("%d", *(int*)ptr);
}

int VerifyInt(const void* ptr, const struct TypeInfo type){
    for (int i = 0; i < type.size; i++){
        if ((*(char*)ptr + i) != type.poizonFillByte ){
            return -1;
        }
    }
    return 0;
}


void Out(const Stack_t* st){
    printf("Итоговое значение: ");
    st->type.PrintElementOfType(st->data);
    printf("\n");
}

void Add(Stack_t* st){
    int x = 0;
    int y = 0;
    int sum = 0;

    StackPop(st, &y);
    StackPop(st, &x);

    sum = x + y;

    StackPush(st, &sum);
}
void Sub(Stack_t* st){
    int x = 0;
    int y = 0;
    int sub = 0;

    StackPop(st, &y);
    StackPop(st, &x);

    sub = x - y;

    StackPush(st, &sub);
}
void Mul(Stack_t* st){
    int x = 0;
    int y = 0;
    int mul = 0;

    StackPop(st, &y);
    StackPop(st, &x);

    mul = x * y;

    StackPush(st, &mul);
}
void Div(Stack_t* st){
    int x = 0;
    int y = 0;
    int div = 0;

    StackPop(st, &y);
    StackPop(st, &x);

    div = x / y;

    StackPush(st, &div);
}

int DoCommand(char* command, int arg){

    printf("DO COMMAND " BLUE "%s" RESET " WITH ARGS " CYAN "%d\n" RESET, command, arg);

    int temp = 0;
    memcpy(&temp, command, 4);

    printf("DEBUG: %d == %d\n", OUT, temp);

    switch (temp) {
        case PUSH:
            printf("case push\n");
            StackPush(&st, &arg);
            break;
        case POP:

        case OUT:
            printf("case out\n");
            Out(&st);
            break;
        case SUB:
            Sub(&st);
            break;
        case ADD:
            Add(&st);
            break;
        case DIV:
            Div(&st);
            break;
        case MUL:
            Mul(&st);
            break;


    }

    return 0;
}
