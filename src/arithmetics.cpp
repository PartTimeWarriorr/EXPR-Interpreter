#include "../include/arithmetics.h"

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
        default:
            return 0;
    }
}

int getOperatorPriority(string symbol)
{
    switch(symbol[0])
    {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        default:
            return 0;
    }
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
        default  : return [](int a, int b){ return -1; };
    }
}