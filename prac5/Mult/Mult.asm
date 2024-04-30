// This file is based on part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: Mult.asm

// Multiplies R1 and R2 and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.


    @R1
    D = M		// load R1 into D

    @i			// set the backwards counter to R1
    M = D

    @product		// product = 0 before program begins
    M = 0

(LOOP)			// while i > 0, keep looping
    @i
    D = M

    @END		// otherwise, goto END
    D;JEQ

    @R2
    D = M

    @product		// add R2 to 'product'
    M = D + M

    @i			// reduce i
    M = M - 1	

    @LOOP		// restart the loop
    0;JMP

(END)
    @product		// load 'product' to D
    D = M
  
    @R0			// store result in R0
    M = D

    @END
    0;JMP 		// infinite loop to prevent hacking





