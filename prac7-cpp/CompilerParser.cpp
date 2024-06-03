#include "CompilerParser.h"
#include "ParseTree.h"
#include "Token.h"


/**
 * Constructor for the CompilerParser
 * @param tokens A linked list of tokens to be parsed
 */
CompilerParser::CompilerParser(std::list<Token*> tokens) {
    this -> Tokens = tokens;
    this -> tokenIterator = this -> Tokens.begin(); // set the iterator to the beginning of the list of tokens 
}


std::list<Token*>::iterator CompilerParser::getIt(){return tokenIterator;}

/**
 * Generates a parse tree for a single program
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileProgram() {
    ParseTree* tree;
    if (have("keyword", "class")){
        compileClass();
    }
    return tree;
}

/**
 * Generates a parse tree for a single class
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClass() {
    // ParseTree* tree = new ParseTree("class", "");
    // tree -> addChild(mustBe("keyword", "class"));
    
    // if (checkIdentifier()) tree -> addChild(mustBe("identifier", current() -> getValue()));
    // else throw ParseException();


    // tree -> addChild(mustBe("symbol", "{"));

    // while(have("keyword", "static") || 
    //       have("keyword", "field"))
    // {
    //     compileClassVarDec();
    // }
  
    // while(have("keyword", "constructor") || 
    //       have("keyword", "function")    ||
    //       have("keyword", "method")   )
    // {
    //     compileSubroutine();
    // }

    // tree -> addChild(mustBe("symbol", "}"));
    // return tree;    

    ParseTree* tree = new ParseTree("class", "");
    if (current() -> getValue() == "class"){
        tree -> addChild(new ParseTree("keyword", "class"));
        next();
        if (checkIdentifier()){
            tree -> addChild(new ParseTree("identifier", current() -> getValue()));
            next();
            if (checkSymbol()){
                if (have("symbol", "{")){
                    tree -> addChild(new ParseTree("symbol", "{"));
                }
                else throw ParseException();
                next();
                while (have("keyword", "static")
                    || have("symbol", "field")){
                    compileClassVarDec();
                }

                next();

                while(have("keyword", "constructor") || 
                have("keyword", "function")    ||
                have("keyword", "method")   )
                {
                compileSubroutine();
                }
                next();

                if (have("symbol", "}")) tree -> addChild(new ParseTree("symbol", "}"));
                else throw ParseException();
            }
        }
        else throw ParseException();
    }
    else throw ParseException();

    std::cout << "Everything went well" << std::endl;
    return tree;
}

/**
 * Generates a parse tree for a static variable declaration or field declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClassVarDec() {
    ParseTree* tree = new ParseTree("classVarDec", "");
    // if (have("keyword", "static")){
    if (have("keyword", "static") || 
        have("keyword", "field"))
    {
        tree -> addChild(mustBe("keyword", current() -> getValue()));
        
    }
    if (have("keyword", "int") || 
        have("keyword", "char") || 
        have("keyword", "boolean") ||
        have("identifier", "regex expression over here"))
        tree -> addChild(mustBe("keyword", current() -> getValue()));
    else throw ParseException();

    tree -> addChild(mustBe("identifier", "regex expression str"));
    while (have("symbol", ",") &&
           have("identifier", "regex expression here"))
    {
        tree -> addChild(mustBe("symbol", ","));
        tree -> addChild(mustBe("identifier", "regex_expression in here"));
    }
    tree -> addChild(mustBe("symbol", ";"));
    return tree;
}

/**
 * Generates a parse tree for a method, function, or constructor
 * @brief Grammar: subroutineDec: ('constructor' | 'field' | 'function')  ('void' | type) subroutineName '(' parameterList ')' subroutineBody
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
    ParseTree* tree = new ParseTree("subroutineDec", "");
    if (have("keyword", "constructor") || 
        have("keyword", "function") || 
        have("keyword", "method"))
    {
        tree -> addChild(mustBe("keyword", current() -> getValue())); 
        if (have("keyword", "void") || 
            have("keyword", "int") || 
            have("keyword", "char") || 
            have("keyword", "boolean") ||
            have("identifier", "regex expression over here"))
        {
            tree -> addChild(mustBe("", ""));
            // subroutine name 
        }

    }   
    else throw ParseException(); // missing subroutine name (either method, constructor or function)
    return tree;
    
}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
    return NULL;
}

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() {
    return NULL;
}

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
    return NULL;
}

/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileStatements() {
    return NULL;
}

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() {
    return NULL;
}

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() {
    return NULL;
}

/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {


    return NULL;
}

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() {
    return NULL;
}

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() {
    return NULL;
}

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() {
    return NULL;
}

/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileTerm() {
    return NULL;
}

/**
 * Generates a parse tree for an expression list
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpressionList() {
    return NULL;
}

// =======================HELPER METHODS BEGIN=======================
// Helper methods have been constructed to help with validating lexical elements and make the overall code more readable. 
/**
 * @brief Check whether the given keyword is 
 * 
 * @param keyword 
 * @return true 
 * @return false 
 */
bool CompilerParser::checkKeywords(){
    if (
        have("keyword", "class") ||
        have("keyword", "constructor") || 
        have("keyword", "function"), 
        have("keyword", "method") || 
        have("keyword", "field") || 
        have("keyword", "static") || 
        have("keyword", "var") || 
        checkType() || // check for int, char, boolean
        have("keyword", "void") || 
        have("keyword", "true") || 
        have("keyword", "false") || 
        have("keyword", "null") || 
        have("keyword", "this") || 
        have("keyword", "let") || 
        have("keyword", "do") || 
        have("keyword", "if") || 
        have("keyword", "else") || 
        have("keyword", "while") || 
        have("keyword", "return") || 
        have("keyword", "loop") 
        ) return true;
    return false;
}


