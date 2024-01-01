#pragma once
#include "saved-expressions.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstddef>
#include <queue>
#include <stack>

using std::ifstream;
using std::stringstream;
using std::stoi;
using std::size_t;
using std::queue;
using std::stack;

bool isReservedWord(string name);
bool isVariableName(string name);
bool isFunctionDefinition(string name);
bool isConstant(string name);
Expression* buildExpressionTree(stringstream& ss);
stringstream parseExpressionString(stringstream& ss);
stringstream convertExpressionToPostfix(stringstream& ss);
void parseEXPRFile(string fileName);

