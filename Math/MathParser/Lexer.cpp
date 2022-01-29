#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#include "..\..\Logs\Logs.h"
#include "MathParser.h"
#include "Lexer.h"
#include "..\MathTree\MathTree.h"


static int AllocTokens(MathLexer* lexer, size_t size);

static int GetTokens(MathLexer* lexer, const Text* text);

static bool CheckCorrectInput(const Text* text);

static bool CompareStrings(const char* ptr, const char* name);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

int MathLexerConstructor(MathLexer* lexer, const Text* text)
{
    assert(lexer);
    assert(text);

    int status = TOKEN_NO_ERRORS;

    if ((status = AllocTokens(lexer, TokenMinSize)) != TOKEN_NO_ERRORS)
        return status;

    lexer->tokenIndex = 0;

    if (!CheckCorrectInput(text))
        return TOKEN_ERR_SYNTAX;

    if ((status = GetTokens(lexer, text)) != TOKEN_NO_ERRORS)
        return status;

    return status;
}

void MathLexerDestructor(MathLexer* lexer)
{
    assert(lexer);

    free(lexer->tokens);
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

static int GetTokens(MathLexer* lexer, const Text* text)
{
    assert(lexer);
    assert(text);

    int status = TOKEN_NO_ERRORS;
    
    size_t parsedCount = 0;
    char* ptr  = text->buffer;
    MathExpression expr = {};

    while (*ptr)
    {
        bool parsed = false;

        while (*ptr == ' ' || *ptr == '\r' || *ptr == '\t' || *ptr == '\n')
            ptr++;

        if (*ptr == '\0')
            break;

        // Операторы
        for (size_t st = 0; !parsed && st < sizeof(MathOperatorNames) / sizeof(MathOperatorNames[0]); st++)
        {
            if (*ptr == MathOperatorNames[st])
            {
                expr.type = ME_OPERATOR;
                expr.me_operator = (MathOperator)st;

                if ((status = AddToken(lexer, &expr)) != TOKEN_NO_ERRORS)
                    return status;

                ptr++;
                parsed = true;
                break;
            }
        }
        
        // Числа
        parsedCount = 0;
        if (!parsed && sscanf(ptr, "%lf%n", &expr.me_number, &parsedCount))
        {
            expr.type = ME_NUMBER;

            if ((status = AddToken(lexer, &expr)) != TOKEN_NO_ERRORS)
                return status;

            ptr += parsedCount;
            parsed = true;
        }

        // Константы
        for (size_t st = 0; !parsed && st < sizeof(MathConstantNames) / sizeof(MathConstantNames[0]); st++)
        {
            if (CompareStrings(ptr, MathConstantNames[st]))
            {
                expr.type = ME_OPERATOR;
                expr.me_operator = (MathOperator)st;

                if ((status = AddToken(lexer, &expr)) != TOKEN_NO_ERRORS)
                    return status;

                ptr += strlen(MathConstantNames[st]);
                parsed = true;
                break;
            }
        }

        // Функции
        for (size_t st = 0; !parsed && st < sizeof(MathFunctionNames) / sizeof(MathFunctionNames[0]); st++)
        {
            if (CompareStrings(ptr, MathFunctionNames[st]))
            {
                expr.type = ME_FUNCTION;
                expr.me_operator = (MathOperator)st;

                if ((status = AddToken(lexer, &expr)) != TOKEN_NO_ERRORS)
                    return status;

                ptr += strlen(MathFunctionNames[st]);
                parsed = true;
                break;
            }
        }

        // Переменные
        // Разделителем переменных являются математические операторы
        char* variableStart = ptr;
        bool  stopSymbol = false;
        while (!parsed && *ptr && !stopSymbol)
        {
            for (size_t st = 0; st < sizeof(MathOperatorNames) / sizeof(MathOperatorNames[0]); st++)
            {
                if (*ptr == MathOperatorNames[st])
                {
                    stopSymbol = true;
                    ptr--;
                    break;
                }
            }
            ptr++;
        }

        size_t varSize = ptr - variableStart;

        if (varSize > 0)
        {
            char* variable = (char*)calloc(varSize + 1, sizeof(char));

            if (!variable)
                return TOKEN_ERR_MEMORY;

            strncpy(variable, variableStart, varSize);

            expr.me_variable = variable;
            expr.type = ME_VARIABLE;
            
            if ((status = AddToken(lexer, &expr)) != TOKEN_NO_ERRORS)
                return status;

            parsed = true;
        }

        // Обработка синтаксической ошибки
        if (!parsed)
        {
            LOG_MATH_TREE_ERR("Синтаксическая ошибка во входной строке.");
            status = TOKEN_ERR_SYNTAX;
            break;
        }
    }

    return status;
}

static bool CompareStrings(const char* ptr, const char* name)
{
    while (*ptr && *name)
    {
        if (*ptr != *name)
            return false;
        ptr++;
        name++;
    }
    return true;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

int AddToken(MathLexer* lexer, const MathExpression* expression)
{
    assert(lexer);

    int status = TOKEN_NO_ERRORS;

    if (lexer->tokenIndex >= lexer->tokensCapacity)
    {
        if ((status = AllocTokens(lexer, lexer->tokensCapacity * TokenScale))!= TOKEN_NO_ERRORS)
            return status;
    }

    lexer->tokens[lexer->tokenIndex++] = *expression;

    return status;
}

static int AllocTokens(MathLexer* lexer, const size_t size)
{
    assert(lexer);

    MathExpression* arr = (MathExpression*)realloc(lexer->tokens, sizeof(MathExpression) * size);
    if (!arr)
    {
        LOG_MATH_TREE_ERR("Не хватает памяти");
        return TOKEN_ERR_MEMORY;
    }

    lexer->tokens = arr;
    lexer->tokensCapacity = size;

    return TOKEN_NO_ERRORS;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

static bool CheckCorrectInput(const Text* text)
{
    assert(text);

    char*  ptr = text->buffer;
    size_t lBracketsCount = 0;
    size_t rBracketsCount = 0;

    while (*ptr)
    {
        if (*ptr == '(')
            lBracketsCount++;
        else if (*ptr == ')')
            rBracketsCount++;

        if (lBracketsCount < rBracketsCount)
        {
            LOG_CALC_ERR("Ошибочная скобочная последовательность.\n"
                         "Количество закрывающих скобок '{' больше количества открывающих '}'");
            return false;
        }

        ptr++;
    }

    return true;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

#ifdef GRAPHVIZ

void LexerGraphicDump(MathTree* tree)
{
    LOG_MATH_TREE_DBG("LexerGraphicDump");

    assert(tree);

    FILE* file = fopen("lexer.gv", "w");

    if (!file)
    {
        LOG_MATH_TREE_ERR("CreateTreeGraph: ошибка открытия файла.");
        return;
    }

    fputs("digraph G\n"
          "{\n"
          "    bgcolor=\"lightblue\"\n"
          "    node [fontcolor=gray11, style=filled, fontsize = 18];\n", file);

    for (size_t st = 0; st < tree->nodesArraySize; st++)
    {
        CreateNodeGraph(file, &tree->nodesArrayPtr[st], 0, true);
    }

    fputs("}", file);

    fclose(file);

    char cmd[500];

    sprintf(cmd, "dot \"lexer.gv\" -Tpng > \"lexerDump.png\"");
    system(cmd);

    remove("lexer.gv");

    system("lexerDump.png");
}

#endif