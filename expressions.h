#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include "arithmetics.h"

using std::string;
using std::unordered_map;
using std::cin;
using std::cout;

class Expression
{
    public:
    virtual Expression* clone() = 0;
    virtual int getValue() const = 0;

    virtual ~Expression() = default;
    
};

class ExpressionFunctionDefinition : public Expression
{

};

unordered_map<string, int> savedVariableNames = {{"aaa", 24}, {"bbb", 12}};
unordered_map<string, const ExpressionFunctionDefinition*> savedFunctionDefinitions;


class ExpressionConst : public Expression
{
    public:
    ExpressionConst* clone()
    {
        return new ExpressionConst(*this);
    }

    int getValue() const
    {
        return value;
    }

    ExpressionConst(int value) : value(value) {}

    virtual ~ExpressionConst() = default;

    private:
    int value;
};

class ExpressionVar : public Expression
{
    public:
    ExpressionVar* clone()
    {
        return new ExpressionVar(*this);
    }

    int getValue() const
    {
        return savedVariableNames[name];
    }

    string getName() const
    {
        return name;
    }

    void assignValue(const Expression*& expression)
    {
        savedVariableNames[name] = expression->getValue();
    }

    ExpressionVar(string name) : name(name) {}

    virtual ~ExpressionVar() = default;

    private:
    string name;
};

class ExpressionBinaryOperator : public Expression
{
    public:
    ExpressionBinaryOperator* clone();

    int getValue() const
    {
        return computeExpression(symbol)(left->getValue(), right->getValue());  
    }

    private:
    Expression* left;
    Expression* right;

    char symbol;
};

class ExpressionAssignmentOperator : public Expression
{
    public:
    ExpressionAssignmentOperator* clone()
    {
        return new ExpressionAssignmentOperator(*this);
    }

    int getValue() const
    {
        return right->getValue();
    }

    void assignValueToVar()
    {
        left->assignValue(right);
    }

    ExpressionAssignmentOperator(ExpressionVar* var, Expression* value) 
    {
        left = var->clone();
        right = value->clone();
    }

    virtual ~ExpressionAssignmentOperator()
    {
        delete left;
        delete right;
    }

    private:
    ExpressionVar* left;
    const Expression* right;
};

class ExpressionRead : public Expression
{
    public:
    ExpressionRead* clone()
    {
        return new ExpressionRead(*this);
    }

    void readValueFromInput() const
    {
        int buffer;
        cin >> buffer;
        savedVariableNames[varToReceiveValue->getName()] = buffer;
    }

    int getValue() const
    {
        return varToReceiveValue->getValue();
    }

    ExpressionRead(ExpressionVar* toBeRead)
    {
        varToReceiveValue = toBeRead->clone();
    }

    virtual ~ExpressionRead() override
    {
        delete varToReceiveValue;
    }

    private:
    const ExpressionVar* varToReceiveValue;
};

class ExpressionPrint : public Expression
{
    public:
    ExpressionPrint* clone()
    {
        return new ExpressionPrint(*this);
    }

    int getValue() const
    {
        return toBePrinted->getValue();
    }

    void printTree()
    {
        cout << toBePrinted->getValue() << '\n';
    }

    ExpressionPrint(Expression* expressionToBePrinted)
    {
        toBePrinted = expressionToBePrinted->clone();
    }

    virtual ~ExpressionPrint() override
    {
        delete toBePrinted;
    }

    private:
    const Expression* toBePrinted;
};


class ExpressionFunctionCall : public Expression
{

};

// bonus

// class ExpressionIfStatement : public Expression
// {

// };

// class ExpressionForCycle : public Expression
// {

// };