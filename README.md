#SLMC

A simple interpreter for the Little man Computer. 
It reads in a series of opcodes, by default from STDIN, and then runs the program. 

##Usage

Compile it with 'make lmc', then run it with './lmc'.
You'll then be able to type in a program. 
When you're done, type 'END', and it'll run.
Easy as that!

##Flags

    '-d'        Activates debugging, where the program memory is dumped to stderr after every instruction.
    '-m <mem>'  Changes the amount of memory available for the lmc. It starts with 100 Memory Cells by default
    '-f <file>' Reads a program from a given file.

##Opcodes

    0   :   HLT [exit code]      halt the program, return with exit code
    1   :   ADD [loc]            add  value at location to register value
    2   :   SUB [loc]            subtract value at location from register value
    3   :   STA [loc]            store register value at memory location
    4   :   LDA [loc]            load value from memory to register
    5   :   BRA [loc]            set pc to value
    6   :   BRZ [loc]            set pc to value if register is 0
    7   :   BRP [loc]            set pc to value if positive
    8   :   INP                  read input as byte to register
    9   :   OUT                  print register value as ascii
    10  :   NOP                  do nothing

##Example programs

###Echo

    8
    9
    5 

##Further modifications

Add an assembler for macros, labels, and mnemonic instructions.
