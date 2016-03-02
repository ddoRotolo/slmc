#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INST_AMT 10

#define BUFF_LEN 16 

typedef struct s_cpu {
    int            *mm;
    int             ac;
    int             pc;
} CPU;

typedef void (*INST_F) (CPU*, int);

void ADD (CPU *c, int arg) {
    c->ac += c->mm[arg];
}

void SUB (CPU *c, int arg) {
    c->ac -= c->mm[arg];
}

void STA (CPU *c, int arg) {
    c->mm[arg] = c->ac;
}

void LDA (CPU *c, int arg) {
    c->ac = c->mm[arg];
}

void BRA (CPU *c, int arg) {
}

void BRZ (CPU *c, int arg) {
}

void BRP (CPU *c, int arg) {
}

void INP (CPU *c, int IGN) {
    c->ac = getchar();
}

void OUT (CPU *c, int IGN) {
    putchar(c->ac);
}

void HLT (CPU *c, int IGN) {
    exit(0);
}

char *inst_str[INST_AMT] = {
    "ADD",
    "SUB",
    "STA",
    "LDA",
    "BRA",
    "BRZ",
    "BRP",
    "INP",
    "OUT",
    "HLT"
};

INST_F inst_func[INST_AMT] = {
    ADD,
    SUB,
    STA,
    LDA,
    BRA,
    BRZ,
    BRP,
    INP,
    OUT,
    HLT
};

void interpret (CPU *c, char *s) {
    char    comm[8];
    int     arg = 0;
    int     i   = 0;

    sscanf(s, "%s %d", comm, &arg);
    fprintf(stderr, "COMM: %s ARG: %d\n", comm, arg);
    while (strncmp(inst_str[i], comm, 3)) {
        i++;
        if (i >= INST_AMT) return;
    }
    (inst_func[i]) (c, arg);
}

void init_cpu(CPU *c, int mem_amt) {
    c->mm = malloc(sizeof(int[mem_amt]));
    c->ac = 0;
    c->pc = 0;
}


int main (int argc, char *argv[]) {
    char line[BUFF_LEN];
    CPU main_cpu;

    init_cpu(&main_cpu, 100);

    while (fgets(line, BUFF_LEN, stdin) != NULL) {
        interpret(&main_cpu, line);
    }
}
