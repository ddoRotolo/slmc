#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg.h"

#define BUFF_LEN 1024
#define CUR_MM(c)   (c->mm[c->pc])
#define CUR_INST(c) (CUR_MM(c) % 100)
#define CUR_DATA(c) (CUR_MM(c) / 100)

typedef struct s_cpu {
    int        *mm;
    int         mem_amt;
    int         pc;
    int         ac;
} CPU;

void init_cpu(CPU *c, int mem_amt) {
    c->mm       = calloc(mem_amt, sizeof(int));
    c->mem_amt  = mem_amt;
    c->pc       = 0;
    c->ac       = 0;
}

void print_cpu(CPU *c) {
    int i;
    fprintf(stderr, "AC: %d PC: %d\n", c->ac, c->pc);
    fprintf(stderr, "MM: %d\n", CUR_MM(c));
    fprintf(stderr, "Memory:\n");
    for (i = 0; i < c->mem_amt; i++) {
        fprintf(stderr, " %d %d |", c->mm[i]);
        if (!(i % 10) && i != 0) fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
}

void load_program(CPU *c, FILE *in) {
    char line[BUFF_LEN];
    int i = 0;
    while (fgets(line, BUFF_LEN, in) != NULL) {
        if (line[0] == '#') continue; //Comments
        if (!strncmp("END", line, 3) || (i > c->mem_amt)) break; 
        sscanf(line, "%d", &c->mm[i]);
        i++; 
    }
}

void exec_inst(CPU *c) {
    switch (CUR_INST(c)) {
        case 0 : exit(CUR_DATA(c));                             break; //HLT
        case 1 : c->ac += CUR_DATA(c);                          break; //ADD
        case 2 : c->ac -= CUR_DATA(c);                          break; //SUB
        case 3 : c->mm[CUR_DATA(c)] = c->ac;                    break; //STA
        case 4 : c->ac = c->mm[CUR_DATA(c)];                    break; //LDA
        case 5 : c->pc = (CUR_DATA(c)) - 1;                     break; //BRA
        case 6 : if (c->ac == 0) c->pc = (CUR_DATA(c)) - 1;     break; //BRZ
        case 7 : if (c->ac >= 0) c->pc = (CUR_DATA(c)) - 1;     break; //BRP
        case 8 : c->ac = getchar();                             break; //INP
        case 9 : putchar((char) c->ac);                         break; //OUT
        case 10:                                                break; //NOP
        default: exit(-1); //Given an OPCODE that doesn't exist
    }
    c->pc++;
}

int main (int argc, char *argv[]) {
    CPU main_cpu;
    FILE *f     = stdin;
    char *argv0;
    int debug   = 0;
    int mem     = 100;

    ARGBEGIN {
        case 'd': debug = 1;                    break;
        case 'm': mem   = atoi(ARGF());         break;
        case 'f': f     = fopen(ARGF(), "r");   break;
        default : break;
    } ARGEND

    init_cpu(&main_cpu, mem);
    load_program(&main_cpu, f);
    if (f != stdin) fclose(f);

    for (;;) { 
        if (debug) print_cpu(&main_cpu);
        exec_inst(&main_cpu); 
    }
}
