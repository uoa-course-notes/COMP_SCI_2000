#include "CompilerParser.h"
#include "ParseTree.h"



/**
 * Constructor for the CompilerParser
 * @param tokens A linked list of tokens to be parsed
 */
CompilerParser::CompilerParser(std::list<Token*> tokens) {
    this -> Tokens = tokens;
    this -> tokenIterator = this -> Tokens.begin(); // set the iterator to the beginning of the list of tokens 
}



ParseTree* CompilerParser::compileProgram() {
    ParseTree* tree = nullptr;
    if (have("keyword", "class")){
        tree  = compileClass(); 
        // compileClass will return a tree of type class with an appropriate value and children, in its subtrees. 
        // This is the only distinction from all other pair of compilerxxx() methods, in some sense, complieProgram() = compileClass().
    }
    else if (have("keyword", "constructor") || 
    have("keyword", "function") || 
    have("keyword", "method")){
        tree = compileSubroutine();
    }
    // having to deal with different types of parse tree structure just shows that the assignment makers didn't put much effort into the course. 
    else if(have("symbol", "{")){
        tree = compileSubroutineBody();
    }
    else if (have("keyword", "var")){
        // std::cout << "yes in var" << std::endl;
        // std::cin.ignore();
        tree = compileVarDec();
    }
    else throw ParseException(); // if the program doesn't start with class 
    return tree;
}



ParseTree* CompilerParser::compileClass() {
    ParseTree* tree = new ParseTree("class", "");
    
    // Checks for keyword: class 
    if (have("keyword", "class")) {
        tree->addChild(new ParseTree("keyword", mustBe("keyword", "class")->getValue()));
    } else {
        throw ParseException();
    }

    // Checks for identifier: Main 
    if (checkIdentifier()) {
        tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
    } else {
        throw ParseException();
    }

    // checks symbol: {
    if (have("symbol", "{")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "{")->getValue()));
    } else {
        throw ParseException();
    }

    // Compile class variable declarations
    while (have("keyword", "static") || have("keyword", "field")) {
        tree->addChild(compileClassVarDec());
    }

    // Compile subroutine declarations
    while (have("keyword", "constructor") || have("keyword", "function") || have("keyword", "method")) {
        tree->addChild(compileSubroutine());
    }
    
    // Checks symbol: }
    if (have("symbol", "}")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "}")->getValue()));
    } else {
        throw ParseException();
    }

    return tree;
}

ParseTree* CompilerParser::compileClassVarDec() {
    ParseTree* tree = new ParseTree("classVarDec", "");

    if (have("keyword", "static") || have("keyword", "field")) {
        tree->addChild(new ParseTree("keyword", mustBe("keyword", current()->getValue())->getValue()));
    } else {
        throw ParseException();
    }

    if (checkType()) {
        tree->addChild(new ParseTree("keyword", mustBe("keyword", current()->getValue())->getValue()));
    } else {
        throw ParseException();
    }

    if (checkIdentifier()) {
        tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
    } else {
        throw ParseException();
    }

    while (have("symbol", ",")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", ",")->getValue()));

        if (checkIdentifier()) {
            tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
        } else {
            throw ParseException();
        }
    }

    if (have("symbol", ";")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", ";")->getValue()));
    } else {
        throw ParseException();
    }

    return tree;
}

