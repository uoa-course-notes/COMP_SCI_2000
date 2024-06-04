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
    else throw ParseException();
    return tree;
}

/**
 * @brief class: 'class' className '{' classVarDec* subroutineDec* '}'
 * 
 *
 * Generates a parse tree for a single class
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClass() {
    ParseTree* tree = new ParseTree("class", "");
    tree->addChild(new ParseTree("keyword", mustBe("keyword", "class")->getValue()));
    tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
    tree->addChild(new ParseTree("symbol", mustBe("symbol", "{")->getValue()));

    // Compile class variable declarations
    while (have("keyword", "static") || have("keyword", "field")) {
        tree->addChild(compileClassVarDec());
    }

    // Compile subroutine declarations
    while (have("keyword", "constructor") || have("keyword", "function") || have("keyword", "method")) {
        tree->addChild(compileSubroutine());
    }

    tree->addChild(new ParseTree("symbol", mustBe("symbol", "}")->getValue()));
    return tree;
}

/**
 * @brief  compileClassVarDec: 
 * 
 *
 * Generates a parse tree for a static variable declaration or field declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClassVarDec() {
    ParseTree* tree = new ParseTree("classVarDec", "");
    tree->addChild(new ParseTree("keyword", 
                                      mustBe("keyword",current()->getValue())->getValue()));
    tree->addChild(new ParseTree("keyword", 
                                       mustBe("keyword",current()->getValue())->getValue()));

                                       
    tree->addChild(new ParseTree("identifier",  
                                        mustBe("identifier", current()->getValue())->getValue()));

    while (have("symbol", ",")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", ",")->getValue()));
        tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
    }

    tree->addChild(new ParseTree("symbol", mustBe("symbol", ";")->getValue()));
    return tree;
}

/**
 * Generates a parse tree for a method, function, or constructor
 * @brief Grammar: subroutineDec: ('constructor' | 'field' | 'function')  ('void' | type) subroutineName '(' parameterList ')' subroutineBody
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
    ParseTree* tree = new ParseTree("subroutineDec", "");
    tree->addChild(new ParseTree("keyword", mustBe("keyword", current()->getValue())->getValue()));
    tree->addChild(new ParseTree("keyword", mustBe("keyword", current()->getValue())->getValue())); // void or type
    tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue())); // subroutineName
    tree->addChild(new ParseTree("symbol", mustBe("symbol", "(")->getValue()));
    tree->addChild(compileParameterList());
    tree->addChild(new ParseTree("symbol", mustBe("symbol", ")")->getValue()));
    tree->addChild(compileSubroutineBody());
    return tree;
}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
    ParseTree* tree = new ParseTree("parameterList", "");
    if (!have("symbol", ")")) { // Check if parameter list is not empty
        tree->addChild(new ParseTree("keyword", mustBe("keyword", current()->getValue())->getValue())); // type
        tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue())); // varName
        while (have("symbol", ",")) {
            tree->addChild(new ParseTree("symbol", mustBe("symbol", ",")->getValue()));
            tree->addChild(new ParseTree("keyword", mustBe("keyword", current()->getValue())->getValue())); // type
            tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue())); // varName
        }
    }
    return tree;
}

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() {
    ParseTree* tree = new ParseTree("subroutineBody", "");
    tree->addChild(new ParseTree("symbol", mustBe("symbol", "{")->getValue()));
    while (have("keyword", "var")) {
        tree->addChild(compileVarDec());
    }
    tree->addChild(compileStatements());
    tree->addChild(new ParseTree("symbol", mustBe("symbol", "}")->getValue()));
    return tree;
}


/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
    ParseTree* tree = new ParseTree("varDec", "");
    tree->addChild(new ParseTree("keyword", mustBe("keyword", "var")->getValue()));
    tree->addChild(new ParseTree("keyword", mustBe("keyword", current()->getValue())->getValue())); // type
    tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue())); // varName
    while (have("symbol", ",")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", ",")->getValue()));
        tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue())); // varName
    }
    tree->addChild(new ParseTree("symbol", mustBe("symbol", ";")->getValue()));
    return tree;
}


/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */

ParseTree* CompilerParser::compileStatements() {
    ParseTree* tree = new ParseTree("statements", "");
    while (true) {
        if (have("keyword", "let")) tree->addChild(compileLet());
        else if (have("keyword", "if")) tree->addChild(compileIf());
        else if (have("keyword", "while")) tree->addChild(compileWhile());
        else if (have("keyword", "do")) tree->addChild(compileDo());
        else if (have("keyword", "return")) tree->addChild(compileReturn());
        else break;
    }
    return tree;
}

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() {
    ParseTree* tree = new ParseTree("letStatement", "");
    tree->addChild(new ParseTree("keyword", mustBe("keyword", "let")->getValue()));
    tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
    if (have("symbol", "[")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "[")->getValue()));
        tree->addChild(compileExpression());
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "]")->getValue()));
    }
    tree->addChild(new ParseTree("symbol", mustBe("symbol", "=")->getValue()));
    tree->addChild(compileExpression());
    tree->addChild(new ParseTree("symbol", mustBe("symbol", ";")->getValue()));
    return tree;
}

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() {
    ParseTree* tree = new ParseTree("ifStatement", "");
    tree->addChild(new ParseTree("keyword", mustBe("keyword", "if")->getValue()));
    tree->addChild(new ParseTree("symbol", mustBe("symbol", "(")->getValue()));
    tree->addChild(compileExpression());
    tree->addChild(new ParseTree("symbol", mustBe("symbol", ")")->getValue()));
    tree->addChild(new ParseTree("symbol", mustBe("symbol", "{")->getValue()));
    tree->addChild(compileStatements());
    tree->addChild(new ParseTree("symbol", mustBe("symbol", "}")->getValue()));
    if (have("keyword", "else")) {
        tree->addChild(new ParseTree("keyword", mustBe("keyword", "else")->getValue()));
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "{")->getValue()));
        tree->addChild(compileStatements());
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "}")->getValue()));
    }
    return tree;
}


