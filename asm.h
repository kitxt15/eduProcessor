#include <cstdio>
#include <cstring>

#include "colors.h"
#include "errno.h"
#include "commands.h"
#include "text.h"
#define MAX_LENGTH 10 * 1000

struct Label {
    bool defined;
    int  address;
};

int   FindFlagPos(int argc, char *argv[]);
int   RemoveCommentsAndSkips(struct Line data[]);
int   WriteFile(const char *filename, struct Line data[], struct Label labels[]);
void *BuffDup(char buffer[]);

int FindFlagPos(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            return i;
        } else {
            continue;
        }
    }
    return -1;
}

int RemoveCommentsAndSkips(struct Line data[]) {

    for (int count = 0; data[count].buffer; count++) {
        char *line = data[count].buffer;
        char *com = strchr(line, ';');
        if (com != nullptr) {
            *com = '\0';
        }

        size_t len = strlen(line);
        while (len > 0 && line[len - 1] == ' ') {
            line[--len] = '\0';
        }
    }
    return 0;
}

static int ReadOpcode(const char* line) {
    char name[64];
    if (sscanf(line, "%63s", name) != 1) {
        return -1;
    }

    if (strcmp(name, "PUSH") == 0) return PUSH_OP;
    if (strcmp(name, "POP") == 0)  return POP_OP;
    if (strcmp(name, "OUT") == 0)  return OUT_OP;
    if (strcmp(name, "MEOW") == 0) return MEOW_OP;
    if (strcmp(name, "meow") == 0) return MEOW_OP;
    if (strcmp(name, "ADD") == 0)  return ADD_OP;
    if (strcmp(name, "SUB") == 0)  return SUB_OP;
    if (strcmp(name, "DIV") == 0)  return DIV_OP;
    if (strcmp(name, "MUL") == 0)  return MUL_OP;
    if (strcmp(name, "SQRT") == 0) return SQRT_OP;
    if (strcmp(name, "JMP") == 0)  return JMP_OP;
    if (strcmp(name, "JA") == 0)   return JA_OP;
    if (strcmp(name, "JB") == 0)   return JB_OP;
    if (strcmp(name, "JE") == 0)   return JE_OP;
    if (strcmp(name, "JMPA") == 0) return JA_OP;
    if (strcmp(name, "JMPE") == 0) return JE_OP;
    if (strcmp(name, "NOP") == 0)  return NOP_OP;
    if (strcmp(name, "CALL") == 0) return CALL_OP;
    if (strcmp(name, "RET") == 0)  return RET_OP;
    if (strcmp(name, "HLT") == 0)  return HLT_OP;

    return -1;
}

int WriteFile(const char *filename, struct Line data[], struct Label labels[]) {

    int address = 0;
    for (int count = 0; data[count].buffer; count++) {
        int id = 0;
        if (sscanf(data[count].buffer, " :%d", &id) == 1) {
            if (id < 0 || id >= MAX_LENGTH) {
                fprintf(stderr, "Неверный номер метки на строке %d: %d\n", count + 1, id);
                return -1;
            }
            labels[id].defined = true;
            labels[id].address = address;
        } else if (ReadOpcode(data[count].buffer) >= 0) {
            address++;
        }
    }

    FILE *fp = fopen(filename, "wb");

    if (fp == NULL) {
        printf(RED "ERROR: Не удалось открыть файл \"%s\"!\n" RESET, filename);
        return -errno;
    }

    for (int count = 0; data[count].buffer; count++) {
        const char* line = data[count].buffer;
        int command = ReadOpcode(line);
        if (command < 0) {
            continue;
        }

        int  arg     = 0;
        char reg     = '\0';
        char closing = '\0';
        int  address = 0;

        int memory_fields = sscanf(line, "%*s [%d %c", &address, &closing);
        int is_memory = memory_fields == 2 && closing == ']';

        char memory_reg  = '\0';
        char reg_closing = '\0';

        int memory_reg_fields = sscanf(line, "%*s [ %c %c", &memory_reg, &reg_closing);
        int is_memory_reg = memory_reg_fields == 2 && reg_closing == ']';

        Command result = {};
        result.command = command;

        if (is_memory) {
            result.arg = address;
            if (command == PUSH_OP) {
                result.command = PUSH_MEM_OP;
            } else if (command == POP_OP) {
                result.command = POP_MEM_OP;
            } else {
                fclose(fp);
                return -1;
            }
        } else if (is_memory_reg) {

            result.r = memory_reg;

            if (command == PUSH_OP) {

                result.command = PUSH_MEM_REG_OP;

            } else if (command == POP_OP) {

                result.command = POP_MEM_REG_OP;

            } else {
                fclose(fp);
                return -1;
            }
        } else if (sscanf(line, "%*s :%d", &arg) == 1) {
            if (arg < 0 || arg >= MAX_LENGTH || !labels[arg].defined) {
                fprintf(stderr, "Неизвестная метка на строке %d: :%d\n", count + 1, arg);
                fclose(fp);
                return -1;
            }
            result.arg = labels[arg].address;

        } else if (sscanf(line, "%*s %d", &arg) == 1) {
            result.arg = arg;

        } else if (command == JMP_OP || command == JA_OP || command == JB_OP ||
                   command == JE_OP || command == CALL_OP) {
            fprintf(stderr, "Ожидался адрес или метка на строке %d\n", count + 1);
            fclose(fp);
            return -1;

        } else if (sscanf(line, "%*s %c", &reg) == 1) {

            result.r = reg;

            if (command == PUSH_OP) {
                result.command = PUSHR_OP;
            } else if (command == POP_OP) {
                result.command = POPR_OP;
            }
        }

        if (fwrite(&result, sizeof(result), 1, fp) != 1) {
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);
    return 0;
}

void *BuffDup(char buffer[]) {

    size_t row_len = strlen(buffer);
    void  *ptr     = calloc(row_len + 1, sizeof(char));
    strcpy((char *)ptr, buffer);

    return ptr;
}
