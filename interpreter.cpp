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

bool isOperator(char symbol)
{
    return symbol == '+' || 
           symbol == '-' || 
           symbol == '*' || 
           symbol == '%' || 
           symbol == '/' || 
           symbol == '^' || 
           symbol == '(' || 
           symbol == ')';
}

bool isOperator(string symbol)
{
    return symbol == "+" || 
           symbol == "-" || 
           symbol == "*" || 
           symbol == "%" || 
           symbol == "/" || 
           symbol == "^" || 
           symbol == "(" || 
           symbol == ")";
}

bool isFunctionDefinition(string name)
{
    size_t openPosition = name.find('[');

    if(openPosition == string::npos)
    {
        return false;
    }

    string functionName = name.substr(0, openPosition);

    for(char c : functionName)
    {
        if(c < 'A' || c > 'Z')
        {
            return false;
        }
    }

    size_t closePosition = name.find(']');

    if(closePosition != name.size() - 1)
    {
        return false;
    }

    string parameterName = name.substr(openPosition + 1, closePosition - openPosition - 1);

    if(!isVariableName(parameterName))
    {
        return false;
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
    return nullptr;
}

// Parse Expression to generalize all input, eg. a + 2*b% ABC[c] -> a + 2 * b % ABC[c]
stringstream parseExpressionString(stringstream& ss)
{
    string expressionString = "";
    string buffer;

    while(ss >> buffer)
    {
        expressionString += buffer;
    }

    if(isOperator(expressionString[0]))
    {
        cout << "Syntax Error at Line #1\n";
        return stringstream("");
    }

    stringstream expressionStream;

    while(!expressionString.empty())
    {
        if(isdigit(expressionString[0]))
        {
            size_t end = expressionString.find_first_not_of("0123456789");
            expressionStream << expressionString.substr(0, end) + ' ';
            expressionString.erase(0, end);
        }
        else if(islower(expressionString[0]))
        {
            size_t end = expressionString.find_first_not_of("abcdefghijklmnopqrstuvwxyz");
            string variableName = expressionString.substr(0, end);

            if(!SavedExpressions::getInstance()->isSavedVariable(variableName))
            {
                cout << variableName << '\n';
                cout << "Syntax Error at Line #2\n";
                return stringstream("");
            }

            expressionStream << variableName + ' ';
            expressionString.erase(0, end);
        }
        else if(isupper(expressionString[0]))
        {   
            size_t end = expressionString.find(']');
            if(end == string::npos)
            {
                cout << "Syntax Error at Line #3\n";
                return stringstream("");
            }

            size_t openBracket = expressionString.find('[');
            if(openBracket == string::npos)
            {
                cout << "Syntax Error at Line #4\n";
                return stringstream("");
            }

            if(!SavedExpressions::getInstance()->isSavedFunctionDefinition(expressionString.substr(0, openBracket)))
            {
                cout << "Syntax Error at Line #5\n";
                return stringstream("");
            }

            if(!SavedExpressions::getInstance()->isSavedVariable(expressionString.substr(openBracket + 1, end - openBracket - 1)))
            {   
                cout << expressionString.substr(openBracket + 1, end - openBracket - 1) << '\n';
                cout << "Syntax Error at Line #6\n";
                return stringstream("");
            }

            expressionStream << expressionString.substr(0, end + 1) + ' ';
            expressionString.erase(0, end + 1);
        }
        else if(isOperator(expressionString[0]))
        {
            expressionStream << expressionString[0];
            expressionStream << " ";
            expressionString.erase(0, 1);
        }
    }

    return expressionStream;

}

stringstream convertExpressionToPostfix(stringstream& ss)
{
    string buffer;
    queue<string> expressionQueue;
    stack<string> expressionStack;

    while(ss >> buffer)
    {
        
        if(isConstant(buffer) || isVariableName(buffer) || isFunctionDefinition(buffer))
        {
            expressionQueue.push(buffer);
        }
        else if(buffer == "(")
        {
            expressionStack.push(buffer);
        }
        else if(buffer == ")")
        {
            
            while(!expressionStack.empty() && expressionStack.top() != "(")
            {
                expressionQueue.push(expressionStack.top());
                expressionStack.pop();
            }

            if(!expressionStack.empty())
            {
                expressionStack.pop();
            }
        }
        else if(isOperator(buffer))
        {

            while(!expressionStack.empty() && isOperator(expressionStack.top()) && getOperatorPriority(expressionStack.top()) >= getOperatorPriority(buffer))
            {
                expressionQueue.push(expressionStack.top());
                expressionStack.pop();
            }

            expressionStack.push(buffer);
        }
        else 
        {
            cout << "Syntax Error on Line #\n";
            return stringstream();
        }
    }

    while(!expressionStack.empty())
    {
        expressionQueue.push(expressionStack.top());
        expressionStack.pop();
    }

    stringstream postfixStream;

    while(!expressionQueue.empty())
    {
        postfixStream << expressionQueue.front();
        postfixStream << " ";
        expressionQueue.pop();
    }

    return postfixStream;
}

// WIP
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