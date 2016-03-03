#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INST_AMT 11
#define BUFF_LEN 32
#define CUR_MM(c) (c->mm[c->pc])

typedef struct s_mbox MAILBOX;

struct s_mbox {
    int op;
    int data;
};

typedef struct s_cpu {
    MAILBOX    *mm;
    int         mem_amt;
    int         pc;
    int         ac;
} CPU;

typedef void (*INST_F) (CPU*);

void HLT (CPU *c) {
    exit(CUR_MM(c).data);
}

void ADD (CPU *c) {
    c->ac += CUR_MM(c).data;
}

void SUB (CPU *c) {
    c->ac -= CUR_MM(c).data;
}

void STA (CPU *c) {
    c->mm[CUR_MM(c).data].data = c->ac;
}

void LDA (CPU *c) {
    c->ac = c->mm[CUR_MM(c).data].data;
}

void BRA (CPU *c) {
    c->pc = CUR_MM(c).data - 1;
}

void BRZ (CPU *c) {
    if (c->ac == 0) {
        c->pc = CUR_MM(c).data - 1;
    }
}

void BRP (CPU *c) {
    if (c->ac >= 0) {
        c->pc = CUR_MM(c).data - 1;
    }
}

void INP (CPU *c) {
    c->ac = getchar();
}

void OUT (CPU *c) {
    putchar((char) c->ac);
}

void NOP (CPU *c) {
}

INST_F inst_func[INST_AMT] = {
    HLT, //0
    ADD, //1
    SUB, //2
    STA, //3
    LDA, //4
    BRA, //5
    BRZ, //6
    BRP, //7
    INP, //8
    OUT, //9
    NOP  //10
};

void exec_inst(CPU *c) {
    inst_func[CUR_MM(c).op](c);
    c->pc++;
}

void init_cpu(CPU *c, int mem_amt) {
    c->mm = malloc(sizeof(MAILBOX[mem_amt]));
    c->mem_amt = mem_amt;
    c->pc = 0;
    c->ac = 0;
}

void load_program(CPU *c) {
    char line[BUFF_LEN];
    int i = 0;
    while (fgets(line, BUFF_LEN, stdin) != NULL) {
        if (!strncmp("END", line, 3)) { break; }
        c->mm[i].data = 0;
        sscanf(line, "%d %d", &c->mm[i].op, &c->mm[i].data);
        i++; 
        if (i > c->mem_amt) break;
    }
}

void print_cpu(CPU *c) {
    int i;
    fprintf(stderr, "AC: %d PC: %d\n", c->ac, c->pc);
    fprintf(stderr, "OP: %d DT: %d\n", CUR_MM(c).op, CUR_MM(c).data);
    fprintf(stderr, "Memory:\n");
    for (i = 0; i < c->mem_amt; i++) {
        fprintf(stderr, " %d %d |", c->mm[i].op, c->mm[i].data);
        if (!(i % 10) && i != 0) { fprintf(stderr, "\n"); }
    }
    fprintf(stderr, "\n");
}

int main (int argc, char *argv[]) {
    CPU main_cpu;
    int debug = 0;
    init_cpu(&main_cpu, 10);
    load_program(&main_cpu);
    for (;;) { 
        if (debug) { print_cpu(&main_cpu); }
        exec_inst(&main_cpu); 
    }
}
