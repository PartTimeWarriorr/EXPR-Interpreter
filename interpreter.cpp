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

    // string parameterName = name.substr(openPosition + 1, closePosition - openPosition - 1);

    // if(!isVariableName(parameterName))
    // {
    //     return false;
    // }

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

bool isFunctionCall(string name)
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

    if(name[name.length() - 1] != ']')
    {
        return false;
    }

    return true;
}

size_t getFunctionClosingBracket(const string& name)
{
    size_t openingBracket = name.find('[');
    if(openingBracket == string::npos)
    {
        cout << "Syntax Error at Line #8\n";
        return -1;
    }

    int openOccurrence = 0, closeOccurence = 0;

    for(size_t i = openingBracket; i < name.length(); ++i)
    {
        switch(name[i])
        {
            case '[':
                ++openOccurrence;
                break;
            case ']':
                ++closeOccurence;
                break;
        }

        if(openOccurrence == closeOccurence)
        {
            return i;
        }
    }

    cout << "Syntax Error at Line #9\n";
    return -1;
}

Expression* buildExpressionTree(stringstream& ss)
{
    stack<Expression*> treeNodeStack;

    string expression;
    Expression *leftOperand, *rightOperand;

    while(ss >> expression)
    {

        if(isOperator(expression))
        {  

            rightOperand = treeNodeStack.top(); 
            treeNodeStack.pop();

            leftOperand = treeNodeStack.top(); 
            treeNodeStack.pop();
            
            treeNodeStack.push(new ExpressionBinaryOperator(expression[0], leftOperand, rightOperand));
        }
        else if(isConstant(expression))
        {

            treeNodeStack.push(new ExpressionConst(stoi(expression)));
        }
        else if(isVariableName(expression))
        {

            treeNodeStack.push(new ExpressionVar(expression));
        }
        else if(isFunctionCall(expression))
        {
            size_t openBracket = expression.find('[');
            string functionNameString = expression.substr(0, openBracket);
            string argumentString = expression.substr(openBracket + 1, expression.length() - openBracket - 2);
            stringstream argumentStream(argumentString);
            
            parseExpressionString(argumentStream);

            convertExpressionToPostfix(argumentStream);
            

            Expression* argumentTreeRoot = buildExpressionTree(argumentStream);

            treeNodeStack.push(new ExpressionFunctionCall(functionNameString, argumentTreeRoot));
        }
    }


    return treeNodeStack.top();
}

// Parse Expression to generalize all input, eg. a + 2*b% ABC[c] -> a + 2 * b % ABC[c]
void parseExpressionString(stringstream& ss)
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
        return;
    }

    string resultStreamString;

    while(!expressionString.empty())
    {
        if(isdigit(expressionString[0]))
        {
            size_t end = expressionString.find_first_not_of("0123456789");
            resultStreamString += expressionString.substr(0, end);
            resultStreamString += ' ';
            expressionString.erase(0, end);
        }
        else if(islower(expressionString[0]))
        {
            size_t end = expressionString.find_first_not_of("abcdefghijklmnopqrstuvwxyz");
            string variableName = expressionString.substr(0, end);

            if(!SavedExpressions::getInstance()->isSavedVariable(variableName) && variableName != SavedExpressions::getInstance()->topArgumentStack().first)
            {
                cout << "Syntax Error at Line #2\n";
                return;
            }

            resultStreamString += variableName;
            resultStreamString += ' ';
            expressionString.erase(0, end);
        }
        else if(isupper(expressionString[0]))
        {   
            // size_t end = expressionString.find(']');
            // if(end == string::npos)
            // {
            //     cout << "Syntax Error at Line #3\n";
            //     return;
            // }

            size_t end = getFunctionClosingBracket(expressionString);

            size_t openBracket = expressionString.find('[');
            if(openBracket == string::npos)
            {
                cout << "Syntax Error at Line #4\n";
                return;
            }

            if(!SavedExpressions::getInstance()->isSavedFunctionDefinition(expressionString.substr(0, openBracket)))
            {
                cout << "Syntax Error at Line #5\n";
                return;
            }

            // if(!SavedExpressions::getInstance()->isSavedVariable(expressionString.substr(openBracket + 1, end - openBracket - 1)) && 
            //    expressionString.substr(openBracket + 1, end - openBracket - 1) != SavedExpressions::currentParameter)
            // {   
            //     cout << expressionString.substr(openBracket + 1, end - openBracket - 1) << ' ' << SavedExpressions::currentParameter << '\n';
            //     cout << "Syntax Error at Line #6\n";
            //     return;
            // }

            resultStreamString += expressionString.substr(0, end + 1);
            resultStreamString += ' ';
            expressionString.erase(0, end + 1);
        }
        else if(isOperator(expressionString[0]))
        {
            resultStreamString += expressionString[0];
            resultStreamString += " ";
            expressionString.erase(0, 1);
        }
        else 
        {
            cout << expressionString << '\n';
            cout << "Syntax Error at Line #7\n";
            return;
        }
    }

    ss = stringstream(resultStreamString);

}

void convertExpressionToPostfix(stringstream& ss)
{
    string buffer;
    queue<string> expressionQueue;
    stack<string> expressionStack;

    while(ss >> buffer)
    {
        
        if(isConstant(buffer) || isVariableName(buffer) || isFunctionCall(buffer))
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
            cout << buffer << '\n';
            cout << "Syntax Error on Line #\n";
            return;
        }
    }

    while(!expressionStack.empty())
    {
        expressionQueue.push(expressionStack.top());
        expressionStack.pop();
    }

    string resultStreamString;

    while(!expressionQueue.empty())
    {
        resultStreamString += expressionQueue.front();
        resultStreamString += " ";
        expressionQueue.pop();
    }

    ss = stringstream(resultStreamString);

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

                parseExpressionString(ss);
                convertExpressionToPostfix(ss);
                Expression* expressionTreeRoot = buildExpressionTree(ss);

                ExpressionAssignmentOperator assignment(new ExpressionVar(varName), expressionTreeRoot);
                assignment.assignValueToVar();
            }

            if(isReservedWord(buffer))
            {
                if(buffer == "print")
                {   
                    parseExpressionString(ss);
                    convertExpressionToPostfix(ss);
                    Expression* expressionTreeRoot = buildExpressionTree(ss);

                    ExpressionPrint print(expressionTreeRoot);
                    print.printTree();
                }

                if(buffer == "read")
                {
                    ss >> buffer;
                    string toBeRead = buffer;

                    ExpressionRead read(new ExpressionVar(toBeRead));
                    read.readValueFromInput();
                }
            }

            if(isFunctionDefinition(buffer))
            {
                size_t openBracket = buffer.find('[');
                string functionName = buffer.substr(0, openBracket);
                size_t end = buffer.find(']');
                string parameterName = buffer.substr(openBracket + 1, end - openBracket - 1);

                ss >> buffer;
                assert(buffer == "=");

                SavedExpressions::getInstance()->pushArgumentStack(parameterName, 0);

                parseExpressionString(ss);
                convertExpressionToPostfix(ss);
                Expression* expressionTreeRoot = buildExpressionTree(ss);

                SavedExpressions::getInstance()->popArgumentStack();

                ExpressionFunctionDefinition* newFunction = new ExpressionFunctionDefinition(functionName, parameterName, expressionTreeRoot);
                newFunction->assignFunction();
            }
        }
    }
}