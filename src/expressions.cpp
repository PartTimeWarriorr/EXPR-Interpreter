#include "../include/expressions.h"
#include "../include/saved-expressions.h"
#include "../include/interpreter.h"

// Const
ExpressionConst* ExpressionConst::clone()
{
    return new ExpressionConst(*this);
}

int ExpressionConst::getValue() const
{
    return value;
}

ExpressionConst::ExpressionConst(int value) : value(value) {}

// Var
ExpressionVar* ExpressionVar::clone()
{
    return new ExpressionVar(*this);
}

int ExpressionVar::getValue() const
{
    // If the argument stack is not empty and the parameter at the top of the stack has this variable's 
    // name, return the argument at the top of the stack. Argument stack is used when evaluating 
    // function calls to recognize the ExpressionVar node as a parameter.
    if(!SavedExpressions::getInstance()->isEmptyArgumentStack() && SavedExpressions::getInstance()->topArgumentStack().first == this->name)
    {
        return SavedExpressions::getInstance()->topArgumentStack().second;
    }

    return SavedExpressions::getInstance()->getSavedVariableValue(name);
}

string ExpressionVar::getName() const
{
    return name;
}

void ExpressionVar::assignValue(const Expression*& expression)
{
    SavedExpressions::getInstance()->saveVariable(name, expression);
}

ExpressionVar::ExpressionVar(string name) : name(name) {}

// Binary Operator
ExpressionBinaryOperator* ExpressionBinaryOperator::clone()
{
    return new ExpressionBinaryOperator(*this);
}

int ExpressionBinaryOperator::getValue() const
{   
    // Checks for division by zero.
    if(symbol == '/' && right->getValue() == 0)
    {
        throw std::invalid_argument("Logical Error: Division by 0 at Line #");
    }

    return computeExpression(symbol)(left->getValue(), right->getValue());
}

ExpressionBinaryOperator::ExpressionBinaryOperator(char symbol) : 
symbol(symbol), left(nullptr), right(nullptr) {} 

ExpressionBinaryOperator::ExpressionBinaryOperator(char symbol, Expression* left, Expression* right) :
symbol(symbol)
{
    this->left = left->clone();
    this->right = right->clone();
}

ExpressionBinaryOperator::~ExpressionBinaryOperator()
{
    delete left;
    delete right;
}

// Assignment Operator
ExpressionAssignmentOperator* ExpressionAssignmentOperator::clone()
{
    return new ExpressionAssignmentOperator(*this);
}

int ExpressionAssignmentOperator::getValue() const
{
    return right->getValue();
}

void ExpressionAssignmentOperator::assignValueToVar()
{
    left->assignValue(right);
}

ExpressionAssignmentOperator::ExpressionAssignmentOperator(ExpressionVar* var, Expression* value) 
{
    left = var->clone();
    right = value->clone();
}

ExpressionAssignmentOperator::~ExpressionAssignmentOperator()
{
    delete left;
    delete right;
}

// Read Keyword
ExpressionRead* ExpressionRead::clone()
{
    return new ExpressionRead(*this);
}

void ExpressionRead::readValueFromInput() const
{
    string buffer;
    cin >> buffer;
    cin.ignore();
    if(!isConstant(buffer))
    {
        throw std::invalid_argument("Invalid input: Read variable value must be a number. Error at Line #");
    }
    SavedExpressions::getInstance()->saveVariable(varToReceiveValue->getName(), stoi(buffer));
}

int ExpressionRead::getValue() const
{
    return varToReceiveValue->getValue();
}

ExpressionRead::ExpressionRead(ExpressionVar* toBeRead)
{
    varToReceiveValue = toBeRead->clone();
}

ExpressionRead::~ExpressionRead()
{
    delete varToReceiveValue;
}

// Print Keyword
ExpressionPrint* ExpressionPrint::clone()
{
    return new ExpressionPrint(*this);
}

int ExpressionPrint::getValue() const
{
    return toBePrinted->getValue();
}

void ExpressionPrint::printTree()
{
    cout << toBePrinted->getValue() << '\n';
}

ExpressionPrint::ExpressionPrint(Expression* expressionToBePrinted)
{
    toBePrinted = expressionToBePrinted->clone();
}

ExpressionPrint::~ExpressionPrint()
{
    delete toBePrinted;
}

// Function Definition
ExpressionFunctionDefinition* ExpressionFunctionDefinition::clone()
{
    return new ExpressionFunctionDefinition(*this);
}

int ExpressionFunctionDefinition::getValue() const
{
    return functionBody->getValue();
}

ExpressionFunctionDefinition::ExpressionFunctionDefinition(string name, string parameterName, Expression* functionBody) :
name(name), parameterName(parameterName)
{
    this->functionBody = functionBody->clone();
}


string ExpressionFunctionDefinition::getName() const
{
    return name;
}

string ExpressionFunctionDefinition::getParameterName() const
{
    return parameterName;
}

Expression* ExpressionFunctionDefinition::getFunctionBody() const
{
    return this->functionBody;
}

ExpressionFunctionDefinition::~ExpressionFunctionDefinition() 
{
    delete functionBody;
}

void ExpressionFunctionDefinition::assignFunction()
{
    SavedExpressions::getInstance()->saveFunction(name, this->clone());
}

// Function Call
ExpressionFunctionCall* ExpressionFunctionCall::clone()
{
    return new ExpressionFunctionCall(*this);
}

int ExpressionFunctionCall::getValue() const
{
    // The function call refers to the saved function definition with the same name. 
    // It pushes the parameter of the function definition to the stack so that it is recognized when
    // evaluating the function call. After getting the result the parameter is popped.
    // Works recursively with nested functions.
    ExpressionFunctionDefinition* functionDefinition = SavedExpressions::getInstance()->getSavedFunctionBody(this->name);
    
    SavedExpressions::getInstance()->pushArgumentStack(functionDefinition->getParameterName(), argument->getValue());

    int result = functionDefinition->getValue();

    SavedExpressions::getInstance()->popArgumentStack();

    return result;
}

ExpressionFunctionCall::ExpressionFunctionCall(string name, Expression* argument) : 
name(name)
{
    this->argument = argument->clone();
}

ExpressionFunctionCall::~ExpressionFunctionCall()
{
    delete argument;
}

