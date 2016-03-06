/*
   DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE 
   Version 2, December 2004 

   Copyright (C) 2004 Sam Hocevar <sam@hocevar.net> 

   Everyone is permitted to copy and distribute verbatim or modified 
   copies of this license document, and changing it is allowed as long 
   as the name is changed. 

   DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE 
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION 

   0. You just DO WHAT THE FUCK YOU WANT TO.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg.h"

#define INST_AMT 11
#define BUFF_LEN 32
#define CUR_MM(c) (c->mm[c->pc])

typedef struct s_mbox MAILBOX;

struct s_mbox {
    unsigned int    op;
    int             data;
};

typedef struct s_cpu {
    MAILBOX    *mm;
    int         mem_amt;
    int         pc;
    int         ac;
} CPU;

/* 
   There used to be a stupid, clever way of adding instructions with 
   an array of function pointers, but it's gone now 
*/

typedef enum e_inst {
    HLT = 0,
    ADD,
    SUB,
    STA,
    LDA,
    BRA,
    BRZ,
    BRP,
    INP,
    OUT,
    NOP
} INST_T;

void exec_inst(CPU *c) {
    switch (CUR_MM(c).op) {
        case HLT: exit(CUR_MM(c).data);                         break;
        case ADD: c->ac += CUR_MM(c).data;                      break;
        case SUB: c->ac -= CUR_MM(c).data;                      break;
        case STA: c->mm[CUR_MM(c).data].data = c->ac;           break;
        case LDA: c->ac = c->mm[CUR_MM(c).data].data;           break;
        case BRA: c->pc = CUR_MM(c).data - 1;                   break;
        case BRZ: if (c->ac == 0) c->pc = CUR_MM(c).data - 1;   break;
        case BRP: if (c->ac >= 0) c->pc = CUR_MM(c).data - 1;   break;
        case INP: c->ac = getchar();                            break;
        case OUT: putchar((char) c->ac);                        break;
        case NOP:                                               break;
        default: exit(1); //Given an OPCODE that doesn't exist
    }
    c->pc++;
}

void init_cpu(CPU *c, int mem_amt) {
    c->mm = malloc(sizeof(MAILBOX[mem_amt]));
    c->mem_amt = mem_amt;
    c->pc = 0;
    c->ac = 0;
}

void load_program(CPU *c, FILE *in) {
    char line[BUFF_LEN];
    int i = 0;
    while (fgets(line, BUFF_LEN, in) != NULL) {
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
    int mem = 100;
    FILE *f = stdin;
    char *argv0;

    ARGBEGIN {
        case 'd': debug = 1;                    break;
        case 'm': mem   = atoi(ARGF());         break;
        case 'f': f     = fopen(ARGF(), "r");   break;
        default: break;
    } ARGEND

    init_cpu(&main_cpu, mem);
    load_program(&main_cpu, f);
    if (f != stdin) fclose(f);

    for (;;) { 
        if (debug) { print_cpu(&main_cpu); }
        exec_inst(&main_cpu); 
    }

    return 0;
}
