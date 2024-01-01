#include "arithmetics.h"

int getOperatorPriority(char symbol)
{
    switch(symbol)
    {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}

int getOperatorPriority(string symbol)
{
    if(symbol == "+" || symbol == "-")
        return 1;
    else if(symbol == "*" || symbol == "/" || symbol == "%")
        return 2;
    else if(symbol == "^")
        return 3;
    else 
        return 0;
}

std::function<int(int, int)> computeExpression(char symbol)
{   
    switch(symbol)
    {
        case '+' : return [](int a, int b){ return a + b; };
        case '-' : return [](int a, int b){ return a - b; };
        case '*' : return [](int a, int b){ return a * b; };
        case '/' : return [](int a, int b){ return a / b; };
        case '%' : return [](int a, int b){ return a % b; };
        case '^' : return [](int a, int b){ return pow(a, b); };
        default  : return [](int a, int b){ return -1; };
    }
}