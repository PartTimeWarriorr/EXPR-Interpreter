#include "saved-expressions.h"
#include "interpreter.h"

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

// int SavedExpressions::getSavedFunctionValue(string name, int argument)
// {
//     // TODO
//     SavedExpressions::currentParameter = savedFunctionDefinitions[name]->getParameterName();
//     SavedExpressions::currentArgument = argument;

// }

SavedExpressions* SavedExpressions::instancePointer = nullptr;

string SavedExpressions::currentParameter = "";
int SavedExpressions::currentArgument = 0;


//testing purposes only 
void SavedExpressions::printSavedFunctions()
{
    for(auto fun : savedFunctionDefinitions)
    {
        cout << fun.first << ' ' << fun.second->getParameterName() << ' ' << (fun.second == nullptr) <<'\n';
    }
}
