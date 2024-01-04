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

    ExpressionBinaryOperator(char symbol);
    ExpressionBinaryOperator(char symbol, Expression* left, Expression* right);

    virtual ~ExpressionBinaryOperator() override;

    public:
    // private:
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
    public:
    ExpressionFunctionDefinition* clone();
    int getValue() const;

    ExpressionFunctionDefinition(string name, string parameterName, Expression* functionBody);
    string getName() const;
    string getParameterName() const;
    Expression* getFunctionBody() const;

    virtual ~ExpressionFunctionDefinition() override;

    void assignFunction();

    private:
    string name;
    string parameterName;
    Expression* functionBody;
};


class ExpressionFunctionCall : public Expression
{
    public:
    ExpressionFunctionCall* clone();
    int getValue() const;

    ExpressionFunctionCall(string name, Expression* argument);

    virtual ~ExpressionFunctionCall() override;

    private:
    string name;
    const Expression* argument; 
};

// bonus

// class ExpressionIfStatement : public Expression
// {

// };

// class ExpressionForCycle : public Expression
// {

// };