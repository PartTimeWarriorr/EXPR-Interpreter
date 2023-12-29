#pragma once
#include "expressions.h"
#include <fstream>
#include <sstream>
#include <cassert>

using std::ifstream;
using std::stringstream;
using std::stoi;

bool isReservedWord(string name);
bool isVariableName(string name);
bool isConstant(string name);
Expression* buildExpressionTree(Expression*& root);
void parseEXPRFile(string fileName);

