#pragma once
#include "saved-expressions.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstddef>

using std::ifstream;
using std::stringstream;
using std::stoi;
using std::size_t;

bool isReservedWord(string name);
bool isVariableName(string name);
bool isFunctionDefinition(string name);
bool isConstant(string name);
Expression* buildExpressionTree(Expression*& root);
stringstream parseExpressionString(stringstream& ss);
void parseEXPRFile(string fileName);

