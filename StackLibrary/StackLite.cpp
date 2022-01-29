#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <cmath>


#include "..\Logs\Logs.h"
#include "StackLite.h"


static Stack* StackResize(Stack *stack, int *error);

static size_t CalculateDecreasedCapacity(const size_t oldCapacity, const size_t stackSize, bool* shouldResize);


int StackConstructor(Stack *stack, const size_t elementSize, const size_t Capacity)
{
    LOG_STACK_DBG("StackConstructor");

    assert(stack);

    int error = STK_NO_ERRORS;

    stack->elementSize   = elementSize;
    stack->stackCapacity = Capacity;
    stack->stackSize     = 0;
    
    if (stack->stackCapacity > 0)
    {
        stack->stackCapacity = (StackMinCapacity > stack->stackCapacity) ? StackMinCapacity : stack->stackCapacity;
        stack->data = calloc(stack->stackCapacity, stack->elementSize);

        if (!stack->data)
        {
            LOG_STACK_ERR_MEMORY;
            error |= STK_ERR_MEMORY;
        }
    }

    return error;
}

void StackDestructor(Stack *stack)
{
    LOG_STACK_DBG("StackDestructor");

    assert(stack);

    if (stack->data)
        free((char*)stack->data - sizeof(int64_t));
    
    memset(stack, 0, sizeof(Stack));
}

int StackPush(Stack *stack, void *data)
{
    LOG_STACK_DBG("StackPush");
    
    assert(stack);
    assert(data);

    int error = STK_NO_ERRORS;

    stack = StackResize(stack, &error);

    memmove((char*)stack->data + stack->elementSize * stack->stackSize++, data, stack->elementSize);

    return error;
}

void* StackPop(Stack *stack, int *error)
{
    LOG_STACK_DBG("StackPop");

    assert(stack);
    //assert(error);

    if (stack->stackSize == 0)
    {
        LOG_STACK_ERR("Попытка извлечь элемент из пустого стека.");
        if (error)
            *error |= STK_ERR_EMPTY;
        return nullptr;
    }
    
    stack->stackSize--;

    stack = StackResize(stack, error);
    
    return (char*)stack->data + (stack->elementSize * stack->stackSize);
}

void* StackGetElemAt(const Stack* stack, const size_t index)
{
    assert(stack);

    if (index > stack->stackSize)
        return nullptr;
    
    return (char*)stack->data + index * stack->elementSize;
}

static Stack* StackResize(Stack *stack, int *error)
{
    LOG_STACK_DBG("StackResize");

    assert(stack);
    //assert(error);

    //size_t oldCapacity = stack->stackCapacity;
    bool shouldResize = false;

    if (stack->stackCapacity == 0)
    {
        stack->stackCapacity = StackMinCapacity;
        shouldResize = true;
    }
    else if (stack->stackSize >= stack->stackCapacity)
    {
        if (stack->stackSize < stack->stackCapacity * 2)
            stack->stackCapacity *= StackCapacityScaleCoefficient;
        else
            stack->stackCapacity = stack->stackSize;
        shouldResize = true;
    }
    else
    {
        stack->stackCapacity = CalculateDecreasedCapacity(stack->stackCapacity, stack->stackSize, &shouldResize);
    }
        
    if (shouldResize)
    {
        void *reallocResult = realloc(stack->data, stack->stackCapacity * stack->elementSize);

        if (reallocResult == nullptr)
        {
            LOG_STACK_ERR_MEMORY;
            if (error)
                *error |= STK_ERR_MEMORY;
            return nullptr;
        }

        stack->data = (char*)reallocResult;
    }

    return stack;
}

static size_t CalculateDecreasedCapacity(const size_t oldCapacity, const size_t stackSize, bool* shouldResize)
{
    size_t proportionalCapacity = (oldCapacity * (0.5 - StackCapacityDecreaseCoefficient)) > 0 ? 
                                   (size_t)(oldCapacity * (0.5 - StackCapacityDecreaseCoefficient)) : 0;
    size_t deltaCapacity        = (oldCapacity / 2.0 - StackMinCapacity) > 0 ? (size_t)(oldCapacity / 2.0 - StackMinCapacity) : 0;
    size_t calculatedCapacity   = proportionalCapacity < deltaCapacity ? proportionalCapacity : deltaCapacity;

    calculatedCapacity = calculatedCapacity < StackMinCapacity ? StackMinCapacity : calculatedCapacity;
    if (calculatedCapacity > stackSize)
    {
        if (calculatedCapacity != oldCapacity)
            *shouldResize = true;
        return calculatedCapacity;
    }
    else
        return oldCapacity;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
//                               Функции диагностики
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

void $StackDump__(Stack *stack, FILE *file,
    const char *stack_variable_name,
    const char *stack_function_name,
    const char *stack_file,
    const int   stack_line)
{
    LOG_STACK_DBG("StackDump");

    assert(stack);
    assert(file);

    const size_t bufferSize = 1000;
    char buffer[bufferSize] = "";

    snprintf(buffer, bufferSize, "StackDump\nStack [0x%p]: \"%s\" called from % s() at |%s, %d|\n",
        stack,
        stack_variable_name,
        stack_function_name, 
        stack_file,
        stack_line);

    fputs(buffer, file);

    fputs("Structure:\n{\n", file);

    if (stack)
    {
        const int leftOffset1 = 4 - 1;// число пробелов, задающих отступ слева. 4 пробела
        const int leftOffset2 = 8 - 1;// число пробелов, задающих отступ слева. 8 пробелов
        
        snprintf(buffer, bufferSize, 
            "%*s elementSize   = %zd\n"
            "%*s stackSize     = %zd\n"
            "%*s stackCapacity = %zd\n\n    data [0x%p]:\n    {\n",
            leftOffset1, "", stack->elementSize,
            leftOffset1, "", stack->stackSize,
            leftOffset1, "", stack->stackCapacity, stack->data);

        fputs(buffer, file);
        
        if (stack->data)
        {
            size_t capacity = stack->stackCapacity;
            size_t size     = stack->stackSize;
            int    *data    = (int*)stack->data;


            for (size_t st = 0; st < capacity; st++)
            {
                snprintf(buffer, bufferSize, "%*s %c[%zd] = %d\n", leftOffset2, "", (st <= size) ? '*' : ' ', st, data[st]);
                fputs(buffer, file);
            }
        }

        fputs("    }\n", file);
    }

    fputs("}\n", file);

    fflush(file);
}