#include "colors.h"
#include "int_type.h"
#include "commands.h"
#include "text.h"
#include <cstddef>
#include <cstdio>

#define MAX_LENGTH 10 * 1000
#define RAM_SIZE   4096

// DONE: load func for some programs together done
// DONE: init doinit proc funcs
// DONE: print proc
// DONE: print addr + commands
// DONE: verify proc func
// DONE: квадратка
// DONE: разные джампы
// DONE: meow
// DONE: SKIP BLANK LINES
// DONE: кружок *б*чий


// NOTE: для проверки подлинности договора см. дату создания файла 4

typedef struct Processor {
    Stack_t st_data;
    Stack_t st_return;
    int     regs[3];
    int     ram[RAM_SIZE];
    bool (*VerifyProcessor)(Processor *cpu);
} Processor;

int  ProcessorConstructor(Processor *cpu);
void ProcessorDestructor(Processor *cpu);

bool VerifyProcessor(Processor *cpu);

void ParseCommands(struct Line data[], Command commands[]);
int  StartProcessor(Processor *cpu, Command commands[], size_t nC);

void PrintState(Processor *cpu, Command commands[], size_t idx);

void Out(const Stack_t *st);
void Add(Stack_t *st);
void Sub(Stack_t *st);
void Mul(Stack_t *st);
void Div(Stack_t *st);

static bool JumpCondition(Stack_t *st, int opcode);

int ProcessorConstructor(Processor *cpu, struct TypeInfo type_data, struct TypeInfo type_ret) {

    for (int i = 0; i < RAM_SIZE; i++) {
        cpu->ram[i] = 0;
    }
    StackConstructor(&cpu->st_data, type_data, MAX_LENGTH);
    PrintStack(&cpu->st_data, 1);
    StackConstructor(&cpu->st_return, type_ret, MAX_LENGTH);

    cpu->regs[0] = 0;
    cpu->regs[1] = 0;
    cpu->regs[2] = 0;
    cpu->VerifyProcessor(cpu);

    return 0;
}

void ProcessorDestructor(Processor *cpu) {
    cpu->VerifyProcessor(cpu);

    StackDeconstructor(&cpu->st_data);
    StackDeconstructor(&cpu->st_return);
    return;
}

bool VerifyProcessor(Processor *cpu) {
    if (StackVerify(&cpu->st_data) == 0 && StackVerify(&cpu->st_return) == 0) {
        return true;
    }
    return false;
}

void ParseCommands(struct Line data[], Command commands[]) {
    int  count          = 0;
    int  char_count     = 0;
    bool isArg          = false;
    int  command        = 0;
    int  scanned        = 0;
    int  scanned_second = 0;
    int  arg            = 0;
    char reg            = 0;

    while (data[count].buffer) {

        scanned        = sscanf(data[count].buffer, "%d %d", &command, &arg);
        scanned_second = 0;
        if (scanned == 1) {
            arg            = -1;
            scanned_second = sscanf(data[count].buffer, "%d %c", &command, &reg);
            printf("chat arg is: %c\n", reg);
        } else if (scanned != 2) {
            printf("im in break");
            break;
        }

        commands[count].command = command;
        commands[count].r       = reg;

        commands[count].command = commands[count].r != 0 ? command + 64 : command;
        commands[count].arg     = arg;
        reg                     = '\0';

        count++;
    }
}

void PrintState(Processor *cpu, Command *commands, int idx){
    printf(YELLOW "--------------------------------------------------------\n" RESET);
    printf("Выполняю команду: %d\n", idx);
    printf("Команды | prev: [%03d] curr: [%03d] next: [%03d]\n", idx - 1 >= 0 ? commands[idx-1].command : -1 , commands[idx].command, idx + 1 < cpu->st_data.capacity ? commands[idx+1].command : -1 );
    printf("arg: [%d]\n", commands[idx].arg);
    printf("regs: a:[%d] b:[%d] c:[%d]\n", cpu->regs[0], cpu->regs[1], cpu->regs[2]);
    printf(CYAN "Стек команд (5 ячеек)\n" RESET);
    PrintStack(&cpu->st_data, 5);
    // printf(CYAN "Стек возврата (5 ячеек)\n" RESET);
    // PrintStack(&cpu->st_return, 5);
    printf(YELLOW "--------------------------------------------------------\n" RESET);
    return;
}

