// Sample Test file for AddSub.asm
// Follows the Test Scripting Language format described in 
// Appendix B of the book "The Elements of Computing Systems"
/*
1st non-default test cases:
Input:
R[1] = 12
R[2] = -4
R[3] = -5

R[0] = 13

R[1] is positive. R[2] and R[3] are negative. R[0] is evaluated accordingly. 


output-list: instructs the simulator what to write to the output file in every subsequent output command in this script.
Each item in the output list has the syntax variable format padL.len.padR. This instructs the simulator to write padL spaces, then
the current varianle value in the specified format using len commands, then padR spaces, then the divider symbol "|".
Format can either be %B 

*/
load AddSub.asm,
output-file AddSub00.out,
compare-to AddSub00.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2 RAM[3]%D2.6.2;

set PC 0,
set RAM[0] 0,  // Set R0
set RAM[1] 3,  // Set R1
set RAM[2] 2,  // Set R2
set RAM[3] 1;  // Set R3
repeat 100 {
  ticktock;    // Run for 100 clock cycles
}
set RAM[1] 3,  // Restore arguments in case program used them
set RAM[2] 2,
set RAM[3] 1,
output;        // Output to file

