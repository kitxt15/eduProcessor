enum intcodes {
    PUSH = 'HSUP',
    POP = '\0POP',
    POP2 = ' POP',
    ADD = '\0DDA',
    SUB = '\0BUS',
    DIV = ' VID',
    MUL = ' LUM',
    OUT = '\0TUO',
    NOP = '\0PON',
    CALL = 'LLAC',
    RET = '\0TER',

    JMP = ' PMJ',
    JMPA = 'APMJ',
    JMPE = 'EPMJ',

    HLT = '\nTLH',

};

enum opcodes {
    PUSH_OP = 1,
    POP_OP = 2,
    PUSHR_OP = 65,
    POPR_OP = 66,
    ADD_OP = 3,
    SUB_OP = 5,
    DIV_OP = 6,
    MUL_OP = 7,
    OUT_OP = 4,
    NOP_OP = 15,
    CALL_OP = 100,
    RET_OP = 101,

    JMP_OP = 17,
    JMPA_OP = 18,
    JMPE_OP = 19,

    HLT_OP = 0
};

enum regs{
    a = 0,
    b = 1,
    c = 2
};


struct {
    int command;
    int arg;
    char r;
} typedef Command;
