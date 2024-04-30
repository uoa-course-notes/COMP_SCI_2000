// Sample Test file for AddSub.asm
// Follows the Test Scripting Language format described in 
// Appendix B of the book "The Elements of Computing Systems"




load AddSub.asm,
output-file AddSub03.out,
compare-to AddSub03.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2 RAM[3]%D2.6.2;


// Setting test argument. 
// Purpose: Test all negative inputs  

set PC 0,
set RAM[0]  0,  // Set R0
set RAM[1]  -21,  // Set R1
set RAM[2]  -34,  // Set R2
set RAM[3]  -29;  // Set R3
repeat 100 {
  ticktock;    // Run for 100 clock cycles
}
// When restoring arguments, be sure to enter the same values.
set RAM[1]  -21,  // Restore arguments in case program used them
set RAM[2]  -34,
set RAM[3]  -29,
output;        // Output to file

