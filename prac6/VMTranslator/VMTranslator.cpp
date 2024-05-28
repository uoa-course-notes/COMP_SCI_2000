#include "VMTranslator.h"

// using namespace std;

/**
 * VMTranslator constructor
 */

std::string moduleName;
// This provides the actual definition of moduleName, resolving the Linker Error that I've been having. 

VMTranslator::VMTranslator() {
    // Your code here
    ss_ASM.str(std::string());
    moduleName = "myModule";
    symbolCounter = 0;
    map_segments.clear();
    map_segments.insert(std::pair<std::string, VMsegments>("constant", seg_constant));
    map_segments.insert(std::pair<std::string, VMsegments>("static", seg_static));
    map_segments.insert(std::pair<std::string, VMsegments>("temp", seg_temp));
    map_segments.insert(std::pair<std::string, VMsegments>("pointer", seg_pointer));
    map_segments.insert(std::pair<std::string, VMsegments>("local", seg_local));
    map_segments.insert(std::pair<std::string, VMsegments>("argument", seg_argument));
    map_segments.insert(std::pair<std::string, VMsegments>("this", seg_this));
    map_segments.insert(std::pair<std::string, VMsegments>("that", seg_that));
}

/**
 * VMTranslator destructor
 */
VMTranslator::~VMTranslator() {
    // Your code here
    map_segments.clear();
}

/** Generate Hack Assembly code for a VM push operation */
std::string VMTranslator::vm_push(std::string segment, int offset){
    ss_ASM.str(std::string());
    std::string indexStr = std::to_string(offset); // convert the offset (int) into string to be write to the command
    std::string registerStr = registerName(segment, offset);
    switch (map_segments[segment])
    {
    case seg_constant:
        write("@" + indexStr + " // push " + segment + " " + indexStr);
        write("D=A");
        write("@SP");
        write("A=M");
        write("M=D");
        write("@SP");
        write("M=M+1");
        break;
    case seg_static:
    case seg_temp:
    case seg_pointer:
        write("@" + registerStr + " // push " + segment + " " + indexStr);
        write("D=A");
        write("@" + indexStr);
        write("A=D+A");
        write("D=M");
        write("@SP");
        write("A=M");
        write("M=D");
        write("@SP");
        write("M=M+1");
        break;
    case seg_argument:
    case seg_local:
    case seg_this:
    case seg_that:
        write("@" + registerStr + " // push " + segment + " " + indexStr);
        write("D=M");
        write("@" + indexStr);
        write("A=D+A");
        write("D=M");
        write("@SP");
        write("A=M");
        write("M=D");
        write("@SP");
        write("M=M+1");
        break;
    default:
        throw std::runtime_error("vm_push(): Invalid segment");
    }
    return ss_ASM.str() + "\n";    
}

