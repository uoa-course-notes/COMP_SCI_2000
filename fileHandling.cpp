#include <cstdio>
#include <iostream>
#include <fstream>





int main(int argc, char** argv){
    std::fstream myFile;
    const std::string fileName = "seth.txt";
    myFile.open(fileName, std::ios::out);  // write mode into a provided text file 

    // make sure the file is open successfully. 
    if (myFile.is_open()){
        // We can write into a file 
        myFile << "Goodbye.\n";
        myFile << "This is the second line.\n";
        myFile.close();
    }

    // Since the text file doesn't exist, writing into such a file will create the file first and then write into it. 
    // myFile.open(fileName, std::ios::app); // append mode 
    // if (myFile.is_open()){
    //     // We can write into a file 
    //     myFile << "This is Seth!\n";
    //     myFile.close();
    // }

    myFile.open(fileName, std::ios::app); // append mode 
    if (myFile.is_open()){
        myFile << "Appended!" << std::endl;
        myFile.close();
    }


    // Reading mode 
    myFile.open(fileName, std::ios::in); // read mode 
    if (myFile.is_open()){
        // Now, we can read from it 
        std::string line; // one line at a time 
        while (std::getline(myFile, line)){
            std::cout << line << std::endl;
        }
        myFile.close();
    }

    // 2 different tasks 
    // How to encipher every letter into a numerical value. 
    // 

    // 1. Decipher a text file of ASCII numbers into the English alphabet to be able to read the text

    // 2. Structure: Read questions and answers into a file. 





    return 0;
}