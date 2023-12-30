#pragma once
#include <functional>
#include <cmath>
#include <string>

using std::string;

int getOperatorPriority(char symbol);
int getOperatorPriority(string symbol);
std::function<int(int, int)> computeExpression(char symbol);

