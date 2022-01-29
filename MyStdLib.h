#ifndef MY_STD_LIB_H_
#define MY_STD_LIB_H_


#include <assert.h>
#include "StringLibrary\StringLibrary.h"


/**
 * @brief     Пропускает пробельные символы.
 * @param ptr Указатель на строку символов
 * @return    false, если строка символов закончилась и был обнаружен '\0'.
*/
bool SkipSpaceSymbols(char** ptr);

bool CheckCorrectParenthesis(const Text* text);


#endif