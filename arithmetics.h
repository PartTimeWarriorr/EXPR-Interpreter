#pragma once
#include <functional>
#include <cmath>

int getOperatorPriority(char c);
std::function<int(int, int)> computeExpression(char symbol);

