#include "../include/expressions.h"
#include "../include/saved-expressions.h"
#include "catch2/catch_all.hpp"
#include <sstream>

TEST_CASE("ExpressionConst - clone function")
{
    // Initializing object and clone
    ExpressionConst expr(12);
    ExpressionConst* cloned = expr.clone();

    // Cloned object should not be the same as the original
    REQUIRE(cloned != &expr);
    REQUIRE(cloned->getValue() == expr.getValue());

    delete cloned;
}

TEST_CASE("ExpressionConst - get value")
{
    ExpressionConst expr(123);

    REQUIRE(expr.getValue() == 123);
}

TEST_CASE("ExpressionVar - clone function")
{
    ExpressionVar expr("name");
    ExpressionVar* cloned = expr.clone();

    REQUIRE(cloned != &expr);
    REQUIRE(cloned->getName() == expr.getName());

    delete cloned;
}

TEST_CASE("ExpressionVar - get name")
{
    ExpressionVar expr("name");

    REQUIRE(expr.getName() == "name");
}

TEST_CASE("ExpressionBinaryOperator - constructor and get value")
{
    ExpressionConst* leftChild = new ExpressionConst(15);
    ExpressionConst* rightChild = new ExpressionConst(3);

    // Initializing objects representing two different binary operations
    ExpressionBinaryOperator* expr = new ExpressionBinaryOperator('/', leftChild, rightChild);
    ExpressionBinaryOperator* expr2 = new ExpressionBinaryOperator('-', leftChild, rightChild);

    REQUIRE(expr->getValue() == 5);
    REQUIRE(expr2->getValue() == 12);

    delete leftChild;
    delete rightChild;
    delete expr;
    delete expr2;
}

TEST_CASE("ExpressionBinaryOperator - get value when dividing by 0")
{
    ExpressionConst* leftChild = new ExpressionConst(12);
    ExpressionConst* rightChild = new ExpressionConst(0);

    // Initializing object with a division ('/') symbol where the right child evaluates to 0
    ExpressionBinaryOperator* expr = new ExpressionBinaryOperator('/', leftChild, rightChild);

    REQUIRE_THROWS_AS(expr->getValue(), std::invalid_argument);

    delete leftChild;
    delete rightChild;
    delete expr;
}

TEST_CASE("ExpressionBinaryOperator - clone function")
{
    ExpressionBinaryOperator expr('-');
    ExpressionBinaryOperator* cloned = expr.clone();

    REQUIRE(cloned != &expr);

    delete cloned;
}

TEST_CASE("ExpressionAssignmentOperator - get value")
{
    ExpressionVar* leftChild = new ExpressionVar("name");
    ExpressionConst* rightChild = new ExpressionConst(20);
    ExpressionAssignmentOperator expr(leftChild, rightChild);

    REQUIRE(expr.getValue() == 20);

    delete leftChild;
    delete rightChild;
}

TEST_CASE("ExpressionAssignmentOperator - clone function")
{
    ExpressionVar* leftChild = new ExpressionVar("name");
    ExpressionConst* rightChild = new ExpressionConst(18);
    ExpressionAssignmentOperator expr(leftChild, rightChild);

    ExpressionAssignmentOperator* cloned = expr.clone();

    REQUIRE(cloned != &expr);
    REQUIRE(cloned->getValue() == expr.getValue());  

    delete leftChild;
    delete rightChild;
}

TEST_CASE("ExpressionAssignmentOperator - assigning value to variable")
{
    ExpressionVar* leftChild = new ExpressionVar("name");
    ExpressionConst* rightChild = new ExpressionConst(12);

    ExpressionAssignmentOperator expr(leftChild, rightChild);
    
    // Before the value is assigned to the variable it should not be in the savedVariables map
    REQUIRE(SavedExpressions::getInstance()->getSavedVariableValue("name") == 0);

    // The right child's evaluation is assigned to the left child variable
    expr.assignValueToVar();

    REQUIRE(SavedExpressions::getInstance()->getSavedVariableValue("name") == 12);

    delete leftChild;
    delete rightChild;

    SavedExpressions::getInstance()->saveVariable("name", 0);
}

TEST_CASE("ExpressionRead - clone")
{
    ExpressionVar* child = new ExpressionVar("name");
    ExpressionRead expr(child);

    ExpressionRead* cloned = expr.clone();

    REQUIRE(cloned != &expr);

    delete child; 
}

TEST_CASE("ExpressionRead - reading value from input")
{   
    // Redirecting standard input so that function can be tested using a different stream of characters
    std::stringstream input;

    std::streambuf* originalInput = cin.rdbuf();

    cin.rdbuf(input.rdbuf());

    ExpressionVar* child = new ExpressionVar("name");
    ExpressionRead expr(child);
    
    SECTION("Invalid Input")
    {
        // Input should not be assigned to variable as it is not a number
        input << "notanint\n";

        REQUIRE_THROWS_AS(expr.readValueFromInput(), std::invalid_argument);
        REQUIRE(SavedExpressions::getInstance()->getSavedVariableValue("name") == 0);

    }

    SECTION("Valid Input")
    {
        input << "24\n";

        expr.readValueFromInput();

        REQUIRE(SavedExpressions::getInstance()->getSavedVariableValue("name") == 24);
    }

    // Returning standard input to normal 
    cin.rdbuf(originalInput);
    delete child;

    SavedExpressions::getInstance()->saveVariable("name", 0);
}