ParseTree* CompilerParser::compileSubroutine() {
    ParseTree* tree = new ParseTree("subroutineDec", "");
    if (have("keyword", "constructor") || have("keyword", "function") || have("keyword", "method")) {
        // std::cout << "IN cfm" << std::endl;
        // std::cin.ignore();
        tree->addChild(new ParseTree("keyword", mustBe("keyword", current()->getValue())->getValue()));
    } else {
        throw ParseException();
    }

    if (checkType() || have("keyword", "void")) {
        // std::cout << "IN type or void" << std::endl;
        // std::cin.ignore();

        // since type() also checks for identifier
        // we include a condition to check whether the current token's type is an identifier
        // if it is, then we know checkType() is satisfied and so 
        if (checkIdentifier()){ // this only holds if 2 conditions are true: 
        // when checkType() is true and when the token is actually an identifier, not a type 
            tree -> addChild(new ParseTree("identifier", mustBe("identifier", current() -> getValue()) -> getValue() ));
        } // otherwise, then it must have been the void keyword or any other type (in fact, either checkType() or keyword: void can be satisified to get to here)
        else tree->addChild(new ParseTree("keyword", mustBe("keyword", current()->getValue())->getValue()));
    } else {
        throw ParseException();
    }

    if (checkIdentifier()) {
        // std::cout << "IN identifier" << std::endl;
        // std::cin.ignore();
        tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
    } else {
        throw ParseException();
    }

    if (have("symbol", "(")) {
        // std::cout << "in symbol (" << std::endl;
        // std::cin.ignore();
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "(")->getValue()));
    } else {
        throw ParseException();
    }
    // std::cout << "yes!!!" << std::endl;
    // std::cin.ignore();
    tree->addChild(compileParameterList());

    
    // (empty|non-empty list ==> add ')' to the tree 
    // std::cout << "tree should be almost done" << std::endl;
    // printCurrentToken();
    // std::cin.ignore();


    if (have("symbol", ")")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", ")")->getValue()));
        // std::cout << "yes it does" << std::endl;
        // std::cin.ignore();
    } else {
        // std::cout << "strange" << std::endl;
        // std::cin.ignore();
        throw ParseException();
    }
    // std::cout << "subroutine should be fine. let's get into subroutine body" << std::endl;
    // std::cin.ignore();

    tree->addChild(compileSubroutineBody());
    // std::cout << "subroutine body ok" << std::endl;
    // std::cin.ignore();
    return tree;
}

ParseTree* CompilerParser::compileParameterList() {
    ParseTree* tree = new ParseTree("parameterList", "");
    // std::cout << "in cplist" << std::endl;
    // std::cin.ignore();
    if (!have("symbol", ")")) { // Check if parameter list is not empty
        // std::cout << "list is not empty" << std::endl;
        // std::cin.ignore();
        // check type 
        if (checkType()) {
            if (checkIdentifier()){ // this only holds if 2 conditions are true: 
        // when checkType() is true and when the token is actually an identifier, not a type 
            tree -> addChild(new ParseTree("identifier", mustBe("identifier", current() -> getValue()) -> getValue() ));
            }
            else tree->addChild(new ParseTree("keyword", mustBe("keyword", current()->getValue())->getValue()));
        } 
        else throw ParseException();

        // check varName
        if (checkIdentifier()) {
            tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
        } else {
            throw ParseException();
        }

        // check (('.' type varName)*)?
        while (have("symbol", ",")) {
            tree->addChild(new ParseTree("symbol", mustBe("symbol", ",")->getValue()));

            if (checkType()) {
                if (checkIdentifier()) tree -> addChild(new ParseTree("identifier", mustBe("identifier", current() -> getValue()) -> getValue()));
                else tree->addChild(new ParseTree("keyword", mustBe("keyword", current()->getValue())->getValue()));
            } 
            else throw ParseException(); // when it's not a valid type (identifier)

            // check varName
            if (checkIdentifier()) { 
                tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
            } 
            else throw ParseException();
        }
    }
    // empty list (just close the list and then return the tree)
    // tree -> addChild(new ParseTree("symbol", mustBe("symbol", current() -> getValue()) -> getValue()));

    return tree;
}

