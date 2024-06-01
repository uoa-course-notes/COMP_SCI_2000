#include "CompilerParser.h"
#include <iostream>
#include <fstream>
#include <vector> 




std::vector<std::string> tokenizer(std::string fileName){
    std::vector<std::string> tokenList;


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