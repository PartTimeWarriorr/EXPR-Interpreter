#pragma once
#include "expressions.h"

class SavedExpressions
{
    private:
    static SavedExpressions* instancePointer;

    unordered_map<string, int> savedVariableNames;
    unordered_map<string, ExpressionFunctionDefinition*> savedFunctionDefinitions;

    SavedExpressions();

    public:
    SavedExpressions(const SavedExpressions& other) = delete;

    static SavedExpressions* getInstance();

    void saveVariable(string, const Expression*&);
    void saveVariable(string, int);

    int getSavedVariableValue(string);

    bool isSavedVariable(string);
    bool isSavedFunctionDefinition(string);
    bool isFunctionParameterName(string, string);

    void saveFunction(string name, ExpressionFunctionDefinition* definition);
    // int getSavedFunctionValue(string name, int argument);
    ExpressionFunctionDefinition* getSavedFunctionBody(string);

    static string currentParameter;
    static int currentArgument;

    void printSavedFunctions();
};
