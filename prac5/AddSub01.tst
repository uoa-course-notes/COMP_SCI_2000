// Sample Test file for AddSub.asm
// Follows the Test Scripting Language format described in 
// Appendix B of the book "The Elements of Computing Systems"




load AddSub.asm,
output-file AddSub01.out,
compare-to AddSub01.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2 RAM[3]%D2.6.2;


// Setting test argument. 
// Purpose: Test positive + negative - negative values 

set PC 0,
set RAM[0]  0,  // Set R0
set RAM[1]  2,  // Set R1
set RAM[2] -4,  // Set R2
set RAM[3] -5;  // Set R3
repeat 100 {
  ticktock;    // Run for 100 clock cycles
}
// When restoring arguments, be sure to enter the same values.
set RAM[1]  2,  // Restore arguments in case program used them
set RAM[2] -4,
set RAM[3] -5,
output;        // Output to file

