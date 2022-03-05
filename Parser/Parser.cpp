#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "..\Math\LanguageTree.h"
#include "Parser.h"
#include "Lexer.h"
#include "ParserDefines.h"


static TreeNode* ProgrammDefinition(LanguageParser* parser);

static TreeNode* FunctionDefenition(LanguageParser* parser);

static TreeNode* ParseBlock(LanguageParser* parser);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

static TreeNode* Parenthesis(LanguageParser* parser);

static TreeNode* AddSubOperations(LanguageParser* parser);

static TreeNode* MulDivOperations(LanguageParser* parser);

static TreeNode* PowerOperation(LanguageParser* parser);

static TreeNode* UnaryMinus(LanguageParser* parser);

static TreeNode* Function(LanguageParser* parser);

static TreeNode* AllocNode(LanguageParser* parser);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\



int LanguageParserConstructor(LanguageParser* parser, LanguageLexer* lexer)
{
    assert(parser);
    assert(lexer);

    parser->Status     = LP_NO_ERRORS;
    parser->NodeIndex  = 0;

    parser->Tokens = lexer->Tokens;
    parser->TokenIndex = 0;
    parser->TokenCount = lexer->TokensCount;

    parser->NodeIndex = 0;
    parser->NodesCapacity = parser->TokenCount;

    parser->Parsed = false;

    parser->Nodes = (TreeNode*)calloc(parser->NodesCapacity, sizeof(TreeNode));
    
    if (!parser->Nodes)
        parser->Status = LP_ERR_NO_MEMORY;

    return parser->Status;
}

void LanguageParserDestructor(LanguageParser* parser)
{
    assert(parser);

    
}

