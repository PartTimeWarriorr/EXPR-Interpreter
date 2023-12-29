#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include "arithmetics.h"
#include "saved-expressions.h"

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

class ExpressionConst : public Expression
{
    public:
    ExpressionConst* clone();
    int getValue() const;

    ExpressionConst(int value);

    virtual ~ExpressionConst() override = default;

    private:
    int value;
};

class ExpressionVar : public Expression
{
    public:
    ExpressionVar* clone();
    int getValue() const;

    string getName() const;

    void assignValue(const Expression*&);

    ExpressionVar(string);

    virtual ~ExpressionVar() override = default;

    private:
    string name;
};

class ExpressionBinaryOperator : public Expression
{
    public:
    ExpressionBinaryOperator* clone();
    int getValue() const;

    private:
    Expression* left;
    Expression* right;

    char symbol;
};

class ExpressionAssignmentOperator : public Expression
{
    public:
    ExpressionAssignmentOperator* clone();
    int getValue() const;

    void assignValueToVar();

    ExpressionAssignmentOperator(ExpressionVar* var, Expression* value);

    virtual ~ExpressionAssignmentOperator() override;

    private:
    ExpressionVar* left;
    const Expression* right;
};

class ExpressionRead : public Expression
{
    public:
    ExpressionRead* clone();
    int getValue() const;

    void readValueFromInput() const;

    ExpressionRead(ExpressionVar* toBeRead);

    virtual ~ExpressionRead() override;

    private:
    const ExpressionVar* varToReceiveValue;
};

class ExpressionPrint : public Expression
{
    public:
    ExpressionPrint* clone();
    int getValue() const;

    void printTree();

    ExpressionPrint(Expression* expressionToBePrinted);

    virtual ~ExpressionPrint() override;

    private:
    const Expression* toBePrinted;
};


class ExpressionFunctionDefinition : public Expression
{

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