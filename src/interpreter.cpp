#include "../include/interpreter.h"

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

bool isOperator(char symbol)
{
    return symbol == '+' || 
           symbol == '-' || 
           symbol == '*' || 
           symbol == '%' || 
           symbol == '/' || 
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
           symbol == "(" || 
           symbol == ")";
}

bool isBinaryOperator(char symbol)
{
    return symbol == '+'||
           symbol == '-' ||
           symbol == '*' ||
           symbol == '%' ||
           symbol == '/';
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

// Get the corresponding closing bracket to a function's opening bracket. 
// In the case of multiple nested functions: "X[Y[3] + Z[a+X[2*3]]]" returns outmost bracket.
size_t getFunctionClosingBracket(const string& name)
{
    size_t openingBracket = name.find('[');

    if(openingBracket == string::npos)
    {
        throw std::invalid_argument("Function opening bracket '[' cannot be found.\nSyntax Error at Line #");
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

    throw std::invalid_argument("Unbalanced function brackets.\nSyntax Error at Line #");
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
            
            try
            {

                parseExpressionString(argumentStream);
                convertExpressionToPostfix(argumentStream);
            }
            catch(const std::exception& exc)
            {
                throw std::invalid_argument("Unrecognized function arguments.\nSyntax Error at Line #");
            }
            

            Expression* argumentTreeRoot = buildExpressionTree(argumentStream);

            treeNodeStack.push(new ExpressionFunctionCall(functionNameString, argumentTreeRoot));
        }
    }

    if(treeNodeStack.size() != 1)
    {
        throw std::invalid_argument("Missing operator.\nSyntax Error at Line #");
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

    if(expressionString.empty())
    {
        throw std::invalid_argument("Syntax Error at Line #");
    }

    if(isBinaryOperator(expressionString[0]))
    {
        throw std::invalid_argument("Expression cannot begin with a binary operator.\nSyntax Error at Line #");
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

            if(!SavedExpressions::getInstance()->isSavedVariable(variableName) && (SavedExpressions::getInstance()->isEmptyArgumentStack() || variableName != SavedExpressions::getInstance()->topArgumentStack().first))
            {
                throw std::invalid_argument("Unrecognized variable name.\nSyntax Error at Line #");
            }

            resultStreamString += variableName;
            resultStreamString += ' ';
            expressionString.erase(0, end);
        }
        else if(isupper(expressionString[0]))
        {   

            size_t end = getFunctionClosingBracket(expressionString);

            size_t openBracket = expressionString.find('[');

            if(!SavedExpressions::getInstance()->isSavedFunctionDefinition(expressionString.substr(0, openBracket)))
            {
                throw std::invalid_argument("Unrecognized function name.\nSyntax Error at Line #");
            }

            resultStreamString += expressionString.substr(0, end + 1);
            resultStreamString += ' ';
            expressionString.erase(0, end + 1);
        }
        else if(isOperator(expressionString[0]))
        {
            
            if(!resultStreamString.empty() && isBinaryOperator(resultStreamString[resultStreamString.length() - 2]) && isBinaryOperator(expressionString[0]))
            {
                throw std::invalid_argument("Expression cannot have two consecutive operators.\nSyntax Error at Line #");
            }

            resultStreamString += expressionString[0];
            resultStreamString += " ";
            expressionString.erase(0, 1);
        }
        else 
        {
            
            throw std::invalid_argument("Syntax Error at Line #");
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
            throw std::invalid_argument("Syntax Error at Line #");
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

void parseEXPRFile(string fileName)
{   
    ifstream file(fileName);
    string line = "";
    int lineNumber = 0;

    while(!file.eof())
    {
        ++lineNumber;
        getline(file, line);
        stringstream ss(line);
        string buffer;

        while(ss >> buffer)
        {
            if(isVariableName(buffer))
            {   
                string varName = buffer;
                ss >> buffer;
                
                if(buffer != "=")
                {
                    cout << "Syntax Error at Line #" << lineNumber << '\n';
                    file.close();
                    return;
                }

                try
                {
                    parseExpressionString(ss);
                    convertExpressionToPostfix(ss);
                    Expression* expressionTreeRoot = buildExpressionTree(ss);

                    ExpressionAssignmentOperator assignment(new ExpressionVar(varName), expressionTreeRoot);
                    assignment.assignValueToVar();
                }
                catch(const std::exception& exc)
                {
                    cout << exc.what() << lineNumber << '\n';
                    file.close();
                    return;
                }
                
            }
            else if(isReservedWord(buffer))
            {
                if(buffer == "print")
                {  
                    try
                    {
                        parseExpressionString(ss);
                        convertExpressionToPostfix(ss);
                        Expression* expressionTreeRoot = buildExpressionTree(ss);

                        ExpressionPrint print(expressionTreeRoot);
                        print.printTree();
                    }
                    catch(const std::exception& exc)
                    {
                        cout << exc.what() << lineNumber << '\n';
                        file.close();
                        return;
                    }
                     
                }

                if(buffer == "read")
                {
                    ss >> buffer;
                    string toBeRead = buffer;

                    if(!ss)
                    {
                        cout << "Variable name cannot be empty.\nSyntax Error at Line #" << lineNumber << '\n';
                        file.close();
                        return;
                    }
                    
                    try
                    {
                        ExpressionRead read(new ExpressionVar(toBeRead));
                        read.readValueFromInput();
                    }
                    catch(const std::exception& exc)
                    {
                        cout << exc.what() << lineNumber << '\n';
                        file.close();
                        return;
                    }

                }
            }
            else if(isFunctionDefinition(buffer))
            {
                size_t openBracket = buffer.find('[');
                string functionName = buffer.substr(0, openBracket);
                size_t end = buffer.find(']');
                string parameterName = buffer.substr(openBracket + 1, end - openBracket - 1);

                ss >> buffer;

                if(buffer != "=")
                {
                    cout << "Syntax Error at Line #" << lineNumber << '\n';
                    file.close();
                    return;
                }

                try 
                {
                    SavedExpressions::getInstance()->pushArgumentStack(parameterName, 0);

                    parseExpressionString(ss);
                    convertExpressionToPostfix(ss);
                    Expression* expressionTreeRoot = buildExpressionTree(ss);

                    SavedExpressions::getInstance()->popArgumentStack();

                    ExpressionFunctionDefinition* newFunction = new ExpressionFunctionDefinition(functionName, parameterName, expressionTreeRoot);
                    newFunction->assignFunction();
                }
                catch(const std::exception& exc)
                {
                    cout << exc.what() << lineNumber << '\n';
                    file.close();
                    return;
                }
                
            }
            else 
            {
                cout << "Syntax Error at Line #" << lineNumber << '\n';
                file.close();
                return;
            }
        }
    }

    file.close();
}