#ifndef LEXER_H_
#define LEXER_H_


#include "..\Config.h"
#include "..\LanguageGrammar.h"
#include "..\Math\LanguageTree.h"

struct LanguageLexer
{
    Expression* Tokens;
    size_t      TokenIndex;
    size_t      TokensCount;
    size_t      TokensCapacity;
};

enum LexerErrors
{
    LXR_NO_ERRORS  = 0,
    LXR_ERR_MEMORY = 1 << 0,
    LXR_ERR_SYNTAX = 1 << 1
};

const size_t TokenMinSize = 100;
const double TokenScale = 2;

int LexerConstructor(LanguageLexer* lexer);

void LexerDestructor(LanguageLexer* lexer);

int LexerGetTokens(LanguageLexer* lexer, const Text* text);

void LexerClear(LanguageLexer* lexer);

#ifdef GRAPHVIZ

void LexerGraphicDump(Tree* tree);

#else

#define LanguageLexerGraphicDump(tree) ;

#endif

#endif