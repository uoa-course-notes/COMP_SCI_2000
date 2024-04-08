// Add up two numbers 
// RAM[2] = RAM[0] + RAM[1]
// Usage: put the values that you wish to add in RAM[0] and RAM[1]


@0 
D=M // A=0, M = RAM[0]
 

@1 
D=D+M  // A=1, M = RAM[1]

@2
M=D  // A=2, M=RAM[2] 


// Best practice 
// To terminate the program safely,end it with an infinite loop
@6 
o;JMP

