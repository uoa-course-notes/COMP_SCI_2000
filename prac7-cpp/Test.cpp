// #include "CompilerParser.h"

#include "CompilerParser.h"
#include "ParseTree.h"



int main(int argc, char** argv){
    std::list<Token*> tokens;
    tokens.push_back(new Token("keyword", "class"));
    tokens.push_back(new Token("identifier", "MyClass"));
    tokens.push_back(new Token("symbol", "{"));
    tokens.push_back(new Token("symbol", "}"));

    // /////////////////////

//     ParseTree* tree; 

//     CompilerParser parser(tokens);
//    // Check for keyword: class
//     bool haveClass = parser.have("keyword", "class");
//     if (haveClass) std::cout << "keyword class is there!" <<  std::endl;
//     else std::cout << "An error" << std::endl;
//     // Check for identifier: className
//     auto listIterator = parser.getIt();
//     // NOTE: tokenIterator's initialization doesn't seem to be a problem here. 

//     // std::cout << (*listIterator) -> getType() << ":" << (*listIterator) -> getValue() << std::endl;

//     // parser.printCurrentToken();
//     // ++listIterator;

//     // parser.printCurrentToken();
//     // parser.printTokens();

//     tree -> addChild(parser.mustBe("keyword", "class"));
    
    // tree -> addChild(new ParseTree("keyword", "class"));
    
    
    // bool haveClassName = parser.checkIdentifier();
    // if (haveClassName) std::cout << "class name is there" << std::endl;
    // else std::cout << "Invalid class name" << std::endl;
    
    // std::cout << tree->tostring(0);
    // /////////////////////



    // =======================================================================
    try {
        CompilerParser parser(tokens);
        ParseTree* result = parser.compileProgram(); // return a parse tree 
        if (result != NULL){
            std::cout << result->tostring() << std::endl;
        }
        else{
            std::cout << "result is null" << std::endl;
            std::cin.ignore();
        }
    } catch (ParseException e) {
        std::cout << "Error Parsing!" << std::endl;
    }

    
    // std::regex r("[a-zA-Z_][a-zA-Z0-9_]*$");

    // std::string tests[] = {"look", "h0e13llo", 
    // "guys10", "no00", "oopow", 
    // "1no", "02op", "90l"};

    // for (auto token: tests){
    //     if (std::regex_match(token, r)){
    //         std::cout << token << "matches!" << std::endl;
    //     }
    //     else std::cout << token << "failed!" << std::endl;
    // }

    // =======================identifier matcher validated===================
    
    // std::regex strConstRegex("^\"[^\n\"]*\"$");
    // std::regex strConstRegex("^[^\n\"]*$");

    // // std::regex strConstRegex(R"([^"\n]*[\n"][^"\n]*)");
    // std::string tests[] = {"look", "h0e13llo", 
    // "gu\"ys10", "no\"00", "\\ooponw", 
    // "1no\n", "02\"o\np", "90l"};
    // int i = 0;
    // for (auto token: tests){
    //     std::cout << "Test #" << ++i << ":  \t";
    //     if (std::regex_match(token,strConstRegex)){
    //         std::cout << token << " matches!" << std::endl;
    //     }
    //     else std::cout << token << " failed!" << std::endl;
    // }
    // =======================string matcher validated===================

    
    // std::regex intConstRegex("^(0|[1-9][0-9]{0,4})$");
    // std::regex intConstRegex("^(0|[1-9][0-9]{0,4})$");
    // std::string tests[] = {"120", "00", "000", 
    // "ok", "90", "-10", "-1", "1", "32767", 
    // "90000", "32768", "-190293", "09931", "00123"};
    // int i = 0;
    // for (auto token: tests){
    //     std::cout << "Test #" << ++i << ":  \t";
    //     if (std::regex_match(token,intConstRegex)){ // 1st: Check if the token matches the pattern
    //         // 2nd: Check if the integer value is within the range 0 to 32767
    //         int value = std::stoi(token);
    //         if(value <= 32767) 
    //             std::cout << token << " matches!" << std::endl;
    //         else std::cout << token << " failed!" << std::endl;
    //     }
    //     else std::cout << token << " failed!" << std::endl;
    // }
    // =======================integer matcher validated===================


    // Simulating parse tree 




    return 0;
}