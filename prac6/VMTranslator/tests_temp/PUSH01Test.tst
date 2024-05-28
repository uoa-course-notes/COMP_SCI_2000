// Testing PUSH01Test.asm file 
// Pushing 3 negative numbers and adding them 
// local[0] = -2 + -3 + -4 = -9 

load PUSH01Test.asm // file to test 
output-file PUSH01Test.out, //output file name 
compare-to PUSH01Test.cmp , // expected output file name 
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2 RAM[3]%D2.6.2; // pretty printing into columns in the .out file 


// Set up the pre-amble or system states 
set PC 0,
set RAM[0] 0 , // Set R0
set RAM[1] 3 , // set R1
set RAM[2] 2 , // set R2
set RAM[3] 1; // set  R3 

repear 100 {
    ticktock; // ruck for 100 clock cycles 
}


output; //Output to file (print one line of output)








