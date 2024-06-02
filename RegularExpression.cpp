#include <iostream>
#include <string>
#include <regex>



int main(int argc, char** argv){
    // std::string expression = "softwareTesting";
    // std::string matchString = "(soft)(.*)"; 
    // std::regex matcher(matchString);
    // bool match = std::regex_match(expression, matcher);
    // if (match) std::cout << "<" << expression << "> matches <" <<  matchString << "> !" << std::endl;
    // else std::cout << "Something went wrong" << std::endl;
    
    if (std::regex_match("softwareTesting", std::regex("(soft)(.*)"))) std::cout << "string:literal => matched" << std::endl;
    

    // 
    const char mystr[]  = "SoftwareTestingHelp";
    std::string str("software");
    std::regex str_expr("(soft)(.*)");
    if (std::regex_match(mystr, str_expr)) std::cout << "string:obj => matched" << std::endl;

    if (std::regex_match(str.begin(), str.end(), str_expr)) std::cout << "str:range(begin-end) => matched" << std::endl;
    

    std::cmatch cm;
    std::regex_match(mystr, cm,str_expr);

    std::smatch sm;
    std::regex_match(str, sm, str_expr);


    


    return 0;
}