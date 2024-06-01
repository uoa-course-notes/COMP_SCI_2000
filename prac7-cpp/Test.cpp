// #include "CompilerParser.h"
#include "ParseTree.h"
#include "Token.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector> 
#include <string>



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
    std::string fileName = "Main.jack";
    std::fstream myFile;

    myFile.open(fileName, std::ios::in);
    if (myFile){
        std::cout << "File exists in your current directory" << std::endl;
        // Read one character at a time 
        // char ch;
        // while(! myFile.eof()){
        //     myFile >> ch;

        //     std::cout << ch; 
        // }

        // Read one line at a time from the file object 
        std::string line;
        while(std::getline(myFile, line)){
            // Print the data from the string line-by-line
            std::cout << line << std::endl;
            
        }
    }else std::cout << "No such file." << std::endl;

    // std::cout << std::endl;
    return 0;
}