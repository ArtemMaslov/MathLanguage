#ifndef MATH_PARSER_H_
#define MATH_PARSER_H_


#include "..\MathExpression\MathExpression.h"
#include "..\MathTree\MathTree.h"
#include "..\..\StringLibrary\StringLibrary.h"


enum MathParserError
{
    MP_NO_ERRORS  = 0,
    MP_ERR_MEMORY = 1 << 0,
    MP_ERR_SYNTAX = 1 << 1
};


bool ReadTreeFromFile(Tree* tree, FILE* file);

void WriteTreeToFile(Tree* problem, FILE* file);


#endif