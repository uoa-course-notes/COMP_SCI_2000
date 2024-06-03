#ifndef COMPILERPARSER_H
#define COMPILERPARSER_H

#include <exception>
#include <regex>
#include <iostream>

#include "ParseTree.h"
#include "Token.h"

class CompilerParser {
    private:
        std::list<Token*> Tokens; // List of tokens 
        std::list<Token*>::iterator tokenIterator; // iterator for the token 
        // Any additional state variables 
    public:
        // We can assume that the given list of tokens below contains
        // tokens that have been correctly tokenized and we don't need to check for bad tokens. 
        CompilerParser(std::list<Token*> tokens);

        ParseTree* compileProgram();
        ParseTree* compileClass();
        ParseTree* compileClassVarDec();
        ParseTree* compileSubroutine();
        ParseTree* compileParameterList();
        ParseTree* compileSubroutineBody();
        ParseTree* compileVarDec();

        ParseTree* compileStatements();
        ParseTree* compileLet();
        ParseTree* compileIf();
        ParseTree* compileWhile();
        ParseTree* compileDo();
        ParseTree* compileReturn();

        ParseTree* compileExpression();
        ParseTree* compileTerm();
        ParseTree* compileExpressionList();

        // Helper methods- to validate against the Grammar 
        bool checkKeywords(); // type is also a part of the list of keywords
        bool checkType();
        bool checkSymbol();
        bool checkConstants();
        bool checkIntegerConstants();
        bool checkStringConstants();
        bool checkIdentifier();

        // 
        void next();
        Token* current();
        bool have(std::string expectedType, std::string expectedValue);
        Token* mustBe(std::string expectedType, std::string expectedValue);

        std::list<Token*>::iterator getIt();
        void printCurrentToken();
        void printTokens();
};

class ParseException : public std::exception {
    public:
        const char* what();
};

#endif /*COMPILERPARSER_H*/