/** Generate Hack Assembly code for a VM pop operation */
std::string VMTranslator::vm_pop(std::string segment, int offset){    
    ss_ASM.str(std::string());
    std::string indexStr = std::to_string(offset);
    std::string registerStr = registerName(segment, offset);
    switch (map_segments[segment])
    {
    case seg_constant:
        throw std::runtime_error("vm_pop(): cannot pop to constant");
        break;
    case seg_static:
        write("@" + registerStr + " // pop " + segment + " " + indexStr);
        write("D=A");
        write("@" + indexStr);
        write("D=D+A");
        write("@R13");
        write("M=D");
        write("@SP");
        write("AM=M-1");
        write("D=M");
        write("@R13");
        write("A=M");
        write("M=D");
        break;
    case seg_argument:
    case seg_local:
    case seg_this:
    case seg_that:
    case seg_temp:
    case seg_pointer:
        write("@" + registerStr + " // pop " + segment + " " + indexStr);
        write("D=M");
        write("@" + indexStr);
        write("D=D+A");
        write("@R13");
        write("M=D");
        write("@SP");
        write("AM=M-1");
        write("D=M");
        write("@R13");
        write("A=M");
        write("M=D");
        break;
    default:
        throw std::runtime_error("vm_pop(): Invalid segment");
    }
    return ss_ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM add operation */
std::string VMTranslator::vm_add(){
    ss_ASM.str(std::string());
    write("@SP // add");
    write("AM=M-1");
    write("D=M");
    write("A=A-1");
    write("M=D+M");
    return ss_ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM sub operation */
std::string VMTranslator::vm_sub(){
    ss_ASM.str(std::string());
    write("@SP // sub");
    write("AM=M-1");
    write("D=M");
    write("A=A-1");
    write("M=M-D");
    return ss_ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM neg operation */
std::string VMTranslator::vm_neg(){
    ss_ASM.str(std::string());
    write("@SP // neg");
    write("A=M");
    write("A=A-1");
    write("M=-M");
    return ss_ASM.str() + "\n";}

/** Generate Hack Assembly code for a VM eq operation */
std::string VMTranslator::vm_eq(){
    ss_ASM.str(std::string());
    std::string labelTrue("JGT_TRUE_" + moduleName + "_" + std::to_string(symbolCounter));
    std::string labelFalse("JGT_FALSE_" + moduleName + "_" + std::to_string(symbolCounter));

    write("@SP // eq");
    write("AM=M-1");
    write("D=M");
    write("@SP");
    write("AM=M-1");
    write("D=M-D");
    write("@" + labelTrue);
    write("D;JEQ");
    write("D=0");
    write("@" + labelFalse);
    write("0;JMP");
    write("(" + labelTrue + ")");
    write("D=-1");
    write("(" + labelFalse + ")");
    write("@SP");
    write("A=M");
    write("M=D");
    write("@SP");
    write("M=M+1");

    symbolCounter++;
    return ss_ASM.str() + "\n";}

/** Generate Hack Assembly code for a VM gt operation */
std::string VMTranslator::vm_gt(){
    ss_ASM.str(std::string());
    std::string labelTrue("JGT_TRUE_" + moduleName + "_" + std::to_string(symbolCounter));
    std::string labelFalse("JGT_FALSE_" + moduleName + "_" + std::to_string(symbolCounter));

    write("@SP // gt");
    write("AM=M-1");
    write("D=M");
    write("@SP");
    write("AM=M-1");
    write("D=M-D");
    write("@" + labelTrue);
    write("D;JGT");
    write("D=0");
    write("@" + labelFalse);
    write("0;JMP");
    write("(" + labelTrue + ")");
    write("D=-1");
    write("(" + labelFalse + ")");
    write("@SP");
    write("A=M");
    write("M=D");
    write("@SP");
    write("M=M+1");

    symbolCounter++;
    return ss_ASM.str() + "\n";}

/** Generate Hack Assembly code for a VM lt operation */
std::string VMTranslator::vm_lt(){
    ss_ASM.str(std::string());
    std::string labelTrue("JLT_TRUE_" + moduleName + "_" + std::to_string(symbolCounter));
    std::string labelFalse("JLT_FALSE_" + moduleName + "_" + std::to_string(symbolCounter));

    write("@SP // lt");
    write("AM=M-1");
    write("D=M");
    write("@SP");
    write("AM=M-1");
    write("D=M-D");
    write("@" + labelTrue);
    write("D;JLT");
    write("D=0");
    write("@" + labelFalse);
    write("0;JMP");
    write("(" + labelTrue + ")");
    write("D=-1");
    write("(" + labelFalse + ")");
    write("@SP");
    write("A=M");
    write("M=D");
    write("@SP");
    write("M=M+1");

    symbolCounter++;
    return ss_ASM.str() + "\n";}

/** Generate Hack Assembly code for a VM and operation */
std::string VMTranslator::vm_and(){
    ss_ASM.str(std::string());
    write("@SP // and");
    write("AM=M-1");
    write("D=M");
    write("A=A-1");
    write("M=D&M");
    return ss_ASM.str() + "\n";

}

/** Generate Hack Assembly code for a VM or operation */
std::string VMTranslator::vm_or(){
    ss_ASM.str(std::string());
    write("@SP // or");
    write("AM=M-1");
    write("D=M");
    write("A=A-1");
    write("M=D|M");
    return ss_ASM.str() + "\n";}

/** Generate Hack Assembly code for a VM not operation */
std::string VMTranslator::vm_not(){
    ss_ASM.str(std::string());
    write("@SP // not");
    write("A=M");
    write("A=A-1");
    write("M=!M");
    return ss_ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM label operation */
std::string VMTranslator::vm_label(std::string label){
    ss_ASM.str(std::string());
    write("(" + label + ") // label " + label);
    return ss_ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM goto operation */
std::string VMTranslator::vm_goto(std::string label){
    ss_ASM.str(std::string());
    write("@" + label + "// goto " + label);
    write("0;JMP");
    return ss_ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM if-goto operation */
std::string VMTranslator::vm_if(std::string label){
    ss_ASM.str(std::string());
    write("@SP // if-goto " + label);
    write("AM=M-1");
    write("D=M");
    write("@" + label);
    write("D;JNE");
    return ss_ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM function operation */
std::string VMTranslator::vm_function(std::string function_name, int n_vars){
    ss_ASM.str(std::string());
    write("(" + function_name + ") // function " + function_name + " " + std::to_string(n_vars));
    for (int n = n_vars; n > 0; n--)
    {
        write("@SP");
        write("AM=M+1");
        write("A=A-1");
        write("M=0");
    }
    return ss_ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM call operation */
std::string VMTranslator::vm_call(std::string function_name, int n_args){
    ss_ASM.str(std::string());
    write("@return_address // call " + function_name + " " + std::to_string(n_args));
    write("D=A");
    write("@SP");
    write("AM=M+1");
    write("A=A-1");
    write("M=D");

    write("@LCL");
    write("D=M");
    write("@SP");
    write("AM=M+1");
    write("A=A-1");
    write("M=D");

    write("@ARG");
    write("D=M");
    write("@SP");
    write("AM=M+1");
    write("A=A-1");
    write("M=D");

    write("@THIS");
    write("D=M");
    write("@SP");
    write("AM=M+1");
    write("A=A-1");
    write("M=D");

    write("@THAT");
    write("D=M");
    write("@SP");
    write("AM=M+1");
    write("A=A-1");
    write("M=D");

    write("@SP");
    write("D=M");
    write("@5");
    write("D=D-A");
    write("@" + std::to_string(n_args));
    write("D=D-A");
    write("@ARG");
    write("M=D");

    write("@SP");
    write("D=M");
    write("@LCL");
    write("M=D");

    write("@funcName");
    write("0;JMP");

    write("(return_address)");
    return ss_ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM return operation */
std::string VMTranslator::vm_return(){
   ss_ASM.str(std::string());
    write("@LCL // return");
    write("D=M");
    write("@R13");
    write("M=D");

    write("@R13");
    write("D=M");
    write("@5");
    write("A=D-A");
    write("D=M");
    write("@R14");
    write("M=D");

    write("@SP");
    write("AM=M-1");
    write("D=M");
    write("@ARG");
    write("A=M");
    write("M=D");

    write("@ARG");
    write("D=M+1");
    write("@SP");
    write("M=D");

    write("@R13");
    write("D=M");
    write("@1");
    write("A=D-A");
    write("D=M");
    write("@THAT");
    write("M=D");

    write("@R13");
    write("D=M");
    write("@2");
    write("A=D-A");
    write("D=M");
    write("@THIS");
    write("M=D");

    write("@R13");
    write("D=M");
    write("@3");
    write("A=D-A");
    write("D=M");
    write("@ARG");
    write("M=D");

    write("@R13");
    write("D=M");
    write("@4");
    write("A=D-A");
    write("D=M");
    write("@LCL");
    write("M=D");

    write("@R14");
    write("A=M");
    write("0;JMP");

    return ss_ASM.str() + "\n";
}

void VMTranslator::write(std::string vmCode)
{
    if (vmCode.find("(") == std::string::npos)
        ss_ASM << "\t"; // put TAB if the ASM command is not a label
    ss_ASM << vmCode << std::endl;
}

std::string VMTranslator::registerName(std::string segment, int index)
{
    if (segment == "static")
        return "16";
    if (segment == "local")
        return "LCL";
    if (segment == "argument")
        return "ARG";
    if (segment == "this")
        return "THIS";
    if (segment == "that")
        return "THAT";
    if (segment == "pointer")
        return "R" + std::to_string(3 + index);
    if (segment == "temp")
        return "R" + std::to_string(5 + index);

    return moduleName + "." + std::to_string(index); // else it is static
}