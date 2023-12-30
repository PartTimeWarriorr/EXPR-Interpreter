#include "saved-expressions.h"

SavedExpressions::SavedExpressions() {}

SavedExpressions* SavedExpressions::getInstance()
{
    if(instancePointer == nullptr)
    {
        instancePointer = new SavedExpressions();
    }

    return instancePointer;
}

void SavedExpressions::saveVariable(string name, const Expression*& expression)
{
    savedVariableNames[name] = expression->getValue();
}

void SavedExpressions::saveVariable(string name, int value)
{
    savedVariableNames[name] = value;
}

int SavedExpressions::getSavedVariableValue(string name)
{
    return savedVariableNames[name];
}

bool SavedExpressions::isSavedVariable(string name)
{
    return savedVariableNames.count(name);
}
bool SavedExpressions::isSavedFunctionDefinition(string name)
{
    return savedFunctionDefinitions.count(name);
}

bool SavedExpressions::isFunctionParameterName(string name, string parameterName)
{
    return savedFunctionDefinitions[name]->getParameterName() == parameterName;    
}

void SavedExpressions::saveFunction(string name, const ExpressionFunctionDefinition* definition)
{
    savedFunctionDefinitions[name] = definition;
}

int SavedExpressions::getSavedFunctionValue(string name, int argument)
{
    // TODO
    return 0;
}

SavedExpressions* SavedExpressions::instancePointer = nullptr;