ParseTree* CompilerParser::compileSubroutineBody() {
    ParseTree* tree = new ParseTree("subroutineBody", "");

    if (have("symbol", "{")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "{")->getValue()));
        // std::cout << "yes, have {" << std::endl;
        // std::cin.ignore();
    } else {
        throw ParseException();
    }

    // check varDec* 
    while (have("keyword", "var")) {
        // std::cout << "in while" << std::endl;
        // std::cin.ignore();
        tree->addChild(compileVarDec());
    }
    // std::cout << "not in while" << std::endl;
    // std::cin.ignore();
    tree->addChild(compileStatements());
    // std::cout << "statements ok" << std::endl;
    // std::cin.ignore();

    if (have("symbol", "}")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "}")->getValue()));
        // std::cout << "have }" << std::endl;
        // std::cin.ignore();
    } else {
        throw ParseException();
    }

    return tree;
}


ParseTree* CompilerParser::compileVarDec() {
    ParseTree* tree = new ParseTree("varDec", "");
    
    // check 'var'
    if (have("keyword", "var")) {
        tree->addChild(new ParseTree("keyword", mustBe("keyword", "var")->getValue()));
    }
    else throw ParseException();
    
    // std::cout << "yes there is var" << std::endl;
    // std::cin.ignore();

    // check type 
    if (checkType()) {
        // Again, check whether it's a type char, int bool or of type Object 
        if (checkIdentifier()) {
            // std::cout << "yes it is " << current() -> getValue() << " and " << current() -> getType() << std::endl;
            // std::cin.ignore();
            tree -> addChild(new ParseTree("identifier", mustBe("identifier", current() -> getValue()) -> getValue()));
        }
        else tree->addChild(new ParseTree("keyword", mustBe("keyword", current()->getValue())->getValue()));
    } 
    else throw ParseException();
    
    // std::cout << "great" << std::endl;
    // std::cin.ignore();
    if (checkIdentifier()) {
        tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
    } else {
        throw ParseException();
    }

    while (have("symbol", ",")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", ",")->getValue()));

        if (checkIdentifier()) {
            tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
        } else {
            throw ParseException();
        }
    }

    if (have("symbol", ";")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", ";")->getValue()));
    } else {
        throw ParseException();
    }

    return tree;
}

ParseTree* CompilerParser::compileStatements() {
    ParseTree* tree = new ParseTree("statements", "");

    // std::cout << current() -> getValue() << "stop" << std::endl;
    // std::cin.ignore();
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

ParseTree* CompilerParser::compileLet() {
    ParseTree* tree = new ParseTree("letStatement", "");
    if (have("keyword", "let")) {
        tree->addChild(new ParseTree("keyword", mustBe("keyword", "let")->getValue()));
    } else {
        throw ParseException();
    }
    // std::cout << "let ok" << std::endl;
    // std::cin.ignore();

    // check varName 
    if (checkIdentifier()) {
        tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
    } 
    else throw ParseException();
    // std::cout << "checkId ok" << std::endl;
    // std::cin.ignore();

    // check ('[' expression ']')
    if (have("symbol", "[")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "[")->getValue()));
        tree->addChild(compileExpression());
        if (have("symbol", "]")) {
            tree->addChild(new ParseTree("symbol", mustBe("symbol", "]")->getValue()));
        } else {
            throw ParseException();
        }
    }
    // std::cout << "shoudl be pok" << std::endl;
    // std::cin.ignore();
    // check '='
    if (have("symbol", "=")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "=")->getValue()));
    } 
    else throw ParseException();
    // std::cout << "there is symbol" << std::endl;
    // std::cin.ignore();

    tree->addChild(compileExpression());

    // std::cout << "compile expression" << std::endl;
    // std::cin.ignore();

    if (have("symbol", ";")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", ";")->getValue()));
    } else {
        throw ParseException();
    }

    return tree;
}

