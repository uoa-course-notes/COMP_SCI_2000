#ifndef VMTRANSLATOR_H
#define VMTRANSLATOR_H


#include <bitset>     // number to binary sting
#include <cstdint>    // this contains uint16_t
#include <functional> // use hash
#include <string>   // 
#include <map>     // An unordered map 
#include <vector>
#include <regex>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>



// using namespace std;
extern std::string moduleName;
// Using extern to inform the compiler that moduleName is defined elsewhere. 

class VMTranslator {
    private:
        void write(std::string vmCode);
        std::string registerName(std::string segment, int index);

        int symbolCounter; 

        enum VMsegments{
            seg_constant,
            seg_static,
            seg_temp,
            seg_local,
            seg_pointer,
            seg_argument,
            seg_this,
            seg_that
        };

        std::map<std::string, VMsegments> map_segments;
        std::stringstream ss_ASM;
    public:
        VMTranslator();
        ~VMTranslator();

        /* Project 07 Subroutines*/
        // 2 memory acess commands.
        std::string vm_push(std::string segment, int offset);
        std::string vm_pop(std::string segment, int offset);

        // 9 ALU commands. 
         std::string vm_add();
         std::string vm_sub();
         std::string vm_neg();

         std::string vm_eq();
         std::string vm_gt();
         std::string vm_lt();
         std::string vm_and();
         std::string vm_or();
         std::string vm_not();

         std::string vm_label(std::string label);
         std::string vm_goto(std::string label);
         std::string vm_if(std::string label);

        // Sub-routine commands. 
        std::string vm_function(std::string function_name, int n_vars);
        std::string vm_call(std::string function_name, int n_args);
        std::string vm_return();
        
};

#endif /* VMTRANSLATOR_H */