bool CompilerParser::checkType(){
    // we match the type and value of the current token using 3 conditional or statements with the verbatims boolean, int and char, with type keyword. 
    // And an extra condition on identifier-checking. 
    if (have("keyword", "boolean") || 
        have("keyword", "int") || 
        have("keyword", "char") || 
        checkIdentifier()) return true;
    else return false; 
}


bool CompilerParser::checkIntegerConstants(){
    std::regex intConstRegex("^(0|[1-9][0-9]{0,4})$");

    // First, check if the token matches the regex pattern (0-checking is embedded in this pattern)
    if (!std::regex_match(current() -> getValue(), intConstRegex)) {
        return false;
    }

    // Second, check if the integer value is within the range 0 to 32767
    int value = std::stoi(current() -> getValue());
    return value <= 32767;
}

/**
 * @brief Validate whether the expected string type and value matches the given current token's type and value. 
 * 
 * @return true, if the token's type and value match the expected ones. 
 * @return false, either the token's type or value failed to match. 
 */
bool CompilerParser::checkStringConstants(){
    std::regex strConstRegex("^[^\n\"]*$");
    bool matched = std::regex_match(current() -> getValue(), strConstRegex);
    if (matched) return have("stringConstant", current() -> getValue());
    else return false;
}

// /**
//  * @brief A sub-helper method to encapsulate integer- and string-constants checking
//  * 
//  * @return true if both 
//  * @return false 
//  */
// bool CompilerParser::checkConstants(){
//     return checkIntegerConstants() && checkStringConstants();
// }

bool CompilerParser::checkIdentifier(){
    std::regex idRegex("[a-zA-Z_][a-zA-Z0-9_]*$");
    bool matched = std::regex_match(current() -> getValue(), idRegex);
    if(matched){ // if the token's value matched the expected value
    // we would do another type-checking on the token type using have by passing in the token's value to avoid errors.
        return have("identifier", current() -> getValue());
    }
    // if matched = false, then we return false, meaning that the token's value does not match the expected value and the so we don't need to check its type. 
    else return false;
}


bool CompilerParser::checkSymbol(){
    if (
        have("symbol", "{") || 
        have("symbol", "}") || 
        have("symbol", "(") || 
        have("symbol", ")") || 
        have("symbol", "[") || 
        have("symbol", "]") || 
        have("symbol", ".") || 
        have("symbol", ",") || 
        have("symbol", ";") || 
        have("symbol", "+") || 
        have("symbol", "-") || 
        have("symbol", "*") || 
        have("symbol", "/") || 
        have("symbol", "&") || 
        have("symbol", "|") || 
        have("symbol", "<") || 
        have("symbol", ">") || 
        have("symbol", "=") || 
        have("symbol", "~") 
    ) return true;
    else return false;
}

// =======================HELPER METHODS END=======================
/**
 * Advance to the next token
 */
void CompilerParser::next(){
    // std::cout << "in next()" << std::endl;
    
    if (tokenIterator != Tokens.end()) 
    {
        // std::cout << "in if next()" << std::endl;
        // printCurrentToken();
        // std::cin.ignore();
        // auto nextIt = tokenIterator;
        // ++nextIt;
        // if (nextIt != Tokens.end()){
        //     std::cout << "Advancing it" << std::endl;
        //     std::cin.ignore();
        // }   
        // else {
        //     std::cout << "Cannot advance, end() reached" << std::endl;
        // }
        tokenIterator++; // advance the itetator to the next element/token 
    }
    else{
        std::cout << "Iterator is already at the end" << std::endl;
        
    }
}

/**
 * @brief Checks if the current token matches the expected type and value 
 *
 * Return the current token
 * @return the Token
 */
Token* CompilerParser::current(){
    // if (tokenIterator != tokens.end()) return *tokenIterator;
    // throw ParseException(); // Handle end of token list by throwing an exception  
    if (tokenIterator != Tokens.end()) return *tokenIterator;
    else throw ParseException(); // Throws an exception when we are at the end of the token list 
}

/**
 * Check if the current token matches the expected type and value.
 * @return true if a match, false otherwise
 */
bool CompilerParser::have(std::string expectedType, std::string expectedValue){
    Token* token = current();
    // std::cout << "in have()" << std::endl;
    // std::cin.ignore();
    return token -> getType() == expectedType && token -> getValue() == expectedValue;
}


/**
 * Check if the current token matches the expected type and value.
 * If so, advance to the next token, returning the current token, otherwise throw a ParseException.
 * @return the current token before advancing
 */
Token* CompilerParser::mustBe(std::string expectedType, std::string expectedValue){ 
    // Token* token = current();
    // std::cout << "in mustBe()" << std::endl;
    if (have(expectedType, expectedValue)){
        // std::cout << "in if mustBe()" << std::endl;
        // std::cin.ignore();
        next(); // advance token (so that we won't check the same token again and introduce exotic bugs)
        return current(); // return the next token to be checked 
    }    
    throw ParseException(); 
}


void CompilerParser::printCurrentToken(){
    auto currentIt = getIt();
    auto token = *currentIt;
    std::cout << "Current token Info: <" <<  token -> getType() 
              << "," << token -> getValue() 
              << ">" << std::endl;
}


void CompilerParser::printTokens(){
    while(tokenIterator != Tokens.end()){
        printCurrentToken();
        next();
    }
}

/**
 * Definition of a ParseException
 * You can use this ParseException with `throw ParseException();`
 */
const char* ParseException::what() {
    return "An Exception occurred while parsing!";
}
