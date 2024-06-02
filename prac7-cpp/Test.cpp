// #include "CompilerParser.h"
#include "ParseTree.h"
#include "Token.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector> 
#include <string>


std::string ch1 = "\u2502", ch2 = "\u2514";
std::string special[2] = {ch1, ch2};

std::vector<Token*> tokenizer(std::string fileName){
    std::vector<Token*> tokenList;
    std::fstream myFile;
    myFile.open(fileName, std::ios::in);
    if (myFile.is_open()){
        std::cout << "File exists in the current directory." << std::endl;
        char ch;
        // This is basically the tokenizing logic 
        
        while(! myFile.eof()){
            myFile >> ch;
            // std::cout << ch; 
        }
    }

    return tokenList;

}





int main(int argc, char** argv){
    // std::string fileName = "Main.jack";
    // std::fstream myFile;

    // myFile.open(fileName, std::ios::in);
    // if (myFile){
    //     std::cout << "File exists in your current directory" << std::endl;
    //     // Read one character at a time 
    //     // char ch;
    //     // while(! myFile.eof()){
    //     //     myFile >> ch;

    //     //     std::cout << ch; 
    //     // }

    //     // Read one line at a time from the file object 
    //     std::string line;
    //     while(std::getline(myFile, line)){
    //         // Print the data from the string line-by-line
    //         std::cout << line << std::endl;
            
    //     }
    // }else std::cout << "No such file." << std::endl;

    // std::cout << "1st char: " << special[0] << "\n\n";
    // std::cout << "2nd char: " << special[1] << std::endl;
    
    // Simulate similar files 
    /*
    class Main{

    }
    
    */
    // std::cout << "For tokens: class Main{ }" << std::endl;
    // std::cout << "Got:" << std::endl;
    // std::cout << "class\n";
    // // std::cout << special[0] << "\t";
    // std::cout << special[1] << "keyword: class" << std::endl;
    // std::cout << special[1] << "identifier: main" << std::endl;
    // std::cout << special[1] << "symbol: {" << std::endl;
    // std::cout << special[1] << "symbol: }" << std::endl;

    // std::cout << std::endl;
    Token t1("keyword", "class");
    ParseTree tree(t1.getType(), t1.getValue());
    
    std::list<Token*> tokens;


    
    return 0;
}