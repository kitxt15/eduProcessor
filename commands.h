enum intcodes {
    PUSH = 'HSUP',
    POP  = '\0POP',
    POP2 = ' POP',
    ADD  = '\0DDA',
    SUB  = '\0BUS',
    DIV  = '\0VID',
    MUL  = '\0LUM',
    OUT  = '\0TUO',
    NOP  = '\0PON',
    CALL = 'LLAC',
    RET  = '\0TER',
    MEOW = 'WEOM',
    JMP  = ' PMJ',
    JMPA = 'APMJ',
    JMPE = 'EPMJ',
    HLT  = '\0TLH',

};

enum opcodes {
    PUSH_OP  = 1,
    POP_OP   = 2,
    PUSHR_OP = 65,
    POPR_OP  = 66,
    ADD_OP   = 3,
    SUB_OP   = 5,
    DIV_OP   = 6,
    MUL_OP   = 7,
    OUT_OP   = 4,
    MEOW_OP  = 8,
    SQRT_OP  = 9,
    NOP_OP   = 15,
    CALL_OP  = 100,
    RET_OP   = 101,

    JMP_OP = 17,
    JA_OP  = 18,
    JE_OP  = 19,
    JB_OP  = 20,

    PUSH_MEM_OP = 129,
    POP_MEM_OP  = 130,

    PUSH_MEM_REG_OP = 193,
    POP_MEM_REG_OP  = 194,

    HLT_OP = 0
};

enum regs { a = 0, b = 1, c = 2 };

struct {
    int  command;
    int  arg;
    char r;
} typedef Command;
