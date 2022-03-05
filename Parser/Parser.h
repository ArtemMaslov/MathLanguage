#ifndef PARSER_H_
#define PARSER_H_


#include "..\Math\LanguageTree.h"
#include "Lexer.h"

enum LanguageParserErrors
{
    LP_NO_ERRORS     = 0,
    LP_ERR_NO_MEMORY = 1 << 0,
    LP_ERR_SYNTAX    = 1 << 1,
};

const size_t NodesMinSize = 100;
const double NodesScale   = 2;

struct LanguageParser
{
    TreeNode*  Nodes;

    size_t NodeIndex;
    size_t NodesCapacity;

    Expression* Tokens;
    size_t TokenIndex;
    size_t TokenCount;

    LanguageParserErrors Status;
    bool Parsed;
};


#endif