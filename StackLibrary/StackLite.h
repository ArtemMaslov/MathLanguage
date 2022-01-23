#ifndef STACK_H_
#define STACK_H_


#include <stdint.h>

/**
 * @brief ��������� ������ ����.
*/
struct Stack
{
    size_t  elementSize;   /// ������ ������ ��������.
    size_t  stackSize;     /// ������� ������ �����. ���������� ���������, ����������� � ����� � ������ ������.
    size_t  stackCapacity; /// ����������� �����, ������������ ����� ���������, ������� ����� ������� ����.

    void*   data;          /// ������ ������.
};

/**
 * @brief ������ ����� ��� ������ ���������� �� ������� ����� � ����.
*/
const char StackErrorStrings[][40] =
{
    "��� ������.",
    "�� ������� ������.",
    "���� ������."
};

/**
 * @brief ������������ ������ �����. ������ ������ ������������ ����������� ��� �����. `0` ��������, ��� ������ ���.
*/
enum StackError
{
    STK_NO_ERRORS  = 0 << 0,
    STK_ERR_MEMORY = 1 << 1,
    STK_ERR_EMPTY  = 1 << 2,
};

/// @brief ����������� ���������� ������ �����.
/// ���� ����� ��������� ������ ������ ���� ������� �����, ���� �� ������� STACK_MIN_CAPACITY.
const size_t StackMinCapacity = 32;
/// @brief ����������� ����������/������� ������� �����. ������ ���� ������ 1.
/// ��� ���������� ������������, ����������� ���������� ��������� ������.
/// ��� ���������� ������������, ���� ����� �������� ������ ������.
/// ����������� ����� ����� ������������� � STACK_CAPACITY_SCALE_COEFFICIENT ���, ���� stackSize >= stackCapacity.
/// ����������� ����� ����� �����������, ���� stackSize < newStackCapacity
/// newStackCapacity = min ( oldCapacity * (0.5 - STACK_CAPACITY_DECREASE_COEFFICIENT) ; abs(oldCapacity / 2.0 - STACK_MIN_CAPACITY) )
/// ��� ����, ���� � ����� ���� ��������, �� ����������� ����� �� ����� ����� ������ STACK_MIN_CAPACITY.
/// ���� stackSize == 0, �� ����������� ����� ��������������� ������ 0.
const size_t StackCapacityScaleCoefficient = 2;
/// @brief ����������� ���������� ����������� �����. ������ ���� �� ������ 0, �� ������ 0.5.
/// ��� ���������� ������������, ����������� ���������� ��������� ������.
/// ��� ���������� ������������, ���� ����� �������� ������ ������.
const double StackCapacityDecreaseCoefficient = 0.1;

/**
 * @brief             ����������� �����.
 * @param stack       ��������� �� ����.
 * @param elementSize ������ ������ �������� � ������� data.
 * @param Capacity    ��������� ����������� �����. �� ��������� 0. ���� �������� ������ 0, �� ��� ������ ����� �������� ������.
 * @return            ��� ������, 0 - � ������ ��������� ����������.
*/
int StackConstructor(Stack *stack, const size_t elementSize, const size_t Capacity = 0);

/**
 * @brief       ���������� �����.
 * @param stack ��������� �� ����.
*/
void  StackDestructor(Stack *stack);

/**
 * @brief       ��������� ������� � ����� �����.
 * @param stack ��������� �� ����.
 * @param value ��������� �� ��������, ������� ����� �������� � ����.
 * @return      ��� ������, 0 - � ������ ��������� ����������.
*/
int   StackPush(Stack *stack, void *expression);

/**
 * @brief       ������� ������� �� ����� ����� � ���������� ���.
 * @param stack ��������� �� ����.
 * @param error ���� �� nullptr, �� ���� ���������� ����� ��������� ���� ������, ������� ��������� �� ����� ������ �������.
 * @return      �������� ��������� �� ����� ��������, nullptr - � ������ ������.
*/
void* StackPop(Stack *stack, int *error = nullptr);

/**
 * @brief       ��������� ���� �� ������� ������.
 * @param stack ��������� �� ����.
 * @return      ���������� ���� ������ �������� ��������� �����.
*/
int   ValidateStack(Stack *stack);

void* StackGetElemAt(const Stack* stack, const size_t index);

//#define StackDump(stack, file, programm_function_name, programm_file, programm_line) \
//StackDump_(stack, file, #stack, __FUNCTION__, __FILE__, __LINE__, programm_function_name, programm_file, programm_line);

/**
 * @brief       ������� ��������� ���������� � ������� ��������� �����.
 * @param stack ��������� �� ����.
 * @param file  ��������� �� ����� ������
*/
#define StackDump(stack, file) \
    $StackDump__(stack, file, #stack, __FUNCTION__, __FILE__, __LINE__);

/**
 * @brief                        ���������� �������. ����������� StackDump ������.
 * @param stack                  ��������� �� ����.
 * @param file                   ��������� �� ����� ������.
 * @param varName    ��� ���������� � ���������� StackLibrary.
 * @param funcName    ��� ������� � ���������� StackLibrary.
 * @param fileName             ���� ���������� StackLibrary.
 * @param codeLine             ����� ������ ���� � ���������� StackLibrary.
*/
void $StackDump__(Stack *stack, FILE *file,
    const char *varName,
    const char *funcName,
    const char *fileName,
    const int   codeLine);


#endif