/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {

    ParseTree* tree = new ParseTree("whileStatement", "");
    tree->addChild(new ParseTree("keyword", mustBe("keyword", "while")->getValue()));
    tree->addChild(new ParseTree("symbol", mustBe("symbol", "(")->getValue()));
    tree->addChild(compileExpression());
    tree->addChild(new ParseTree("symbol", mustBe("symbol", ")")->getValue()));
    tree->addChild(new ParseTree("symbol", mustBe("symbol", "{")->getValue()));
    tree->addChild(compileStatements());
    tree->addChild(new ParseTree("symbol", mustBe("symbol", "}")->getValue()));
    return tree;
}

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() {
    ParseTree* tree = new ParseTree("doStatement", "");
    tree->addChild(new ParseTree("keyword", mustBe("keyword", "do")->getValue()));
    tree->addChild(compileTerm());
    tree->addChild(new ParseTree("symbol", mustBe("symbol", ";")->getValue()));
    return tree;
}

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() {
    ParseTree* tree = new ParseTree("returnStatement", "");
    tree->addChild(new ParseTree("keyword", mustBe("keyword", "return")->getValue()));
    if (!have("symbol", ";")) {
        tree->addChild(compileExpression());
    }
    tree->addChild(new ParseTree("symbol", mustBe("symbol", ";")->getValue()));
    return tree;
}

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() {
    ParseTree* tree = new ParseTree("expression", "");
    tree->addChild(compileTerm());
    while (have("symbol", "+") || have("symbol", "-") || have("symbol", "*") || have("symbol", "/") ||
           have("symbol", "&") || have("symbol", "|") || have("symbol", "<") || have("symbol", ">") ||
           have("symbol", "=")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", current()->getValue())->getValue()));
        tree->addChild(compileTerm());
    }
    return tree;
}

/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileTerm() {
  ParseTree* tree = new ParseTree("term", "");
    if (have("integerConstant", current()->getValue())) {
        tree->addChild(new ParseTree("integerConstant", mustBe("integerConstant", current()->getValue())->getValue()));
    } else if (have("stringConstant", current()->getValue())) {
        tree->addChild(new ParseTree("stringConstant", mustBe("stringConstant", current()->getValue())->getValue()));
    } else if (have("keyword", current()->getValue())) {
        tree->addChild(new ParseTree("keyword", mustBe("keyword", current()->getValue())->getValue()));
    } else if (have("identifier", current()->getValue())) {
        tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
        if (have("symbol", "[")) {
            tree->addChild(new ParseTree("symbol", mustBe("symbol", "[")->getValue()));
            tree->addChild(compileExpression());
            tree->addChild(new ParseTree("symbol", mustBe("symbol", "]")->getValue()));
        } else if (have("symbol", "(")) {
            tree->addChild(new ParseTree("symbol", mustBe("symbol", "(")->getValue()));
            tree->addChild(compileExpressionList());
            tree->addChild(new ParseTree("symbol", mustBe("symbol", ")")->getValue()));
        } else if (have("symbol", ".")) {
            tree->addChild(new ParseTree("symbol", mustBe("symbol", ".")->getValue()));
            tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
            tree->addChild(new ParseTree("symbol", mustBe("symbol", "(")->getValue()));
            tree->addChild(compileExpressionList());
            tree->addChild(new ParseTree("symbol", mustBe("symbol", ")")->getValue()));
        }
    } else if (have("symbol", "(")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "(")->getValue()));
        tree->addChild(compileExpression());
        tree->addChild(new ParseTree("symbol", mustBe("symbol", ")")->getValue()));
    } else if (have("symbol", "-") || have("symbol", "~")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", current()->getValue())->getValue()));
        tree->addChild(compileTerm());
    } else {
        throw ParseException();
    }
    return tree;}

/**
 * Generates a parse tree for an expression list
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpressionList() {
    ParseTree* tree = new ParseTree("expressionList", "");
    if (!have("symbol", ")")) { // Check if expression list is not empty
        tree->addChild(compileExpression());
        while (have("symbol", ",")) {
            tree->addChild(new ParseTree("symbol", mustBe("symbol", ",")->getValue()));
            tree->addChild(compileExpression());
        }
    }
    return tree;}

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
    if (tokenIterator != Tokens.end()) 
    {
        tokenIterator++; // advance the itetator to the next element/token 
    }
    // else throw ParseException();
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
    // std::cout << "in have()" << std::endl;
    // std::cin.ignore();
    if (tokenIterator != Tokens.end()) {
        Token* token = current();
        return token -> getType() == expectedType && token -> getValue() == expectedValue;
    }
    return false;
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
        Token* token = current();
        next(); // advance token (so that we won't check the same token again and introduce exotic bugs)
        return token; // return the next token to be checked 
    }    
    else throw ParseException();  // throw an exception if the token does not match 
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
