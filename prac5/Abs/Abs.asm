// Calculates the absolute value of R1 and stores the result in R0.
// (R0, R1 refer to RAM[0], and RAM[1], respectively.)

// Put your code here.


// A symbol or macro for 0 once translated by the assembler 
@R0 //  M = RAM[0]


// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/lecture/Abs.asm

// Stores the absolute value of R0 in R1

// Put your code here.
    @R1
    D=M
    @NONNEGATIVE
    D; JGE
(NEGATIVE)
    @R1
    D=-M
    @R0
    M=D
    @END
    0; JMP
(NONNEGATIVE)
    @R1
    D=M
    @R0
    M=D
(END)
    @END
    0; JMP
















