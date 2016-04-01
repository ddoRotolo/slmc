#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg.h"

#define INST_AMT 11

char *mnemonics[INST_AMT] =
{
    "HLT",
    "ADD",
    "SUB",
    "STA",
    "LDA",
    "BRA",
    "BRZ",
    "BRP",
    "INP",
    "OUT",
    "NOP",
};

int mnemonics_to_inst (char *mnem) {
    int i;
    for (i = 0; i < INST_AMT; i++) {
        if (!strncmp(mnemonics[i], mnem, 3)) {
            return i;
        }
    }
    return -1;
}

int main (int argc, char *argv[]) {
    return 0;
}
