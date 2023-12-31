#pragma once
#include "expressions.h"
#include <stack>
#include <utility>

using std::stack;
using std::pair;

class SavedExpressions
{
    private:
    static SavedExpressions* instancePointer;

    unordered_map<string, int> savedVariableNames;
    unordered_map<string, ExpressionFunctionDefinition*> savedFunctionDefinitions;
    stack<pair<string, int>> functionArgumentStack;

    SavedExpressions();

    public:
    SavedExpressions(const SavedExpressions& other) = delete;

    static SavedExpressions* getInstance();

    void saveVariable(string, const Expression*&);
    void saveVariable(string, int);
    void saveFunction(string name, ExpressionFunctionDefinition* definition);

    int getSavedVariableValue(string);
    ExpressionFunctionDefinition* getSavedFunctionBody(string);

    bool isSavedVariable(string);
    bool isSavedFunctionDefinition(string);
    bool isFunctionParameterName(string, string);

    void printSavedVariables();
    void printSavedFunctions();

    void popArgumentStack();
    void pushArgumentStack(string, int);
    pair<string, int> topArgumentStack();
    bool isEmptyArgumentStack();

    void clearSavedVariables();
    void clearSavedFunctions();
};
