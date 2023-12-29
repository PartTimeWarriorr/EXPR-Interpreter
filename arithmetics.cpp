#include "arithmetics.h"

int getOperatorPriority(char c)
{
    switch(c)
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