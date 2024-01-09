#include "../include/interpreter.h"
#include "catch2/catch_all.hpp"

TEST_CASE("Check if reserved word")
{
    REQUIRE(isReservedWord("print"));
    REQUIRE(isReservedWord("read"));

    REQUIRE(!isReservedWord("readd"));
    REQUIRE(!isReservedWord(""));
}

TEST_CASE("Check if variable name")
{
    REQUIRE(isVariableName("abc"));
    REQUIRE(!isVariableName("aBC"));
    REQUIRE(!isVariableName("abc!"));
    REQUIRE(!isVariableName("abc2345"));

    // Reserved words should not be valid variable names
    REQUIRE(!isVariableName("print"));
    REQUIRE(!isVariableName("read"));
}

TEST_CASE("Check if constant (number)")
{
    REQUIRE(isConstant("123"));

    REQUIRE(!isConstant("doe123"));
    REQUIRE(!isConstant("123!£$"));
    REQUIRE(!isConstant("123ABC[]"));
}


TEST_CASE("Check if operator - overload 1")
{
    REQUIRE(isOperator('+'));
    REQUIRE(isOperator('-'));
    REQUIRE(isOperator('*'));
    REQUIRE(isOperator('/'));
    REQUIRE(isOperator('%'));
    REQUIRE(isOperator('('));
    REQUIRE(isOperator(')'));

    REQUIRE(!isOperator('$'));
}

TEST_CASE("Check if operator - overload 2")
{   
    REQUIRE(isOperator("+"));
    REQUIRE(isOperator("-"));
    REQUIRE(isOperator("*"));
    REQUIRE(isOperator("/"));
    REQUIRE(isOperator("%"));
    REQUIRE(isOperator("("));
    REQUIRE(isOperator(")"));

    REQUIRE(!isOperator("$"));
    REQUIRE(!isOperator("()"));
    REQUIRE(!isOperator("+abv"));
}

TEST_CASE("Check if binary operator")
{
    // This function is used only for throwing an error when there's
    // two consecutive binary operators in the input 
    REQUIRE(isBinaryOperator('+'));
    REQUIRE(isBinaryOperator('-'));
    REQUIRE(isBinaryOperator('/'));
    REQUIRE(isBinaryOperator('%'));
    REQUIRE(isBinaryOperator('/'));

    REQUIRE(!isBinaryOperator('('));
    REQUIRE(!isBinaryOperator(')'));
}

TEST_CASE("Check if function definition")
{
    // Function definitions should only be initialized with the 
    // parameter name in square brackets
    REQUIRE(isFunctionDefinition("ABC[param]"));

    // A function can be defined without a parameter - 
    // only evaluation to the expression assigned to it
    REQUIRE(isFunctionDefinition("ABC[]"));

    REQUIRE(!isFunctionDefinition("ABCparam]"));

    REQUIRE(!isFunctionDefinition("Abc[param]"));
    REQUIRE(!isFunctionDefinition("A12[param]"));

    REQUIRE(!isFunctionDefinition("ABC[param]a"));
    REQUIRE(!isFunctionDefinition("ABC[213abv]"));

    // Technically passes as a function definition by these rules but
    // will result in a syntax error
    REQUIRE(isFunctionDefinition("[]"));

}

TEST_CASE("Check if function call")
{
    REQUIRE(isFunctionCall("FUN[a]"));
    REQUIRE(isFunctionCall("FUN[a+2]"));

    // The argument of the function call is evaluated afterwards,
    // so this will throw an exception only when it gets to evaluating it
    REQUIRE(isFunctionCall("FUN[aa++382ddc]"));

    REQUIRE(!isFunctionCall("FUNabv]"));
    REQUIRE(!isFunctionCall("FUn[234]"));
    REQUIRE(!isFunctionCall("FUN[abc/4]-df"));

    REQUIRE(isFunctionCall("FUN[ A[a+2]/ B[3-2] ]"));
}

TEST_CASE("Get function call's closing bracket position")
{
    REQUIRE(getFunctionClosingBracket("FUN[3+2*4]*(2+3)+abc") == 9);
    REQUIRE(getFunctionClosingBracket("FUN[A[3-2]+B[12/2]]*4/2") == 18);

    // Missing opening bracket
    REQUIRE_THROWS_WITH(getFunctionClosingBracket("FUNabc]+3"), "Function opening bracket '[' cannot be found.\nSyntax Error at Line #");

    // Unbalanced amount of square brackets
    REQUIRE_THROWS_WITH(getFunctionClosingBracket("FUN[A[3-2+B[12/2]]*4/2"), "Unbalanced function brackets.\nSyntax Error at Line #");
}

TEST_CASE("Check if parentheses are balanced")
{
    REQUIRE(hasBalancedParentheses("((D[(2)])+2)"));

    REQUIRE(!hasBalancedParentheses(")(D[23]+4)*3"));
    REQUIRE(!hasBalancedParentheses("(FUN[(d)]+3)*3)"));
}