TEST_CASE("ExpressionRead - get value")
{
    ExpressionVar* leftChild = new ExpressionVar("name");
    ExpressionRead expr(leftChild);

    ExpressionConst* rightChild = new ExpressionConst(18);
    ExpressionAssignmentOperator assign(leftChild, rightChild);
    assign.assignValueToVar();

    // ExpressionRead should return the value of its child
    REQUIRE(expr.getValue() == 18);

    delete leftChild;
    delete rightChild;
    
    SavedExpressions::getInstance()->saveVariable("name", 0);
}

TEST_CASE("ExpressionPrint - clone")
{
    ExpressionVar* child = new ExpressionVar("var");
    ExpressionPrint expr(child);

    ExpressionPrint* clone = expr.clone();

    REQUIRE(clone != &expr);

    delete child;
}

TEST_CASE("ExpressionPrint - get value")
{
    ExpressionConst* child = new ExpressionConst(14);
    ExpressionPrint expr(child);

    REQUIRE(expr.getValue() == 14);

    delete child;
}

// Essentially emulates what the ExpressionFunctionCall::getValue function does
TEST_CASE("ExpressionFunctionDefinition - get value")
{
    ExpressionConst* leftChild = new ExpressionConst(15);
    ExpressionVar* rightChild = new ExpressionVar("var");

    // Saves the parameter name as a variable so it can be recognized during evaluation
    SavedExpressions::getInstance()->saveVariable("var", 3);

    ExpressionBinaryOperator* expr = new ExpressionBinaryOperator('/', leftChild, rightChild);
    
    ExpressionFunctionDefinition* functionDef = new ExpressionFunctionDefinition("FUN", "var", expr);

    REQUIRE(functionDef->getValue() == 5);
    REQUIRE(functionDef->getValue() == expr->getValue());

    SavedExpressions::getInstance()->saveVariable("var", 0);

    delete functionDef;
}

TEST_CASE("ExpressionFunctionDefinition - getters") 
{
    ExpressionConst* functionBody = new ExpressionConst(2);
    ExpressionFunctionDefinition expr("FUN", "param", functionBody);

    REQUIRE(expr.getValue() == 2);
    REQUIRE(expr.getName() == "FUN");
    REQUIRE(expr.getParameterName() == "param");
    REQUIRE(expr.getFunctionBody()->getValue() == functionBody->getValue());

    delete functionBody;
}

TEST_CASE("ExpressionFunctionDefinition - clone")
{
    ExpressionConst* functionBody = new ExpressionConst(6);
    ExpressionFunctionDefinition expr("FUN", "param", functionBody);

    ExpressionFunctionDefinition* cloned = expr.clone();

    REQUIRE(cloned->getName() == expr.getName());
    REQUIRE(cloned->getParameterName() == expr.getParameterName());
    REQUIRE(cloned->getFunctionBody() == expr.getFunctionBody());
    REQUIRE(cloned->getFunctionBody()->getValue() == expr.getFunctionBody()->getValue());

    delete functionBody;
}

TEST_CASE("ExpressionFunctionDefinition - assign function") 
{
    ExpressionConst* functionBody = new ExpressionConst(22);
    ExpressionFunctionDefinition expr("FUN", "param", functionBody);

    // Saves function definition in savedFunctionDefinitions map
    expr.assignFunction();

    REQUIRE(SavedExpressions::getInstance()->getSavedFunctionBody("FUN") != nullptr);
    REQUIRE(SavedExpressions::getInstance()->getSavedFunctionBody("FUN")->getValue() == 22);

    SavedExpressions::getInstance()->saveFunction("FUN", nullptr);
    REQUIRE(SavedExpressions::getInstance()->getSavedFunctionBody("FUN") == nullptr);

    delete functionBody;
}

TEST_CASE("ExpressionFunctionCall - getting value")
{
    ExpressionConst* leftChild = new ExpressionConst(15);
    ExpressionVar* rightChild = new ExpressionVar("param");

    ExpressionBinaryOperator* functionBody = new ExpressionBinaryOperator('/', leftChild, rightChild);
    
    // Assigns function definition so that it is recognized when evaluating the ExpressionFunctionCall
    ExpressionFunctionDefinition* functionDef = new ExpressionFunctionDefinition("FUN", "param", functionBody);
    functionDef->assignFunction();

    // Function should be saved in savedFunctionDefinitions
    REQUIRE(SavedExpressions::getInstance()->getSavedFunctionBody("FUN") != nullptr);

    ExpressionConst* argument = new ExpressionConst(5);
    ExpressionFunctionCall functionCall("FUN", argument);

    // Parameter name should not be saved as a variable (instead being saved in the parameter stack during evaluation)
    REQUIRE(!SavedExpressions::getInstance()->isSavedVariable("param"));
    REQUIRE(functionCall.getValue() == 3);

    SavedExpressions::getInstance()->saveFunction("FUN", nullptr);
    REQUIRE(SavedExpressions::getInstance()->getSavedFunctionBody("FUN") == nullptr);

    delete leftChild;
    delete rightChild;
    delete functionBody;
    delete argument;
}

TEST_CASE("ExpressionFunctionCall - clone")
{
    ExpressionConst* argument = new ExpressionConst(15);
    ExpressionFunctionCall expr("fun", argument);

    ExpressionFunctionCall* cloned = expr.clone();

    REQUIRE(cloned != &expr);
    
    delete argument;
}