#ifndef STACK_H_
#define STACK_H_


#include <stdint.h>

/**
 * @brief Структура данных стек.
*/
struct Stack
{
    size_t  elementSize;   /// Размер одного элемента.
    size_t  stackSize;     /// Текущий размер стека. Количество элементов, находящихся в стеке в данный момент.
    size_t  stackCapacity; /// Вместимость стека, максимальное число элементов, которые может хранить стек.

    void*   data;          /// Массив данных.
};

/**
 * @brief Массив строк для вывода информации об ошибках стека в логи.
*/
const char StackErrorStrings[][40] =
{
    "Без ошибок.",
    "Не хватает памяти.",
    "Стек пустой."
};

/**
 * @brief Перечисление ошибок стека. Каждая ошибка представляет определённый бит числа. `0` означает, что ошибок нет.
*/
enum StackError
{
    STK_NO_ERRORS  = 0 << 0,
    STK_ERR_MEMORY = 1 << 1,
    STK_ERR_EMPTY  = 1 << 2,
};

/// @brief Минимальный допустимый размер стека.
/// Стек может содержать массив данных либо нулевой длины, либо не меньшей STACK_MIN_CAPACITY.
const size_t StackMinCapacity = 32;
/// @brief Коэффициент расширения/сужения размера стека. Должен быть больше 1.
/// При увеличении коэффициента, уменьшается количество выделений памяти.
/// При уменьшении коэффициента, стек будет занимать меньше памяти.
/// Вместимость стека будет увеличиваться в STACK_CAPACITY_SCALE_COEFFICIENT раз, если stackSize >= stackCapacity.
/// Вместимость стека будет уменьшаться, если stackSize < newStackCapacity
/// newStackCapacity = min ( oldCapacity * (0.5 - STACK_CAPACITY_DECREASE_COEFFICIENT) ; abs(oldCapacity / 2.0 - STACK_MIN_CAPACITY) )
/// При этом, если в стеке есть элементы, то вместимость стека не может стать меньше STACK_MIN_CAPACITY.
/// Если stackSize == 0, то вместимость стека устанавливается равной 0.
const size_t StackCapacityScaleCoefficient = 2;
/// @brief Коэффициент уменьшения вместимости стека. Должен быть не меньше 0, не больше 0.5.
/// При увеличении коэффициента, уменьшается количество выделений памяти.
/// При уменьшении коэффициента, стек будет занимать меньше памяти.
const double StackCapacityDecreaseCoefficient = 0.1;

/**
 * @brief             Конструктор стека.
 * @param stack       Указатель на стек.
 * @param elementSize Размер одного элемента в массиве data.
 * @param Capacity    Начальная вместимость стека. По умолчанию 0. Если значение больше 0, то под данные будет выделена память.
 * @return            Код ошибки, 0 - в случае успешного завершения.
*/
int StackConstructor(Stack *stack, const size_t elementSize, const size_t Capacity = 0);

/**
 * @brief       Деструктор стека.
 * @param stack Указатель на стек.
*/
void  StackDestructor(Stack *stack);

/**
 * @brief       Добавляет элемент в конец стека.
 * @param stack Указатель на стек.
 * @param value Указатель на значение, которое нужно добавить в стек.
 * @return      Код ошибки, 0 - в случае успешного завершения.
*/
int   StackPush(Stack *stack, void *expression);

/**
 * @brief       Удаляет элемент из конца стека и возвращает его.
 * @param stack Указатель на стек.
 * @param error Если не nullptr, то этой переменной будут присвоены коды ошибок, которые произошли во время работы функции.
 * @return      Значение удалённого из стека элемента, nullptr - в случае ошибки.
*/
void* StackPop(Stack *stack, int *error = nullptr);

/**
 * @brief       Проверяет стек на наличии ошибок.
 * @param stack Указатель на стек.
 * @return      Возвращает коды ошибок текущего состояния стека.
*/
int   ValidateStack(Stack *stack);

void* StackGetElemAt(const Stack* stack, const size_t index);

//#define StackDump(stack, file, programm_function_name, programm_file, programm_line) \
//StackDump_(stack, file, #stack, __FUNCTION__, __FILE__, __LINE__, programm_function_name, programm_file, programm_line);

/**
 * @brief       Выводит детальную информацию о текущем состоянии стека.
 * @param stack Указатель на стек.
 * @param file  Указатель на поток вывода
*/
#define StackDump(stack, file) \
    $StackDump__(stack, file, #stack, __FUNCTION__, __FILE__, __LINE__);

/**
 * @brief                        Внутренняя функция. Используйте StackDump взамен.
 * @param stack                  Указатель на стек.
 * @param file                   Указатель на поток вывода.
 * @param varName    Имя переменной в библиотеке StackLibrary.
 * @param funcName    Имя функции в библиотеке StackLibrary.
 * @param fileName             Файл библиотеки StackLibrary.
 * @param codeLine             Номер строки кода в библиотеке StackLibrary.
*/
void $StackDump__(Stack *stack, FILE *file,
    const char *varName,
    const char *funcName,
    const char *fileName,
    const int   codeLine);


#endif