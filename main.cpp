#include <iostream>
#include "interpreter.h"
#include <ctype.h>
using std::cout;
int main()
{

    // ifstream file("test.txt");
    // string line = "";
    // cout << "# ";
    // while(!file.eof())
    // {
    //     getline(file, line);
    //     cout << line << "\n# ";
    // }

    // ifstream file("test.txt");
    // string line = "";

    // while(!file.eof())
    // {
    //     getline(file, line);
    //     stringstream ss(line);
    //     string buffer;
    //     while(ss >> buffer)
    //     {
    //         cout << buffer << '.';
    //     }

    //     cout << "EOL\n";
    // }

    // if(islower('C'))
    //  cout << "yes";

    parseEXPRFile("basicTest.txt");

    // SavedExpressions::getInstance()->printSavedFunctions();

    // stringstream expr("2 3 + 0 +");
    // Expression* expressionTreeRoot = buildExpressionTree(expr);
    // cout << expressionTreeRoot->getValue();
    // ExpressionPrint print(expressionTreeRoot);
    // print.printTree();

    // cout << stoi("2");
}