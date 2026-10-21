#include <cstdio>
#include <cstring>

#include "colors.h"
#include "errno.h"
#include "commands.h"
#include "text.h"
#define MAX_LENGTH  10*1000

struct Label {
    int id;
    int address;
};

int FindFlagPos(int argc, char* argv[]);
int RemoveCommentsAndSkips(struct Line data[]);
int WriteFile(const char* filename, struct Line data[], struct Label labels[]);
void* BuffDup(char buffer[]);

int FindFlagPos(int argc, char* argv[]){
    for (int i = 0; i < argc; i++){
        if (strcmp(argv[i], "-o") == 0){
            return i;
        }
        else {
            continue;
        }
    }
    return -1;
}

int RemoveCommentsAndSkips(struct Line data[]){

    int count = 0;
    int count_char = 0;
    size_t len = 0;

    char* com = nullptr;
    char* null = nullptr;
    while (data[count].buffer != nullptr){

          com = strchr(data[count].buffer, ';');
          if ( com != nullptr){
              *com = '\0';
          }

          null = strchr(data[count].buffer, '\0');
          printf("OUTSIDE WHILE\n");
          printf("*(null - count_char) = %c\n", *(null - count_char));
          printf("strlen(data[count].buffer) - count_char = %zu\n", strlen(data[count].buffer) - count_char);
          while (*(null - count_char - 1) == ' ' && strlen(data[count].buffer) - count_char > 0 ){
              printf("INSIDE WHILE\n");
              printf("*(null - count_char) = %c\n", *(null - count_char));
              printf("strlen(data[count].buffer) - count_char = %zu\n", strlen(data[count].buffer) - count_char);
              *(null - count_char -1) = '\0';
              count_char++;
          }

          count++;
          count_char = 0;
      }
      return 0;
}


int WriteFile(const char* filename, struct Line data[], struct Label labels[]) {

    FILE* fp = fopen(filename, "w");

    if (fp == NULL){ printf(RED "ERROR: Не удалось открыть файл \"%s\"!\n" RESET, filename); return -errno; }

    int count = 0;
    int char_count = 0;
    int command_len = 0;
    int check_buffer = 0;


    int nEL_const=0;
    int nEL_reg=0;
    int nEL_jmp=0;

    bool isArg = false;

    char command = 0;
    int arg = 0;
    char reg = 0;


    void* temp_ptr = calloc(64, 1);

    while (data[count].buffer){
        if (data[count].buffer[0] == ':'){
            if (sscanf((char *)data[count].buffer + 1, "%d", &arg) == 1){
                    labels[arg].id = arg;
                    labels[arg].address = count;
                    count++;
                    continue;
            }
            else{count++;continue;}
        }
        count++;
    }

    count = 0;
    while ( data[count].buffer ) {



        nEL_jmp = sscanf(data[count].buffer, "%s %c%d", (char *)temp_ptr, &reg, &arg);
        printf("njmp: <%d>\n", nEL_jmp);

        if (nEL_jmp != 3 || reg != ':'){
            nEL_const = sscanf(data[count].buffer, "%s %d", (char *)temp_ptr, &arg);
            nEL_reg = sscanf(data[count].buffer, "%s %c", (char *)temp_ptr, &reg);
        }

        printf("data[count].buffer is <%s>\n", data[count].buffer);
        memcpy(&check_buffer, data[count].buffer, 4);
        printf("another important info: %d and %d\n", check_buffer, ADD);

        switch (check_buffer) {
            case PUSH:
                command = PUSH_OP;
                break;
            case POP:
                command = POP_OP;
            case POP2:
                command = POP_OP;
                break;
            case OUT:
                command = OUT_OP;
                break;
            case SUB:
                command = SUB_OP;
                break;
            case ADD:
                command = ADD_OP;
                break;
            case DIV:
                command = DIV_OP;
                break;
            case MUL:
                command = MUL_OP;
                break;
            case JMP:
                command = JMP_OP;
                break;
            case JMPA:
                command = JMPA_OP;
                break;

            case NOP:
                command = NOP_OP;
                break;

            case CALL:
                command = CALL_OP;
                break;

            case RET:
                command = RET_OP;
                break;


            case HLT:
                command = HLT_OP;
                break;
        }

        fprintf(fp, "%d", command);

        if(nEL_const == 2){
            fprintf(fp, " %d", arg);
        }
        else if(nEL_reg == 2 && nEL_jmp != 3){
            fprintf(fp, " %c", reg);
        }
        else if (nEL_jmp == 3){
            if (labels[arg].address){
                fprintf(fp, " %d", labels[arg].address+1);
            }
        }
        fputs("\n", fp);

        command = NOP_OP;

        count++;

        //end
    }

    // second iteration for labels -> future;

    fputs("\0", fp);

    fclose(fp);
    return 0;
}


void* BuffDup(char buffer[]){

    size_t row_len = strlen(buffer);
    void* ptr = calloc(row_len+1, sizeof(char));
    strcpy((char*)ptr, buffer);

    return ptr;
}