ParseTree* CompilerParser::compileIf() {
    ParseTree* tree = new ParseTree("ifStatement", "");
    if (have("keyword", "if")) {
        tree->addChild(new ParseTree("keyword", mustBe("keyword", "if")->getValue()));
    } else {
        throw ParseException();
    }

    if (have("symbol", "(")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "(")->getValue()));
    } else {
        throw ParseException();
    }

    tree->addChild(compileExpression());

    if (have("symbol", ")")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", ")")->getValue()));
    } else {
        throw ParseException();
    }

    if (have("symbol", "{")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "{")->getValue()));
    } else {
        throw ParseException();
    }

    tree->addChild(compileStatements());

    if (have("symbol", "}")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "}")->getValue()));
    } else {
        throw ParseException();
    }

    if (have("keyword", "else")) {
        tree->addChild(new ParseTree("keyword", mustBe("keyword", "else")->getValue()));
        if (have("symbol", "{")) {
            tree->addChild(new ParseTree("symbol", mustBe("symbol", "{")->getValue()));
        } else {
            throw ParseException();
        }
        tree->addChild(compileStatements());
        if (have("symbol", "}")) {
            tree->addChild(new ParseTree("symbol", mustBe("symbol", "}")->getValue()));
        } else {
            throw ParseException();
        }
    }

    return tree;
}

ParseTree* CompilerParser::compileWhile() {
    ParseTree* tree = new ParseTree("whileStatement", "");
    if (have("keyword", "while")) {
        tree->addChild(new ParseTree("keyword", mustBe("keyword", "while")->getValue()));
    } else {
        throw ParseException();
    }

    if (have("symbol", "(")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "(")->getValue()));
    } else {
        throw ParseException();
    }

    tree->addChild(compileExpression());

    if (have("symbol", ")")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", ")")->getValue()));
    } else {
        throw ParseException();
    }

    if (have("symbol", "{")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "{")->getValue()));
    } else {
        throw ParseException();
    }

    tree->addChild(compileStatements());

    if (have("symbol", "}")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "}")->getValue()));
    } else {
        throw ParseException();
    }

    return tree;
}

ParseTree* CompilerParser::compileDo() {
    ParseTree* tree = new ParseTree("doStatement", "");
    if (have("keyword", "do")) {
        tree->addChild(new ParseTree("keyword", mustBe("keyword", "do")->getValue()));
    } else {
        throw ParseException();
    }

    tree->addChild(compileTerm());

    if (have("symbol", ";")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", ";")->getValue()));
    } else {
        throw ParseException();
    }

    return tree;
}

ParseTree* CompilerParser::compileReturn() {
    ParseTree* tree = new ParseTree("returnStatement", "");
    if (have("keyword", "return")) {
        tree->addChild(new ParseTree("keyword", mustBe("keyword", "return")->getValue()));
    } else {
        throw ParseException();
    }

    if (!have("symbol", ";")) {
        tree->addChild(compileExpression());
    }

    if (have("symbol", ";")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", ";")->getValue()));
    } else {
        throw ParseException();
    }

    return tree;
}

ParseTree* CompilerParser::compileExpression() {
    ParseTree* tree = new ParseTree("expression", "");
    // std::cout << current() -> getValue() << " in expression with type = " << current() -> getType() << std::endl;   
    // std::cin.ignore();
    
    if (have("keyword", "skip")){
        tree -> addChild(new ParseTree("keyword", mustBe("keyword", "skip") -> getValue()));
        // std::cout << "yes skip " << std::endl;
        // std::cin.ignore();
    }
    else{ // if not then we check for term     
        // check term
        tree->addChild(compileTerm());
        // std::cout << "in term ok" << std::endl;
        // std::cin.ignore();

        // check (op term)*
        while (have("symbol", "+") || have("symbol", "-") || have("symbol", "*") || have("symbol", "/") ||
            have("symbol", "&") || have("symbol", "|") || have("symbol", "<") || have("symbol", ">") ||
            have("symbol", "=")) {
            tree->addChild(new ParseTree("symbol", mustBe("symbol", current()->getValue())->getValue()));
            tree->addChild(compileTerm());
        }

    }
    // std::cout << "ye" << std::endl;
    // std::cin.ignore();
    return tree;
}