int LanguageParserParseText(LanguageParser* parser, Tree* tree)
{
    assert(parser);

    while (!parser->Parsed)
    {
        FunctionDefenition(parser);
        if (parser->Status != LP_NO_ERRORS)
        {
            break;
        }

        ProgrammDefinition(parser);
        if (parser->Status != LP_NO_ERRORS)
        {
            break;
        }
    }

    return parser->Status;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                Рекурсивный спуск
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

static TreeNode* ProgrammDefinition(LanguageParser* parser)
{
    assert(parser);

    if (TOKEN_TYPE(ML_TYPE_LANG_CSTR) && TOKEN_CSTR == ML_CSTR_PROGRAM_START)
    {
        // Обработать лексему "начало программы".
        parser->TokenIndex++;

        // Создаём узлы дерева и обрабатываем возникающие ошибки.
        TreeNode* body      = ParseBlock(parser);
        ASSERT_NODE(body);
        // !body

        TreeNode* cstr2     = AllocNode(parser);
        ASSERT_NODE(cstr2);

        CSTR_NODE(cstr2);
        // !cstr2

        TreeNode* cstr1     = AllocNode(parser);
        ASSERT_NODE(cstr1);

        CSTR_NODE(cstr1);
        // !cstr1

        TreeNode* progLabel = AllocNode(parser);
        ASSERT_NODE(progLabel);

        progLabel->Value.Type       = ML_TYPE_IDENTIFIER;
        GrammarToken* token         = FindConstructionToken(ML_CSTR_PROGRAM_START, GRAMMAR_ALL);

        progLabel->Value.Identifier = (char*)calloc(token->TreeNameSize, sizeof(char));

        if (!progLabel->Value.Identifier)
        {
            // #err
            // #log
            parser->Status = LP_ERR_NO_MEMORY;
            return nullptr;
        }

        strcpy(progLabel->Value.Identifier, token->TreeName);
        // !progLabel

        TreeNode* progStart = AllocNode(parser);
        ASSERT_NODE(progStart);
        
        progStart->Value.Type         = ML_TYPE_LANG_CSTR;
        progStart->Value.Construction = ML_CSTR_PROGRAM_START;
        // !progStart

        // Подвязываем узлы к дереву.
        // Добавляем к дереву потомков cstr2.
        TreeAddRightNode(cstr2, body);

        // Добавляем к дереву потомков cstr1.
        TreeAddRightNode(cstr1, cstr2);
        TreeAddLeftNode(cstr1, progLabel);

        TreeAddRightNode(progStart, cstr1);

        return progStart;
    }

    return nullptr;
}

static TreeNode* FunctionDefenition(LanguageParser* parser)
{
    assert(parser);

    if (TOKEN_TYPE(ML_TYPE_IDENTIFIER) && TOKEN_CSTR == ML_CSTR_PROGRAM_START)
    {
        // Обработать лексему "начало программы".

        // Создаём узлы дерева и обрабатываем возникающие ошибки.
        TreeNode* body      = ParseBlock(parser);
        ASSERT_NODE(body);
        // !body

        TreeNode* cstr2     = AllocNode(parser);
        ASSERT_NODE(cstr2);

        CSTR_NODE(cstr2);
        // !cstr2

        TreeNode* cstr1     = AllocNode(parser);
        ASSERT_NODE(cstr1);

        CSTR_NODE(cstr1);
        // !cstr1

        TreeNode* progLabel = AllocNode(parser);
        ASSERT_NODE(progLabel);

        progLabel->Value.Type       = ML_TYPE_IDENTIFIER;
        GrammarToken* token         = FindConstructionToken(ML_CSTR_PROGRAM_START, GRAMMAR_ALL);

        progLabel->Value.Identifier = (char*)calloc(token->TreeNameSize, sizeof(char));

        if (!progLabel->Value.Identifier)
        {
            // #err
            // #log
            parser->Status = LP_ERR_NO_MEMORY;
            return nullptr;
        }

        strcpy(progLabel->Value.Identifier, token->TreeName);
        // !progLabel

        TreeNode* progStart = AllocNode(parser);
        ASSERT_NODE(progStart);

        progStart->Value.Type         = ML_TYPE_LANG_CSTR;
        progStart->Value.Construction = ML_CSTR_PROGRAM_START;
        // !progStart

        // Подвязываем узлы к дереву
        // Добавляем к дереву потомков cstr2
        TreeAddRightNode(cstr2, body);

        // Добавляем к дереву потомков cstr1
        TreeAddRightNode(cstr1, cstr2);
        TreeAddLeftNode(cstr1, progLabel);

        TreeAddRightNode(progStart, cstr1);

        return progStart;
    }

    return nullptr;
}

static TreeNode* FunctionCall(LanguageParser* parser)
{
    assert(parser);

    return nullptr;
}

static TreeNode* ParseBlock(LanguageParser* parser)
{

}

static TreeNode* Assigment(LanguageParser* parser)
{

    return nullptr;
}

static TreeNode* Block(LanguageParser* parser)
{
    return nullptr;

}

static TreeNode* While(LanguageParser* parser)
{
    return nullptr;
}

static TreeNode* For(LanguageParser* parser)
{

    return nullptr;
}

static TreeNode* If(LanguageParser* parser)
{

    return nullptr;
}

static TreeNode* FunctionalOperator(LanguageParser* parser)
{
    return nullptr;

}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                             Математические выражения
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

static TreeNode* Parenthesis(LanguageParser* parser)
{
    assert(parser);
    return nullptr;
}

static TreeNode* Function(LanguageParser* parser)
{
    assert(parser);
    return nullptr;
}

static TreeNode* AddSubOperations(LanguageParser* parser)
{
    assert(parser);
    return nullptr;
}

static TreeNode* MulDivOperations(LanguageParser* parser)
{
    assert(parser);
    return nullptr;
}

static TreeNode* PowerOperation(LanguageParser* parser)
{        
    assert(parser);
    return nullptr;
}

static TreeNode* UnaryMinus(LanguageParser* parser)
{
    assert(parser);
    return nullptr;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                               Расширяющийся массив
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

static bool __reallocNodes(LanguageParser* parser);

static TreeNode* AllocNode(LanguageParser* parser)
{
    assert(parser);

    if (parser->NodeIndex >= parser->NodesCapacity &&
        !__reallocNodes(parser))
    {
        // #err
        // #log
        parser->Status = LP_ERR_NO_MEMORY;
        return nullptr;
    }

    return parser->Nodes + parser->NodeIndex++;
}

static bool __reallocNodes(LanguageParser* parser)
{
    assert(parser);
    // Для ускорения программы перед вызовом функции, необходимо проверить нужно ли её вызывать. 
    assert(parser->NodeIndex >= parser->NodesCapacity);

    TreeNode* nodes = (TreeNode*)realloc(parser->Nodes, parser->NodesCapacity * NodesScale * sizeof(TreeNode));

    if (!nodes)
    {
        // #log
        // #err
        parser->Status = LP_ERR_NO_MEMORY;
        return false;
    }

    parser->Nodes = nodes;

    return parser->Nodes;
}

#include "ParserUndef.h"