int StartProcessor(Processor *cpu, Command commands[], size_t nC) {
    Stack_t *st = &cpu->st_data;

    int  command = 0;
    int  arg     = 0;
    int  r       = 0;
    int  temp    = 0;
    bool jump    = false;

    for (int i = 0; i < nC; i++) {

        command = commands[i].command;
        arg     = commands[i].arg;
        r       = commands[i].r;
        PrintState(cpu, commands, i);
        switch (commands[i].command) {
        case NOP_OP:
            printf(RED "[NOP_OP]\n" RESET);
            continue;
            break;
        case PUSH_OP:
            printf(RED "[PUSH_OP]\n" RESET);
            StackPush(st, (void *)&arg);
            printf("Pushing %d...\n", arg); // debug
            continue;
            break;
        case PUSHR_OP:
            printf(RED "[PUSHR_OP] + %c\n" RESET, r);
            switch (r) {
            case 'a':
                StackPush(st, (void *)&cpu->regs[0]);
                continue;
                break;
            case 'b':
                StackPush(st, (void *)&cpu->regs[1]);
                continue;
                break;
            case 'c':
                StackPush(st, (void *)&cpu->regs[2]);
                continue;
                break;
            }
            continue;
            break;
        case POP_OP:
            printf(RED "[POP_OP]\n" RESET);
            StackPop(st, NULL);
            // printf("Popping...\n");
            continue;
            break;
        case POPR_OP:
            printf(RED "[POPR_OP] + %c\n" RESET, r);
            // printf("NOW REGS[0] IS ???\n");
            switch (r) {
            case 'a':
                // printf("NOW REGS[0] IS %d\n", cpu->regs[0]);
                StackPop(st, (void *)&cpu->regs[0]);
                continue;
                break;
            case 'b':
                // printf("NOW REGS[1] IS %d\n", cpu->regs[1]);
                StackPop(st, (void *)&cpu->regs[1]);
                continue;
                break;
            case 'c':
                // printf("NOW REGS[2] IS %d\n", cpu->regs[2]);
                StackPop(st, (void *)&cpu->regs[2]);
                continue;
                break;
            }
        case ADD_OP:
            printf(RED "[ADD_OP]\n" RESET);
            Add(st);
            // printf("Adding...\n");
            continue;
            break;
        case MUL_OP:
            printf(RED "[MUL_OP]\n" RESET);
            Mul(st);
            continue;
            break;
        case DIV_OP:
            printf(RED "[DIV_OP]\n" RESET);
            Div(st);
            continue;
            break;
        case SUB_OP:
            printf(RED "[SUB_OP]\n" RESET);
            Sub(st);
            // printf("Adding...\n");
            continue;
            break;
        case OUT_OP:
            Out(st);
            continue;
            break;
        case MEOW_OP:
            printf(RED "meow-meow-meow!\n" RESET);
            getchar();
            continue;
        case JMP_OP:
        case JA_OP:
        case JB_OP:
        case JE_OP:
            if (command != JMP_OP && !JumpCondition(st, command)) {
                continue;
            }
            if (arg < 0 || (size_t)arg >= nC) {
                fprintf(stderr, "Неверный адрес перехода: %d\n", arg);
                return -1;
            }
            i = arg - 1;
            continue;
        case CALL_OP:
            printf(RED "[CALL_OP] + %d\n" RESET, arg);
            temp = i;
            StackPush(&cpu->st_return, &temp);
            PrintStack(&cpu->st_return, 2);

            if (arg >= 0) {
                i = arg - 1;
            }

            temp = -1;
            continue;
            break;
        case RET_OP:
            StackPop(&cpu->st_return, &temp);
            i = temp;
            printf(RED "[RET_OP] + %d\n" RESET, i);
            continue;
            break;
        case HLT_OP: printf("HLT\n"); return 0;

        case PUSH_MEM_OP:
            if (arg < 0 || arg >= RAM_SIZE) {
                fprintf(stderr, "Неверный адрес в озу: %d\n", arg);
                return -1;
            }

            StackPush(st, &cpu->ram[arg]);
            continue;

        case POP_MEM_OP:
            if (arg < 0 || arg >= RAM_SIZE) {
                fprintf(stderr, "Неверный адрес в озу: %d\n", arg);
                return -1;
            }

            if (st->size == 0) {
                fprintf(stderr, "Стек пустой =)\n");
                return -1;
            }

            StackPop(st, &cpu->ram[arg]);
            continue;
        case PUSH_MEM_REG_OP:
        case POP_MEM_REG_OP:  {
            int reg_index = r - 'a'; // v asm

            int address = cpu->regs[reg_index];

            if (address < 0 || address >= RAM_SIZE) {
                fprintf(stderr, "Неверный адрес в озу: %d\n", address);
                return -1;
            }

            if (command == PUSH_MEM_REG_OP) {
                StackPush(st, &cpu->ram[address]);
            } else {
                if (st->size == 0) {
                    fprintf(stderr, "Стек пустой =)\n");
                    return -1;
                }

                StackPop(st, &cpu->ram[address]);
            }

            continue;
        }
        }
    }
    return -1;
}

void Out(const Stack_t *st) {
    printf("Итоговое значение: ");
    // st->type.PrintElementOfType((char*)st->data+st->type.size*st->size);
    if (st->size >= 1) {
        st->type.PrintElementOfType(st->data);
    } else {
        printf("0");
    }
    getchar();

    printf("\n");
}

void Add(Stack_t *st) {
    int x   = 0;
    int y   = 0;
    int sum = 0;

    if (st->size <= 1) {
        return;
    }

    StackPop(st, &y);
    StackPop(st, &x);

    sum = x + y;

    StackPush(st, &sum);
}
void Sub(Stack_t *st) {
    int x   = 0;
    int y   = 0;
    int sub = 0;

    if (st->size <= 1) {
        return;
    }

    StackPop(st, &y);
    StackPop(st, &x);

    sub = x - y;

    StackPush(st, &sub);
}
void Mul(Stack_t *st) {
    int x   = 0;
    int y   = 0;
    int mul = 0;

    StackPop(st, &y);
    StackPop(st, &x);

    mul = x * y;

    StackPush(st, &mul);
}
void Div(Stack_t *st) {
    int x   = 0;
    int y   = 0;
    int div = 0;

    StackPop(st, &y);
    StackPop(st, &x);

    div = x / y;

    StackPush(st, &div);
}

static bool JumpCondition(Stack_t *st, int opcode) {

    int x = 0;
    int y = 0;

    if (st->size <= 1) {
        return false;
    }

    StackPop(st, &y);
    StackPop(st, &x);

    StackPush(st, &x);
    StackPush(st, &y);

    switch (opcode) {
    case JA_OP: return x > y;
    case JB_OP: return x < y;
    case JE_OP: return x == y;
    default:    return false;
    }
}
