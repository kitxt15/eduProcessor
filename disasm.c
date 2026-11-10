#include <stdio.h>

#include "commands.h"

int main(int argc, char *argv[]) {

    const char *filename = argc > 1 ? argv[1] : "fibonacci.bin";
    FILE       *fp       = fopen(filename, "rb");
    if (fp == NULL) {
        perror(filename);
        return 1;
    }

    FILE *out = fopen("disasm.txt", "w");
    if (out == NULL) {
        perror("disasm.txt");
        fclose(fp);
        return 1;
    }

    Command cmd   = {0};
    size_t  bytes = 0;
    while ((bytes = fread(&cmd, 1, sizeof(cmd), fp)) == sizeof(cmd)) {
        switch (cmd.command) {
        case PUSH_OP:         fprintf(out, "PUSH %d\n",      cmd.arg);   break;
        case POP_OP:          fprintf(out, "POP\n");                     break;
        case PUSHR_OP:        fprintf(out, "PUSH %c\n",      cmd.r);     break;
        case POPR_OP:         fprintf(out, "POP %c\n",       cmd.r);     break;
        case PUSH_MEM_OP:     fprintf(out, "PUSH [%d]\n",    cmd.arg);   break;
        case POP_MEM_OP:      fprintf(out, "POP [%d]\n",     cmd.arg);   break;
        case PUSH_MEM_REG_OP: fprintf(out, "PUSH [%c]\n",    cmd.r);     break;
        case POP_MEM_REG_OP:  fprintf(out, "POP [%c]\n",     cmd.r);     break;
        case ADD_OP:          fprintf(out, "ADD\n");                     break;
        case SUB_OP:          fprintf(out, "SUB\n");                     break;
        case MUL_OP:          fprintf(out, "MUL\n");                     break;
        case DIV_OP:          fprintf(out, "DIV\n");                     break;
        case OUT_OP:          fprintf(out, "OUT\n");                     break;
        case MEOW_OP:         fprintf(out, "MEOW\n");                    break;
        case SQRT_OP:         fprintf(out, "SQRT\n");                    break;
        case NOP_OP:          fprintf(out, "NOP\n");                     break;
        case JMP_OP:          fprintf(out, "JMP %d\n",       cmd.arg);   break;
        case JA_OP:           fprintf(out, "JA %d\n",        cmd.arg);   break;
        case JE_OP:           fprintf(out, "JE %d\n",        cmd.arg);   break;
        case JB_OP:           fprintf(out, "JB %d\n",        cmd.arg);   break;
        case CALL_OP:         fprintf(out, "CALL %d\n",      cmd.arg);   break;
        case RET_OP:          fprintf(out, "RET\n");                     break;
        case HLT_OP:          fprintf(out, "HLT\n");                     break;
        default:
                              fprintf(stderr, "неизвестный код %d\n", cmd.command);
                              fclose(out);
                              fclose(fp);
                              return 1;
        }
    }

    int failed = ferror(fp) || bytes != 0;
    if (failed) {
        fprintf(stderr, "failed reading in %s\n", filename);
    }
    if (ferror(out)) failed = 1;
    if (fclose(out) != 0) failed = 1;
    fclose(fp);
    return failed;
}
