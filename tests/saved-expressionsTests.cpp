#include "../include/saved-expressions.h"
#include "catch2/catch_all.hpp"

TEST_CASE("Singleton class initialization")
{
    // Both instances should be equal since this is a singleton class
    SavedExpressions* instance1 = SavedExpressions::getInstance();
    SavedExpressions* instance2 = SavedExpressions::getInstance();

    REQUIRE(instance1 == instance2);
}

TEST_CASE("Saving variable")
{
    SavedExpressions::getInstance()->saveVariable("var", 40);

    REQUIRE(SavedExpressions::getInstance()->isSavedVariable("var"));
    // Reset saved variable map
    SavedExpressions::getInstance()->clearSavedVariables();
}

TEST_CASE("Getting saved variable value")
{
    // Before saving variable in map it should return 0
    REQUIRE(SavedExpressions::getInstance()->getSavedVariableValue("var") == 0);
    
    SavedExpressions::getInstance()->saveVariable("var", 56);
    REQUIRE(SavedExpressions::getInstance()->getSavedVariableValue("var") == 56);

    // Redefining variable should change its value to the new one
    SavedExpressions::getInstance()->saveVariable("var", 9);
    REQUIRE(SavedExpressions::getInstance()->getSavedVariableValue("var") == 9);

    SavedExpressions::getInstance()->saveVariable("name", 11);
    REQUIRE(SavedExpressions::getInstance()->getSavedVariableValue("name") == 11);

    SavedExpressions::getInstance()->clearSavedVariables();
}

TEST_CASE("Check if variable is saved in map")
{   
    // Returns false when variable name isn't present in map
    REQUIRE(!SavedExpressions::getInstance()->isSavedVariable("var"));

    SavedExpressions::getInstance()->saveVariable("var", 1);

    REQUIRE(SavedExpressions::getInstance()->isSavedVariable("var"));

    SavedExpressions::getInstance()->clearSavedVariables();
}

TEST_CASE("Check if string is parameter name of corresponding function")
{
    ExpressionConst* functionBody = new ExpressionConst(54);
    ExpressionFunctionDefinition functionDefinition("FUN", "param", functionBody);
    functionDefinition.assignFunction();

    REQUIRE(SavedExpressions::getInstance()->isFunctionParameterName("FUN", "param"));
    REQUIRE(!SavedExpressions::getInstance()->isFunctionParameterName("FUN", "other_name"));

    // Reset saved function map
    SavedExpressions::getInstance()->clearSavedFunctions();
}

TEST_CASE("Saving function definition")
{
    REQUIRE(!SavedExpressions::getInstance()->isSavedFunctionDefinition("FUN"));
    
    ExpressionConst* functionBody = new ExpressionConst(54);
    ExpressionFunctionDefinition* functionDefinition = new ExpressionFunctionDefinition("FUN", "param", functionBody);
    
    SavedExpressions::getInstance()->saveFunction("FUN", functionDefinition);
    REQUIRE(SavedExpressions::getInstance()->isSavedFunctionDefinition("FUN"));

    SavedExpressions::getInstance()->clearSavedFunctions();
}   

TEST_CASE("Getting saved function body")
{
    ExpressionConst* functionBody = new ExpressionConst(21);
    ExpressionFunctionDefinition* functionDefinition = new ExpressionFunctionDefinition("FUN", "param", functionBody);
    
    SavedExpressions::getInstance()->saveFunction("FUN", functionDefinition);
    
    REQUIRE(SavedExpressions::getInstance()->getSavedFunctionBody("FUN")->getValue() == functionDefinition->getValue());
    REQUIRE(SavedExpressions::getInstance()->getSavedFunctionBody("FUN")->getName() == functionDefinition->getName());
    REQUIRE(SavedExpressions::getInstance()->getSavedFunctionBody("FUN")->getParameterName() == functionDefinition->getParameterName());

    SavedExpressions::getInstance()->clearSavedFunctions();
}

TEST_CASE("Argument stack tests - 1")
{ 
    // Argument stack should be empty by default  
    REQUIRE(SavedExpressions::getInstance()->isEmptyArgumentStack());

    SavedExpressions::getInstance()->pushArgumentStack("param", 8);

    REQUIRE(SavedExpressions::getInstance()->topArgumentStack().first == "param");
    REQUIRE(SavedExpressions::getInstance()->topArgumentStack().second == 8);
    REQUIRE(!SavedExpressions::getInstance()->isEmptyArgumentStack());

    SavedExpressions::getInstance()->pushArgumentStack("param2", 5);

    REQUIRE(SavedExpressions::getInstance()->topArgumentStack().first == "param2");
    REQUIRE(SavedExpressions::getInstance()->topArgumentStack().second == 5);
    
    SavedExpressions::getInstance()->popArgumentStack();

    REQUIRE(SavedExpressions::getInstance()->topArgumentStack().first == "param");
    REQUIRE(SavedExpressions::getInstance()->topArgumentStack().second == 8);
    REQUIRE(!SavedExpressions::getInstance()->isEmptyArgumentStack());

    SavedExpressions::getInstance()->popArgumentStack();

    REQUIRE(SavedExpressions::getInstance()->isEmptyArgumentStack());

}   

