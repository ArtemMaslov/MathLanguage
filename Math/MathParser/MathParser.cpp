#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>


#include "MathParser.h"
#include "Lexer.h"
#include "..\..\Logs\Logs.h"

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

struct MathParser
{
    MathNode*       nodes;
    size_t          nodeIndex;
    size_t          nodesSize;

    MathParserError status;
};

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

static void WriteTreeNodeToFile(MathNode* node, FILE* file, size_t recursiveLength);

static MathParserError ParseMathTree(const Text* text, MathTree* tree);

static MathNode* ParseParenthesis(MathParser* parser);

static MathNode* ParseAddSubOperations(MathParser* parser);

static MathNode* ParseMulDivOperations(MathParser* parser);

static MathNode* ParsePowerOperation(MathParser* parser);

static MathNode* ParseUnaryMinus(MathParser* parser);

static MathNode* ParseFunction(MathParser* parser);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

void WriteTreeToFile(MathTree* problem, FILE* file)
{
    assert(problem);
    assert(file);

    if (problem->root)
        WriteTreeNodeToFile(problem->root, file, 0);
}

bool ReadTreeFromFile(MathTree* tree, FILE* file)
{
    assert(tree);
    assert(file);

    Text text = {};

    TreeConstructor(tree);

    if (!ReadFile(&text, file))
        return false;

    if (ParseMathTree(&text, tree) != MP_NO_ERRORS)
        return false;
    
    TextDestructor(&text);

    return true;
}

static void WriteTreeNodeToFile(MathNode* node, FILE* file, size_t recursiveLength)
{
    assert(node);
    assert(file);

    fprintf(file, "%*s(\n", recursiveLength * 4, "");
    
    if (node->nodeLeft && node->nodeLeft->expression.type != ME_UNKNOWN)
        WriteTreeNodeToFile(node->nodeLeft, file, recursiveLength + 1);

    fprintf(file, "%*s", (recursiveLength + 1) * 4, "");
    PrintMathExpression(&node->expression, file);
    fputc('\n', file);

    if (node->nodeRight && node->nodeRight->expression.type != ME_UNKNOWN)
        WriteTreeNodeToFile(node->nodeRight, file, recursiveLength + 1);
    
    fprintf(file, "%*s)\n", recursiveLength * 4, "");
}


///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