// Generalizes input, separating every element with a space
TEST_CASE("Parse expression from input")
{
    SECTION("Valid input - 1")
    {
        stringstream ss("1234 /a + 2*b% ABC[c]");

        // Variables and function definitions should be saved to be recognized when parsing
        SavedExpressions::getInstance()->saveVariable("a", 0);
        SavedExpressions::getInstance()->saveVariable("b", 0);
        SavedExpressions::getInstance()->saveVariable("c", 0);

        SavedExpressions::getInstance()->saveFunction("ABC", nullptr);

        parseExpressionString(ss);

        REQUIRE(ss.str() == "1234 / a + 2 * b % ABC[c] ");

        SavedExpressions::getInstance()->clearSavedVariables();
        SavedExpressions::getInstance()->clearSavedFunctions();

    }

    SECTION("Valid input - 2")
    {
        stringstream ss("ABC[(A[a+2]+B[23])*2]/2");

        // Only functions need to be defined, because variables used as
        // function arguments will be parsed later on when building
        // the expression tree
        SavedExpressions::getInstance()->saveFunction("ABC", nullptr);
        SavedExpressions::getInstance()->saveFunction("A", nullptr);
        SavedExpressions::getInstance()->saveFunction("B", nullptr);

        parseExpressionString(ss);

        REQUIRE(ss.str() == "ABC[(A[a+2]+B[23])*2] / 2 ");

        SavedExpressions::getInstance()->clearSavedFunctions();
    }

    SECTION("Invalid input - 1")
    {
        // Parsing an empty string throws an exception
        stringstream ss("");

        REQUIRE_THROWS_AS(parseExpressionString(ss), std::invalid_argument);

        ss = stringstream(" ");

        REQUIRE_THROWS_AS(parseExpressionString(ss), std::invalid_argument);
    }

    SECTION("Invalid input - 2")
    {
        // A string cannot begin with a binary operator
        stringstream ss("+34*20/(4-2)");

        REQUIRE_THROWS_WITH(parseExpressionString(ss), "Expression cannot begin with a binary operator.\nSyntax Error at Line #");
    }

    SECTION("Invalid input - 3")
    {
        // A string cannot be parsed if it contains undefined variables
        stringstream ss(" 34  *  (a-2) +b");

        SavedExpressions::getInstance()->saveVariable("b", 0);

        REQUIRE_THROWS_WITH(parseExpressionString(ss), "Unrecognized variable name.\nSyntax Error at Line #");

        SavedExpressions::getInstance()->clearSavedVariables();
    }

    SECTION("Invalid input - 4")
    {
        stringstream ss("Aabc]*( 23-  10 ) ");

        SavedExpressions::getInstance()->saveFunction("A", nullptr);
        SavedExpressions::getInstance()->saveVariable("abc", 0);

        // Function missing an opening bracket
        REQUIRE_THROWS_AS(parseExpressionString(ss), std::invalid_argument);

        SavedExpressions::getInstance()->clearSavedVariables();
        SavedExpressions::getInstance()->clearSavedFunctions();
    }

    SECTION("Invalid input - 5")
    {   
        // Undefined nested functions (for example: "B") do not need to be defined now
        // because they will be parsed later on when building the expression tree
        stringstream ss("A[B[3+2]* 2]/ C[7]");

        SavedExpressions::getInstance()->saveFunction("A", nullptr);

        REQUIRE_THROWS_WITH(parseExpressionString(ss), "Unrecognized function name.\nSyntax Error at Line #");

        SavedExpressions::getInstance()->clearSavedFunctions();
    }


    SECTION("Invalid input - 6")
    {
        // Two consecutive binary operators are not allowed (example: "+-")
        stringstream ss("23* abc+-4*(9 - 3)");

        SavedExpressions::getInstance()->saveVariable("abc", 0);

        REQUIRE_THROWS_WITH(parseExpressionString(ss), "Expression cannot have two consecutive operators.\nSyntax Error at Line #");

        SavedExpressions::getInstance()->clearSavedVariables();
    }

    
    SECTION("Valid input - 3")
    {
        // Previous section does not count for parentheses
        stringstream ss("(((B[a+2])/2)* 32)");

        SavedExpressions::getInstance()->saveFunction("B", nullptr);

        parseExpressionString(ss);

        REQUIRE(ss.str() == "( ( ( B[a+2] ) / 2 ) * 32 ) ");

        SavedExpressions::getInstance()->clearSavedFunctions();
    }

    SECTION("Invalid input - 7")
    {
        stringstream ss("B[[abc] + 3* 4");

        SavedExpressions::getInstance()->saveFunction("B", nullptr);

        // Unrecognized character '[' is reached
        REQUIRE_THROWS_AS(parseExpressionString(ss), std::invalid_argument);

        ss = stringstream("23+   45/ $");

        REQUIRE_THROWS_AS(parseExpressionString(ss), std::invalid_argument);

        SavedExpressions::getInstance()->clearSavedFunctions();
    }

    SECTION("Invalid input - 8")
    {
        stringstream ss("((D[2]))+2)");

        SavedExpressions::getInstance()->saveFunction("D", nullptr);

        REQUIRE_THROWS_WITH(parseExpressionString(ss), "Unbalanced parentheses.\nSyntax Error at Line #");

        ss = stringstream("((D[(2)])+2)");

        parseExpressionString(ss);

        REQUIRE(ss.str() == "( ( D[(2)] ) + 2 ) ");

        SavedExpressions::getInstance()->clearSavedFunctions();
    }

}

