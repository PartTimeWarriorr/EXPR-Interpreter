#pragma once
#include <string>
#include <fstream>


using std::string;
using std::ifstream;
using std::ostream;

class Tokenizer
{
    public:
    enum TokenType{OPEN_PAR, CLOSE_PAR, NUMBER, ARITH_OPER, ASSIGN_OPER, EO_FILE, VARIABLE, FUNCTION_CALL, PRINT, READ};
    
    struct Token
    {
        char symbol;
        int value;
        string name;
        TokenType type;
    };

};

std::istream& operator>>(ifstream& in, Tokenizer::Token& token)
{

    string next;
}

// Maybe read line by line.. so operator>>(ifstream& line, Tokenizer::Token& token)