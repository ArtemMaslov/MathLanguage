#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#include "..\Logs\Logs.h"
#include "..\MyStdLib.h"
#include "Lexer.h"


static int AllocTokens(LanguageLexer* lexer, size_t size);

static int GetTokens(LanguageLexer* lexer, const Text* text);

static bool CheckCorrectParenthesis(const Text* text);

static bool SkipComments(char** buf);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

int LexerConstructor(LanguageLexer* lexer, const Text* text)
{
    assert(lexer);
    assert(text);

    int status = LXR_NO_ERRORS;

    if ((status = AllocTokens(lexer, TokenMinSize)) != LXR_NO_ERRORS)
        return status;

    lexer->TokenIndex = 0;

    //if (!CheckCorrectParenthesis(text))
    //    return LXR_ERR_SYNTAX;

    if ((status = GetTokens(lexer, text)) != LXR_NO_ERRORS)
        return status;

    return status;
}

void LexerDestructor(LanguageLexer* lexer)
{
    assert(lexer);

    free(lexer->Tokens);
    lexer->Tokens = nullptr;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

static int GetTokens(LanguageLexer* lexer, const Text* text)
{
    assert(lexer);
    assert(text);

    int status = LXR_NO_ERRORS;
    
    int parsedCount = 0;
    char* ptr  = text->buffer;
    Expression expr = {};

    while (*ptr)
    {
        bool parsed = false;

        if (!SkipSpaceSymbols(&ptr))
            break;

        if (!SkipComments(&ptr))
            break;

        // Математические операторы
        for (size_t grammarTypeIndex = 0; !parsed && grammarTypeIndex < GrammarTokensTypesCount; grammarTypeIndex++)
        {
            GrammarToken* tokens       = GrammarTokens[grammarTypeIndex]->Tokens;
            size_t        tokensLength = GrammarTokens[grammarTypeIndex]->TokensLength;

            for (size_t grammarIndex = 0; grammarIndex < tokensLength; grammarIndex++)
            {
                if (!strncmp(ptr, tokens[grammarIndex].TokenName, tokens[grammarIndex].TokenSize))
                {
                    expr.Type = GrammarTokens[grammarTypeIndex]->TokensType;

                    memcpy(&expr.Constant, &tokens[grammarIndex].TokenCode, sizeof(tokens[grammarIndex].TokenCode));

                    if ((status = AddToken(lexer, &expr)) != LXR_NO_ERRORS)
                        return status;

                    ptr   += tokens[grammarIndex].TokenSize;
                    parsed = true;
                    break;
                }
            }
        }

        // Числа
        parsedCount = 0;
        if (!parsed && sscanf(ptr, "%lf%n", &expr.Number, &parsedCount))
        {
            expr.Type = ML_TYPE_NUMBER;

            if ((status = AddToken(lexer, &expr)) != LXR_NO_ERRORS)
                return status;

            ptr += parsedCount - 1;

            if (*ptr != '.')
                ptr++;

            parsed = true;
        }

        // Переменные
        // Разделителем переменных являются математические операторы
        char* variableStart = ptr;
        bool  stopSymbol = false;
        while (!parsed && *ptr && !stopSymbol)
        {
            for (size_t st = 0; st < VariableSeparatorsLength; st++)
            {
                if (*ptr == VariableSeparators[st])
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
                return LXR_ERR_MEMORY;

            strncpy(variable, variableStart, varSize);

            expr.Identifier = variable;
            expr.Type = ML_TYPE_IDENTIFIER;
            
            if ((status = AddToken(lexer, &expr)) != LXR_NO_ERRORS)
                return status;

            parsed = true;
        }

        // Обработка синтаксической ошибки
        if (!parsed)
        {
            LOG_TREE_ERR("Синтаксическая ошибка во входной строке.\n");
            status = LXR_ERR_SYNTAX;
            break;
        }
    }

    return status;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

int AddToken(LanguageLexer* lexer, const Expression* expression)
{
    assert(lexer);
    assert(expression);

    int status = LXR_NO_ERRORS;

    if (lexer->TokenIndex >= lexer->TokensCapacity)
    {
        if ((status = AllocTokens(lexer, (size_t)(lexer->TokensCapacity * TokenScale) + 1))!= LXR_NO_ERRORS)
            return status;
    }

    lexer->Tokens[lexer->TokenIndex++] = *expression;

    return status;
}

static int AllocTokens(LanguageLexer* lexer, const size_t size)
{
    assert(lexer);

    Expression* arr = (Expression*)realloc(lexer->Tokens, sizeof(Expression) * size);
    if (!arr)
    {
        LOG_TREE_ERR("Не хватает памяти");
        return LXR_ERR_MEMORY;
    }

    lexer->Tokens = arr;
    lexer->TokensCapacity = size;

    return LXR_NO_ERRORS;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

/**
 * @brief     Пропускает пробельные символы.
 * @param ptr Указатель на строку символов
 * @return    false, если строка символов закончилась и был обнаружен '\0'.
*/
static bool SkipComments(char** buf)
{
    assert(buf);

    if ((*buf)[0] == '/' && (*buf)[1] == '/')      // Однострочный комментарий
    {
        (*buf) += 2;

        while (**buf && **buf != '\n')
            (*buf)++;

        if (**buf)
        {
            (*buf)++;
            return true;
        }
        else
            return false;
    }
    else if ((*buf)[0] == '/' && (*buf)[1] == '*') // Многострочный комментарий
    {
        (*buf) += 2;

        while (**buf && (*buf)[0] != '*' && (*buf)[1] != '/')
            (*buf)++;
        
        if (**buf)
        {
            (*buf) += 2;
            return true;
        }
        else
            return false;
    }

    return true;
}

/**
 * @brief      Проверяет, является ли идентификатор ключевым словом (Функциональные операторы, стандартные функции). Если да, то меняет тип выражения.
 *             Ключевые слова не чувствительны к регистру, поэтому выражения "ЕСЛИ" и "если" нужно распознавать как одно и то же.
 *             При первом анализе ключевые слова считаются идентификаторами. Данная функция анализирует идентификаторы и распознаёт в них ключевые слова.
 * @param expr 
*/
static void CheckIdentifierForKeywords(Expression* expr)
{
    assert(expr);


}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

#ifdef GRAPHVIZ

void LexerGraphicDump(Tree* tree)
{
    LOG_TREE_DBG("LexerGraphicDump");

    assert(tree);

    FILE* file = fopen("lexer.gv", "w");

    if (!file)
    {
        LOG_TREE_ERR("CreateTreeGraph: ошибка открытия файла.");
        return;
    }

    TreeGraphNodeId = 1;

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

    sprintf(cmd, "dot \"lexer.gv\" -Tsvg > \"lexerDump.svg\"");
    system(cmd);

    //remove("lexer.gv");

    system("lexerDump.svg");
}

#endif