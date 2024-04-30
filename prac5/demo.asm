// Add up two numbers 
// RAM[2] = RAM[0] + RAM[1]
// Usage: put the values that you wish to add in RAM[0] and RAM[1]


@0 // A = 0 ==> M=RAM[0] 
D=M


@1 // A = 1 ==> M=RAM[1]
D = D + M // D = RAM[0] + RAM[1]


@2 // M = RAM[2]
M = D // M = RAM[2] = RAM[1] + RAM[0]


// It's of best practice if one adds an infinite loop or
// unconditional jumps at the end of each .asm program to avoid 
// inadvertently executing malicious code. 
// @6 // this line is needed because 
0;JMP // Unconditional jump. 
