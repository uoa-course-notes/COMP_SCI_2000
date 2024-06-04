#include "CompilerParser.h"

ParseTree* CompilerParser::compileProgram() {
    ParseTree* tree = new ParseTree("program", "");
    tree->addChild(compileClass());
    return tree;
}

ParseTree* CompilerParser::compileClass() {
    ParseTree* tree = new ParseTree("class", "");

    if (have("keyword", "class")) {
        tree->addChild(new ParseTree("keyword", mustBe("keyword", "class")->getValue()));
    } else {
        throw ParseException();
    }

    if (checkIdentifier()) {
        tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
    } else {
        throw ParseException();
    }

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
        tree->addChild(new ParseTree("keyword", mustBe("keyword", current()->getValue())->getValue()));
    } else {
        throw ParseException();
    }

    if (checkType() || have("keyword", "void")) {
        tree->addChild(new ParseTree("keyword", mustBe("keyword", current()->getValue())->getValue()));
    } else {
        throw ParseException();
    }

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

    tree->addChild(compileParameterList());

    if (have("symbol", ")")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", ")")->getValue()));
    } else {
        throw ParseException();
    }

    tree->addChild(compileSubroutineBody());
    return tree;
}

ParseTree* CompilerParser::compileParameterList() {
    ParseTree* tree = new ParseTree("parameterList", "");

    if (!have("symbol", ")")) { // Check if parameter list is not empty
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
        }
    }

    return tree;
}

ParseTree* CompilerParser::compileSubroutineBody() {
    ParseTree* tree = new ParseTree("subroutineBody", "");

    if (have("symbol", "{")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "{")->getValue()));
    } else {
        throw ParseException();
    }

    while (have("keyword", "var")) {
        tree->addChild(compileVarDec());
    }

    tree->addChild(compileStatements());

    if (have("symbol", "}")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "}")->getValue()));
    } else {
        throw ParseException();
    }

    return tree;
}

ParseTree* CompilerParser::compileVarDec() {
    ParseTree* tree = new ParseTree("varDec", "");

    if (have("keyword", "var")) {
        tree->addChild(new ParseTree("keyword", mustBe("keyword", "var")->getValue()));
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

ParseTree* CompilerParser::compileLet() {
    ParseTree* tree = new ParseTree("letStatement", "");
    if (have("keyword", "let")) {
        tree->addChild(new ParseTree("keyword", mustBe("keyword", "let")->getValue()));
    } else {
        throw ParseException();
    }

    if (checkIdentifier()) {
        tree->addChild(new ParseTree("identifier", mustBe("identifier", current()->getValue())->getValue()));
    } else {
        throw ParseException();
    }

    if (have("symbol", "[")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "[")->getValue()));
        tree->addChild(compileExpression());
        if (have("symbol", "]")) {
            tree->addChild(new ParseTree("symbol", mustBe("symbol", "]")->getValue()));
        } else {
            throw ParseException();
        }
    }

    if (have("symbol", "=")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", "=")->getValue()));
    } else {
        throw ParseException();
    }

    tree->addChild(compileExpression());

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
    tree->addChild(compileTerm());

    while (have("symbol", "+") || have("symbol", "-") || have("symbol", "*") || have("symbol", "/") ||
           have("symbol", "&") || have("symbol", "|") || have("symbol", "<") || have("symbol", ">") ||
           have("symbol", "=")) {
        tree->addChild(new ParseTree("symbol", mustBe("symbol", current()->getValue())->getValue()));
        tree->addChild(compileTerm());
    }

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



/**
 * Definition of a ParseException
 * You can use this ParseException with `throw ParseException();`
 */
const char* ParseException::what() {
    return "An Exception occurred while parsing!";
}
