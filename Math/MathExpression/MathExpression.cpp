#include <stdio.h>
#include <assert.h>
#include <string.h>


#include "MathExpression.h"
#include "..\..\MParser\MParser.h"
#include "..\..\Logs\Logs.h"


static int CheckStrForMathExpressionType(const char* ptr, const char* meStrings,
                                         size_t stringsCount, size_t stringsLength, size_t count);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

bool MathExpressionEqual(const MathExpression* expr1, const MathExpression* expr2)
{
    assert(expr1);
    assert(expr2);

    if (expr1->type != expr2->type)
    {
        return false;
    }

    switch (expr1->type)
    {
        case ME_NUMBER:
            if (expr1->me_number == expr2->me_number)
                return true;
            break;
        case ME_CONSTANT:
            if (expr1->me_constant == expr2->me_constant)
                return true;
            break;
        case ME_OPERATOR:
            if (expr1->me_operator == expr2->me_operator)
                return true;
            break;
        case ME_VARIABLE:
            if (expr1->me_variable == expr2->me_variable)
                return true;
            break;
        case ME_FUNCTION:
            if (expr1->me_function == expr2->me_function)
                return true;
            break;
        default:
            LOG_MATH_TREE_ERR("Неопознанный тип математического выражения.");
            break;
    }
    return false;
}

void PrintMathExpression(const MathExpression* expr, FILE* file)
{
    assert(expr);
    assert(file);

    switch (expr->type)
    {
        case ME_NUMBER:
            fprintf(file, "%.6lg", expr->me_number);
            break;
        case ME_CONSTANT:
            fputs(MathConstantNames[expr->me_constant], file);
            break;
        case ME_OPERATOR:
            fputc(MathOperatorNames[expr->me_operator], file);
            break;
        case ME_VARIABLE:
            fputs(expr->me_variable, file);
            break;
        case ME_FUNCTION:
            fputs(MathFunctionNames[expr->me_function], file);
            break;
        default:
            LOG_MATH_TREE_ERR("Неизвестный тип математического выражения.");
            break;
    }
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

static int CheckStrForMathExpressionType(const char* ptr, const char* meStrings,
                                          size_t stringsCount, size_t stringsLength, size_t count)
{
    assert(ptr);
    assert(meStrings);

    for (size_t st = 0; st < stringsCount; st++)
    {
        const char* _meStr = meStrings + st * stringsLength;
        if (strncmp(_meStr, ptr, count) == 0)
        {
            size_t strLen = strlen(_meStr);
            const char* unparsed = ptr + strLen;
            size_t unparsedCount = count - strLen;
            if (SkipInputString(unparsed, unparsedCount))
            {
                return st;
            }
        }
    }
    return -1;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\