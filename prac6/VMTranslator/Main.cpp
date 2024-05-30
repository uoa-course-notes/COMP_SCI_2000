#include "VMTranslator.h"

// using namespace std;


/*
argv[argc] = {___, file.vm}
*/
// std::string moduleName;



/** A quick-and-dirty parser */ 
int main(int argc, char** argv){
    if(argc > 1){
        std::fstream file;
        file.open(argv[1],std::ios::in); // in reading mode 
        if (file.is_open()){   // Is our file open? 
            VMTranslator vmt; // Create the vmt translator object. 
            std::string line; // Read line by line from the file 
            while(getline(file, line)){
                std::regex endl_re("\\r*\\n+"); 
                std::regex space_re("\\s+");
                line = std::regex_replace(line, endl_re, "");
                line = std::regex_replace(line, space_re, " ");
                std::vector <std::string> tokens;
                std::stringstream ss(line);
                std::string token;
                // Tokenize
                while(getline(ss, token, ' ')){
                    tokens.push_back(token);
                }
                if(tokens.size()==1){
                    if(tokens[0]=="add")
                       std::cout << vmt.vm_add() << std::endl;
                    else if(tokens[0]=="sub")
                      std::cout << vmt.vm_sub() << std::endl;
                    else if(tokens[0]=="neg")
                      std::cout << vmt.vm_neg() << std::endl;
                    else if(tokens[0]=="eq")
                      std::cout << vmt.vm_eq() << std::endl;
                    else if(tokens[0]=="gt")
                      std::cout << vmt.vm_gt() << std::endl;
                    else if(tokens[0]=="lt")
                      std::cout << vmt.vm_lt() << std::endl;
                    else if(tokens[0]=="and")
                      std::cout << vmt.vm_and() << std::endl;
                    else if(tokens[0]=="or")
                      std::cout << vmt.vm_or() << std::endl;
                    else if(tokens[0]=="not")
                      std::cout << vmt.vm_not() << std::endl;
                    else if(tokens[0]=="return")
                       std::cout << vmt.vm_return() << std::endl;  
                } 
                
                else if(tokens.size()==2){
                    if(tokens[0]=="label")
                        std::cout << vmt.vm_label(tokens[1]) << std::endl;
                    else if(tokens[0]=="goto")
                        std::cout << vmt.vm_goto(tokens[1]) << std::endl;
                    else if(tokens[0]=="if-goto")
                        std::cout << vmt.vm_if(tokens[1]) << std::endl;
                } 
                
                else if(tokens.size()==3){
                    int t2;
                    try {
                        t2 = stoi(tokens[2]);
                    } catch (const std::invalid_argument& ia) {
                        std::cerr << "Unable to parse int." << tokens[2] << std::endl;
                        break;
                    }
                    if(tokens[0]=="push")
                        std::cout << vmt.vm_push(tokens[1],t2) << std::endl;
                    else if(tokens[0]=="pop")
                        std::cout << vmt.vm_pop(tokens[1],t2) << std::endl;
                    else if(tokens[0]=="function")
                        std::cout << vmt.vm_function(tokens[1],t2) << std::endl;
                    else if(tokens[0]=="call")
                        std::cout << vmt.vm_call(tokens[1],t2) << std::endl;
                }
            }
            file.close();
        }

    }

    else{ // argc < = 1
      throw std::runtime_error("main(): please specify a VM file. *.vm[*.asm]");
    }
} 