ParseTree* CompilerParser::compileTerm() {
    ParseTree* tree = new ParseTree("term", "");

    if (checkIntegerConstants()) {
        tree->addChild(new ParseTree("integerConstant", mustBe("integerConstant", current()->getValue())->getValue()));
    } else if (checkStringConstants()) {
        tree->addChild(new ParseTree("stringConstant", mustBe("stringConstant", current()->getValue())->getValue()));
    } else if (checkKeywords()) {
        tree->addChild(new ParseTree("keyword", mustBe("keyword", current()->getValue())->getValue()));
    } else if (checkIdentifier()) {
        tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
        if (have("symbol", "[")) {
            tree->addChild(new ParseTree("symbol", mustBe("symbol", "[")->getValue()));
            tree->addChild(compileExpression());
            if (have("symbol", "]")) {
                tree->addChild(new ParseTree("symbol", mustBe("symbol", "]")->getValue()));
            } else {
                throw ParseException();
            }
        } else if (have("symbol", "(")) {
            tree->addChild(new ParseTree("symbol", mustBe("symbol", "(")->getValue()));
            tree->addChild(compileExpressionList());
            if (have("symbol", ")")) {
                tree->addChild(new ParseTree("symbol", mustBe("symbol", ")")->getValue()));
            } else {
                throw ParseException();
            }
        } else if (have("symbol", ".")) {
            tree->addChild(new ParseTree("symbol", mustBe("symbol", ".")->getValue()));
            if (checkIdentifier()) {
                tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
            } else {
                throw ParseException();
            }
            if (have("symbol", "(")) {
                tree->addChild(new ParseTree("symbol", mustBe("symbol", "(")->getValue()));
            } else {
                throw ParseException();
            }
            tree->addChild(compileExpressionList());
            if (have("symbol", ")")) {
                tree->addChild(new ParseTree("symbol", mustBe("symbol", ")")->getValue()));
            } else {
                throw ParseException();
            }
        }
    } else if (have("symbol", "(")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "(")->getValue()));
        tree->addChild(compileExpression());
        if (have("symbol", ")")) {
            tree->addChild(new ParseTree("symbol", mustBe("symbol", ")")->getValue()));
        } else {
            throw ParseException();
        }
    } else if (have("symbol", "-") || have("symbol", "~")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", current()->getValue())->getValue()));
        tree->addChild(compileTerm());
    } else {
        throw ParseException();
    }

    return tree;
}

ParseTree* CompilerParser::compileExpressionList() {
    ParseTree* tree = new ParseTree("expressionList", "");

    if (!have("symbol", ")")) { // Check if expression list is not empty
        tree->addChild(compileExpression());

        while (have("symbol", ",")) {
            tree->addChild(new ParseTree("symbol", mustBe("symbol", ",")->getValue()));
            tree->addChild(compileExpression());
        }
    }

    return tree;
}

void CompilerParser::next() {
    if (tokenIterator != Tokens.end()) ++tokenIterator;
}

Token* CompilerParser::current() {
    if (tokenIterator != Tokens.end()) return *tokenIterator;
    else throw ParseException(); // Handle end of token list
}

bool CompilerParser::have(std::string expectedType, std::string expectedValue) {
    if (tokenIterator != Tokens.end()) {
        Token* token = current();
        return token->getType() == expectedType && token->getValue() == expectedValue;
    }
    return false;
}

Token* CompilerParser::mustBe(std::string expectedType, std::string expectedValue) {
    if (have(expectedType, expectedValue)) {
        Token* token = current();
        next();
        return token;
    }
    throw ParseException(); // Throw an exception if the token does not match
}

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


std::list<Token*>::iterator CompilerParser::getIt(){return tokenIterator;}


/**
 * Definition of a ParseException
 * You can use this ParseException with `throw ParseException();`
 */
const char* ParseException::what() {
    return "An Exception occurred while parsing!";
}
