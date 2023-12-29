#pragma once
#include <functional>
#include <cmath>

int getOperatorPriority(char c)
{
    switch(c)
    {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}

// int computeExpression(char oper, int left, int right)
// {

//     switch(oper)
//     {
//         case '+' : return left + right;
//         case '-' : return left - right;
//         case '*' : return left * right;
//         case '/' : return left / right;
//         case '^' : return pow(left, right);
//         default : return -1;
//     }
// }

std::function<int(int, int)> computeExpression(char symbol)
{
    switch(symbol)
    {
        case '+' : return [](int a, int b){ return a + b; };
        case '-' : return [](int a, int b){ return a - b; };
        case '*' : return [](int a, int b){ return a * b; };
        case '/' : return [](int a, int b){ return a / b; };
        case '^' : return [](int a, int b){ return pow(a, b); };
        default  : return [](int a, int b){ return -1; };
    }
}


