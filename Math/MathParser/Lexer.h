#ifndef MATH_LEXER_H_
#define MATH_LEXER_H_


#include "..\..\StringLibrary\StringLibrary.h"

struct MathLexer
{
    MathExpression* tokens;
    size_t tokenIndex;
    size_t tokensCapacity;
};

enum TokerError
{
    TOKEN_NO_ERRORS  = 0,
    TOKEN_ERR_MEMORY = 1 << 0,
    TOKEN_ERR_SYNTAX = 1 << 1
};

const size_t TokenMinSize = 100;
const double TokenScale = 2;

int  MathLexerConstructor(MathLexer* parser, const Text* text);

void MathLexerDestructor(MathLexer* parser);

int  AddToken(MathLexer* parser, const MathExpression* expression);

#ifdef GRAPHVIZ

void LexerGraphicDump(MathTree* tree);

#else

#define LexerGraphicDump(tree) ;

#endif

#endif

