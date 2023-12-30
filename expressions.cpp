#include "expressions.h"
#include "saved-expressions.h"

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
    return computeExpression(symbol)(left->getValue(), right->getValue());
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
    int buffer;
    cin >> buffer;
    SavedExpressions::getInstance()->saveVariable(varToReceiveValue->getName(), buffer);
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

