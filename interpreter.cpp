#include "interpreter.h"

bool isReservedWord(string name)
{
    if(name == "print" || name == "read")
    {
        return true;
    }   

    return false;
}

bool isVariableName(string name)
{   
    if(isReservedWord(name))
    {
        return false;
    }

    for(char c : name)
    {
        if(c < 'a' || c > 'z')
        {
            return false;
        }
    }

    return true;
}

bool isConstant(string name)
{
    for(char c : name)
    {
        if(c < '0' || c > '9')
        {
            return false;
        }
    }

    return true;
}

Expression* buildExpressionTree(Expression*& root)
{
    
}

void parseEXPRFile(string fileName)
{   
    ifstream file(fileName);
    string line = "";

    while(!file.eof())
    {
        getline(file, line);
        stringstream ss(line);
        string buffer;

        while(ss >> buffer)
        {
            if(isVariableName(buffer))
            {   
                string varName = buffer;
                ss >> buffer;
                assert(buffer == "=");

                ss >> buffer;
                int varValue = stoi(buffer);

                ExpressionAssignmentOperator assignment(new ExpressionVar(varName), new ExpressionConst(varValue));
                assignment.assignValueToVar();
            }

            if(isReservedWord(buffer))
            {
                if(buffer == "print")
                {   
                    ss >> buffer;
                    string toBePrinted = buffer;

                    if(isVariableName(toBePrinted))
                    {
                        ExpressionPrint print(new ExpressionVar(toBePrinted));
                        print.printTree();
                    }
                }

                if(buffer == "read")
                {
                    ss >> buffer;
                    string toBeRead = buffer;

                    ExpressionRead read(new ExpressionVar(toBeRead));
                    read.readValueFromInput();
                }
            }
        }
    }
}