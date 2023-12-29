#pragma once
#include "expressions.h"

class SavedExpressions
{
    private:
    static SavedExpressions* instancePointer;

    unordered_map<string, int> savedVariableNames;
    unordered_map<string, const ExpressionFunctionDefinition*> savedFunctionDefinitions;

    SavedExpressions();

    public:
    SavedExpressions(const SavedExpressions& other) = delete;

    static SavedExpressions* getInstance();

    void saveVariable(string, const Expression*&);
    void saveVariable(string, int);

    int getSavedVariableValue(string);

    // void saveFunction(string name, const ExpressionFunctionDefinition*& definition)
    // {
    //     return;
    // }

    // int getSavedFunctionValue(string name, int argument)
    // {
    //     return 0;
    // }
};
