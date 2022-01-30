#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>


#include "..\Logs\Logs.h"
#include "StringsUtf8.h"

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

struct Utf8Letter 
{
    char UpperCase[4];
    char LowerCase[4];
};

const Utf8Letter RussianLetters[] = 
{
    {"А", "а"},
    {"Б", "б"},
    {"В", "в"},
    {"Г", "г"},
    {"Д", "д"},
    {"Е", "е"},
    {"Ё", "ё"},
    {"Ж", "ж"},
    {"З", "з"},
    {"И", "и"},
    {"Й", "й"},
    {"К", "к"},
    {"Л", "л"},
    {"М", "м"},
    {"Н", "н"},
    {"О", "о"},
    {"П", "п"},
    {"Р", "р"},
    {"С", "с"},
    {"Т", "т"},
    {"У", "у"},
    {"Ф", "ф"},
    {"Х", "х"},
    {"Ц", "ц"},
    {"Ч", "ч"},
    {"Ш", "ш"},
    {"Щ", "щ"},
    {"Ъ", "ъ"},
    {"Ы", "ы"},
    {"Ь", "ь"},
    {"Э", "э"},
    {"Ю", "ю"},
    {"Я", "я"},
};

const Utf8Letter EnglishLetters[] = 
{
    {"A", "a"},
    {"B", "b"},
    {"C", "c"},
    {"D", "d"},
    {"E", "e"},
    {"F", "f"},
    {"G", "g"},
    {"H", "h"},
    {"I", "i"},
    {"J", "j"},
    {"K", "k"},
    {"L", "l"},
    {"M", "m"},
    {"N", "n"},
    {"O", "o"},
    {"P", "p"},
    {"Q", "q"},
    {"R", "r"},
    {"S", "s"},
    {"T", "t"},
    {"U", "u"},
    {"V", "v"},
    {"W", "w"},
    {"X", "x"},
    {"Y", "y"},
    {"Z", "z"}
};

struct Utf8Alphabet
{
    const Utf8Letter* const Alphabet;
    const size_t            LettersCount;
    const unsigned int      FirstUpperLetter;
    const unsigned int      LastUpperLetter;

    const unsigned int      FirstLowerLetter;
    const unsigned int      LastLowerLetter;
};

const Utf8Alphabet RussianAlphabet = 
{
    RussianLetters,
    sizeof(RussianLetters) / sizeof(RussianLetters[0]),
    Utf8GetCharCode("А"),
    Utf8GetCharCode("Я"),
    Utf8GetCharCode("а"),
    Utf8GetCharCode("я"),
};

const Utf8Alphabet EnglishAlphabet = 
{
    EnglishLetters,
    sizeof(EnglishLetters) / sizeof(EnglishLetters[0]),
    Utf8GetCharCode("A"),
    Utf8GetCharCode("Z"),
    Utf8GetCharCode("а"),
    Utf8GetCharCode("z"),
};

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

/**
 * @brief        Приводит символы верхнего регистра в нижний регистр. Символы нижнего регистра остаются без изменений.
 *               Работает только с латиницей и кириллицей.
 * @param string Строка, закодированная в Utf8
*/
void Utf8ToLower(char* string)
{
    assert(string);

    while (*string)
    {
        unsigned int charCode = Utf8GetCharCode(string);

        if (charCode >= RussianAlphabet.FirstUpperLetter && charCode <= RussianAlphabet.LastUpperLetter)
        {
            charCode += RussianAlphabet.FirstLowerLetter - RussianAlphabet.FirstUpperLetter;

            Utf8SetCharCode(string, charCode);
        }
        else if (charCode >= EnglishAlphabet.FirstUpperLetter && charCode <= EnglishAlphabet.LastUpperLetter)
        {
            charCode += EnglishAlphabet.FirstLowerLetter - EnglishAlphabet.FirstUpperLetter;

            Utf8SetCharCode(string, charCode);
        }

        Utf8GetNextChar(&string);
    }
}

/**
 * @brief        Приводит символы верхнего регистра в нижний регистр. Символы нижнего регистра остаются без изменений.
 *               Работает только с латиницей и кириллицей.
 * @param string Строка, закодированная в Utf8
*/
void Utf8ToUpper(char* string)
{
    assert(string);

    while (*string)
    {
        unsigned int charCode = Utf8GetCharCode(string);

        if (charCode >= RussianAlphabet.FirstLowerLetter && charCode <= RussianAlphabet.LastLowerLetter)
        {
            charCode += RussianAlphabet.FirstUpperLetter - RussianAlphabet.FirstLowerLetter;

            Utf8SetCharCode(string, charCode);
        }
        else if (charCode >= EnglishAlphabet.FirstLowerLetter && charCode <= EnglishAlphabet.LastLowerLetter)
        {
            charCode += EnglishAlphabet.FirstUpperLetter - EnglishAlphabet.FirstLowerLetter;

            Utf8SetCharCode(string, charCode);
        }

        Utf8GetNextChar(&string);
    }
}

void Utf8GetNextChar(char** string)
{
    assert(string);
    assert(*string);

    if (**string >= 0xF0)      // 4 байта
    {
        (*string) += 4;
    }
    else if (**string >= 0xE0) // 3 байта
    {
        (*string) += 3;
    }
    else if (**string >= 0xC0) // 2 байта
    {
        (*string) += 2;
    }
    else                       // 1 байт
    {
        (*string) += 1;
    }
}

unsigned int Utf8GetCharCode(const char* ptr)
{
    assert(ptr);

    unsigned int result     = 0;
    size_t       bytesCount = 0;

    if (*ptr >= 0xF0)      // 4 байта
    {
        bytesCount = 4;
        result = (*ptr) & 0x07;
    }
    else if (*ptr >= 0xE0) // 3 байта
    {
        bytesCount = 3;
        result = (*ptr) & 0x0F;
    }
    else if (*ptr >= 0xC0) // 2 байта
    {
        bytesCount = 2;
        result = (*ptr) & 0x1F;
    }
    else                   // 1 байт
    {
        bytesCount = 1;
        result = (*ptr) & 0x7F;
    }
    
    ptr++;
    for (size_t st = 0; st < bytesCount - 1; st++)
    {
        result << 6;
        result |= (*ptr) & 0x3F;
        ptr++;
    }

    return result;
}

void Utf8SetCharCode(char* ptr, unsigned int charCode)
{
    if (charCode >= 0xF0 << 24)      // 4 байта
    {
        ptr[0] = charCode & (0x07 << 24);
        ptr[1] = charCode & (0x3F << 16);
        ptr[2] = charCode & (0x3F << 8);
        ptr[3] = charCode & (0x3F);
    }
    else if (charCode >= 0xE0 << 16) // 3 байта
    {
        ptr[0] = charCode & (0x0F << 16);
        ptr[1] = charCode & (0x3F << 8);
        ptr[2] = charCode & (0x3F);
    }
    else if (charCode >= 0xC0 << 8)  // 2 байта
    {
        ptr[0] = charCode & (0x1F << 8);
        ptr[1] = charCode & (0x3F);
    }
    else                             // 1 байт
    {
        *ptr = charCode & 0x7F;
    }
}