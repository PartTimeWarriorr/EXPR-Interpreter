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
    
}