#include "../include/saved-expressions.h"
#include "../include/interpreter.h"

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

void SavedExpressions::saveFunction(string name, ExpressionFunctionDefinition* definition)
{
    savedFunctionDefinitions[name] = definition->clone();
    
}

ExpressionFunctionDefinition* SavedExpressions::getSavedFunctionBody(string name)
{
    return savedFunctionDefinitions[name];
}

SavedExpressions* SavedExpressions::instancePointer = nullptr;

//testing purposes only 
void SavedExpressions::printSavedVariables()
{
    for(auto var : savedVariableNames)
    {
        cout << var.first << ' ' << var.second << '\n';
    }
}

//testing purposes only 
void SavedExpressions::printSavedFunctions()
{
    for(auto fun : savedFunctionDefinitions)
    {
        cout << fun.first << ' ' << fun.second->getParameterName() << ' ' << (fun.second == nullptr) <<'\n';
    }
}

void SavedExpressions::popArgumentStack()
{
    functionArgumentStack.pop();
}

void SavedExpressions::pushArgumentStack(string parameter, int value)
{
    functionArgumentStack.push(pair<string, int>(parameter, value));
}

pair<string, int> SavedExpressions::topArgumentStack()
{
    return functionArgumentStack.top();
}

bool SavedExpressions::isEmptyArgumentStack()
{
    return functionArgumentStack.empty();
}