static MathParserError ParseMathTree(const Text* text, MathTree* tree)
{
    assert(text);
    assert(tree);
    
    MathLexer  lexer  = {};
    MathParser parser = {};

    MathLexerConstructor(&lexer, text);

    tree->allocatedSeparately = false;
    parser.nodes     = tree->nodesArrayPtr       = (MathNode*)calloc(lexer.tokenIndex, sizeof(MathNode));
    parser.nodesSize = tree->nodesArraySize      = lexer.tokenIndex;
    parser.status    = MP_NO_ERRORS;

    if (!parser.nodes)
    {
        LOG_MATH_TREE_ERR("Ошибка выделения памяти");
        return MP_ERR_MEMORY;
    }

    for (size_t st = 0; st < lexer.tokenIndex; st++)
    {
        tree->nodesArrayPtr[st].childCount = 0;

        tree->nodesArrayPtr[st].nodeLeft   = nullptr;
        tree->nodesArrayPtr[st].nodeRight  = nullptr;
        tree->nodesArrayPtr[st].parent     = nullptr;

        tree->nodesArrayPtr[st].expression = lexer.tokens[st];
    }
    
    LexerGraphicDump(tree);

    MathNode* node = ParseAddSubOperations(&parser);
    
    MathLexerDestructor(&lexer);

    if (parser.status == MP_NO_ERRORS)
    {
        tree->root = node;
        CreateTreeGraph("taskGraph", tree->root, true);
    }

    return parser.status;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                Рекурсивный спуск
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\


static MathNode* ParseParenthesis(MathParser* parser)
{
    assert(parser);

    MathNode* curNode   = parser->nodes + parser->nodeIndex;
    MathNode* nextNode  = parser->nodes + parser->nodeIndex + 1;
    MathNode* rightNode = nullptr;

    if (!curNode)
    {
        parser->status = MP_ERR_SYNTAX;
        return nullptr;
    }

    switch (curNode->expression.type)
    {
        case ME_OPERATOR:

            if (curNode->expression.me_operator != ME_L_PARENTHESIS)
            {
                parser->status = MP_ERR_SYNTAX;
                return nullptr;
            }
            parser->nodeIndex++;

            rightNode = ParseAddSubOperations(parser);

            if (!rightNode || parser->status != MP_NO_ERRORS)
                return nullptr;

            if (parser->nodes[parser->nodeIndex].expression.me_operator != ME_R_PARENTHESIS)
            {
                parser->status = MP_ERR_SYNTAX;
                return nullptr;
            }
            parser->nodeIndex++;

            return rightNode;

        case ME_NUMBER:
        case ME_VARIABLE:
        case ME_CONSTANT:

            parser->nodeIndex++;
            return curNode;

        case ME_FUNCTION:

            return ParseFunction(parser);
    }

    parser->status = MP_ERR_SYNTAX;
    return nullptr;
}

static MathNode* ParseFunction(MathParser* parser)
{
    assert(parser);
    
    MathNode* curNode   = parser->nodes + parser->nodeIndex;
    MathNode* nextNode  = parser->nodes + parser->nodeIndex + 1;
    MathNode* rightNode = nullptr;

    if (curNode && curNode->expression.type != ME_FUNCTION)
        return nullptr;

    if (!curNode || !nextNode)
    {
        parser->status = MP_ERR_SYNTAX;
        return nullptr;
    }
    
    parser->nodeIndex++;

    switch (nextNode->expression.type)
    {
        case ME_NUMBER:
        case ME_VARIABLE:
        case ME_CONSTANT:
            rightNode = nextNode;
            parser->nodeIndex++;
            break;

        case ME_FUNCTION:
            rightNode = ParseFunction(parser);
            break;

        case ME_OPERATOR:
            rightNode = ParseParenthesis(parser);

            if (!rightNode || parser->status != MP_NO_ERRORS)
                return nullptr;

            break;

        default:
            parser->status = MP_ERR_SYNTAX;
            return nullptr;
    }

    TreeAddRightNode(curNode, rightNode);

    return curNode;
}

static MathNode* ParseAddSubOperations(MathParser* parser)
{
    assert(parser);

    MathExpression expr  = {};
    MathNode* leftNode   = nullptr;
    MathNode* parentNode = nullptr;

    bool oper = false;

    do
    {
        oper = false;

        MathNode* rightNode = ParseMulDivOperations(parser);

        if (!rightNode || parser->status != MP_NO_ERRORS)
            return nullptr;

        if (leftNode)
        {
            TreeAddLeftNode(parentNode, leftNode);
            TreeAddRightNode(parentNode, rightNode);

            leftNode = parentNode;
        }
        else
            leftNode = rightNode;

        if (parser->nodeIndex >= parser->nodesSize)
            return leftNode;

        parentNode = &parser->nodes[parser->nodeIndex];
        expr       =  parser->nodes[parser->nodeIndex].expression;

        if  (expr.type        == ME_OPERATOR &&
            (expr.me_operator == ME_ADDITION || expr.me_operator == ME_SUBTRACTION))
        {
            oper = true;
            parser->nodeIndex++;
        }
    } 
    while (oper);

    return leftNode;
}

static MathNode* ParseMulDivOperations(MathParser* parser)
{
    assert(parser);

    MathExpression expr  = {};
    MathNode* leftNode   = nullptr;
    MathNode* parentNode = nullptr;
    
    bool oper = false;

    do
    {
        oper = false;

        MathNode* rightNode = ParsePowerOperation(parser);

        if (!rightNode || parser->status != MP_NO_ERRORS)
            return nullptr;

        if (leftNode)
        {
            TreeAddLeftNode(parentNode, leftNode);
            TreeAddRightNode(parentNode, rightNode);

            leftNode = parentNode;
        }
        else
            leftNode = rightNode;
        
        if (parser->nodeIndex >= parser->nodesSize)
            return leftNode;

        parentNode = &parser->nodes[parser->nodeIndex];
        expr       =  parser->nodes[parser->nodeIndex].expression;

        if  (expr.type        == ME_OPERATOR &&
            (expr.me_operator == ME_MULTIPLICATION || expr.me_operator == ME_DIVISION))
        {
            oper = true;
            parser->nodeIndex++;
        }
    } 
    while (oper);

    return leftNode;
}

static MathNode* ParsePowerOperation(MathParser* parser)
{        
    assert(parser);

    MathExpression expr  = {};
    MathNode* leftNode   = nullptr;
    MathNode* parentNode = nullptr;
    
    bool oper = false;

    do
    {
        oper = false;

        MathNode* rightNode = ParseParenthesis(parser);

        if (!rightNode || parser->status != MP_NO_ERRORS)
            return nullptr;

        if (leftNode)
        {
            TreeAddLeftNode(parentNode, leftNode);
            TreeAddRightNode(parentNode, rightNode);

            leftNode = parentNode;
        }
        else
            leftNode = rightNode;
        
        if (parser->nodeIndex >= parser->nodesSize)
            return leftNode;

        parentNode = &parser->nodes[parser->nodeIndex];
        expr       =  parser->nodes[parser->nodeIndex].expression;

        if  (expr.type == ME_OPERATOR && expr.me_operator == ME_POWER)
        {
            oper = true;
            parser->nodeIndex++;
        }
    } 
    while (oper);

    return leftNode;
}

static MathNode* ParseUnaryMinus(MathParser* parser)
{
    assert(parser);

    assert(parser->nodeIndex < parser->nodesSize);

    MathNode* curNode = parser->nodes + parser->nodeIndex;

    if (curNode->expression.type == ME_OPERATOR)
    {
        if (parser->nodeIndex + 1 >= parser->nodesSize)
        {
            parser->status = MP_ERR_SYNTAX;
            return nullptr;
        }

        parser->nodeIndex++;

        switch (curNode->expression.me_operator)
        {
            case ME_ADDITION: // Унарный плюс
                return ParseParenthesis(parser);
                break;
            case ME_SUBTRACTION: // Унарный минус
                MathNode* node = ParseParenthesis(parser);

                // Дописать
                break;
        }
    }
}