TEST_CASE("Convert parsed expression to postfix notation")
{
    // It is assumed that input has already been parsed with the 
    // parseExpressionString() function so in convertExpressionToPostfix()
    // variables and functions are never checked if they are saved (it's already been done)

    SECTION("Parse to postfix - 1")
    {
        stringstream ss("( ( ( B[a+2] ) / ( 2 + 3 ) ) * 32 ) ");

        convertExpressionToPostfix(ss);

        REQUIRE(ss.str() == "B[a+2] 2 3 + / 32 * ");    

        SavedExpressions::getInstance()->clearSavedFunctions();
    }

    SECTION("Parse to postfix - 2")
    {
        stringstream ss("( abc + 32 * 4 ) / 12 % cba / ( D[2] - 1 ) ");

        SavedExpressions::getInstance()->saveFunction("D", nullptr);
        SavedExpressions::getInstance()->saveVariable("abc", 0);
        SavedExpressions::getInstance()->saveVariable("cba", 0);

        convertExpressionToPostfix(ss);

        REQUIRE(ss.str() == "abc 32 4 * + 12 / cba % D[2] 1 - / ");

        SavedExpressions::getInstance()->clearSavedVariables();
        SavedExpressions::getInstance()->clearSavedFunctions();
    }
}

TEST_CASE("Building expression trees and returning the root")
{

    SECTION("Valid input")
    {
        stringstream ss("FUN[var+3] 2 / 32 * ");

        // Initializing FUN function definition
        ExpressionConst* constValue2 = new ExpressionConst(2);
        ExpressionVar* functionParam = new ExpressionVar("x");
        ExpressionBinaryOperator* divisionOperation = new ExpressionBinaryOperator('/', constValue2, functionParam);
        ExpressionFunctionDefinition* functionDefinition = new ExpressionFunctionDefinition("FUN", "x", divisionOperation);

        // Saving function and variable names
        SavedExpressions::getInstance()->saveFunction("FUN", functionDefinition);
        SavedExpressions::getInstance()->saveVariable("var", 4);

        // Initializing function call argument
        ExpressionVar* savedVar = new ExpressionVar("var");
        ExpressionConst* constValue3 = new ExpressionConst(3);
        ExpressionBinaryOperator* additionOperation = new ExpressionBinaryOperator('+', savedVar, constValue3);

        // Initializing function call
        ExpressionFunctionCall* functionCall = new ExpressionFunctionCall("FUN", additionOperation);

        // Initializing division operation (FUN[] / 2)
        ExpressionBinaryOperator* divisionOperation2 = new ExpressionBinaryOperator('/', functionCall, constValue2);

        // Initializing multiplication (... * 32)
        ExpressionConst* constValue32 = new ExpressionConst(32);
        ExpressionBinaryOperator* multiplicationOperation = new ExpressionBinaryOperator('*', divisionOperation2, constValue32);


        REQUIRE(buildExpressionTree(ss)->getValue() == multiplicationOperation->getValue());

        SavedExpressions::getInstance()->clearSavedFunctions();
        SavedExpressions::getInstance()->clearSavedVariables();
    }

    SECTION("Invalid input - 1")
    {
        stringstream ss("FUN[a+2] 2 / 32 * ");

        SavedExpressions::getInstance()->saveFunction("FUN", nullptr);

        // Function argument parsing reaches an undefined variable ("a")
        // and throws an error
        REQUIRE_THROWS_WITH(buildExpressionTree(ss), "Unrecognized function arguments.\nSyntax Error at Line #");

        SavedExpressions::getInstance()->clearSavedFunctions();
    }

    SECTION("Invalid input - 2")
    {
        stringstream ss("45 2 * 3 + 4 / 8 ");

        // At the end of the stringstream the constant 8 gets pushed into the stack.
        // At that point the stack has more than one element - the root of the expression tree and 
        // a ExpressionConst node. An exception is throw as the stream doesn't represent a single expression.
        REQUIRE_THROWS_WITH(buildExpressionTree(ss), "Missing operator.\nSyntax Error at Line #");
    }
}
