#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "MyStdLib.h"
#include "StringLibrary\StringLibrary.h"
#include "Logs\Logs.h"


bool SkipSpaceSymbols(char** ptr)
{
    assert(ptr);

    while (**ptr == ' ' || **ptr == '\r' || **ptr == '\n' || **ptr == '\t')
        (*ptr)++;

    return **ptr != '\0';
}

bool CheckCorrectParenthesis(const Text* text)
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