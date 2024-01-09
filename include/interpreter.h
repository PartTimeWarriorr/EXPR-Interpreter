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
bool isConstant(string name);

bool isOperator(char symbol);
bool isOperator(string symbol);
bool isBinaryOperator(char symbol);

bool isFunctionDefinition(string name);
bool isFunctionCall(string name);

size_t getFunctionClosingBracket(const string& name);

Expression* buildExpressionTree(stringstream& ss);
void parseExpressionString(stringstream& ss);
bool hasBalancedParentheses(const string& expression);
void convertExpressionToPostfix(stringstream& ss);
void parseEXPRFile(string fileName);

