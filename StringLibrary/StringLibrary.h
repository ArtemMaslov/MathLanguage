#ifndef STRING_LIBRARY_H
#define STRING_LIBRARY_H


#include <stdio.h>


struct String
{
    char*  ptr;
    size_t length;
};

struct Text
{
    char*   buffer;
    size_t  bufferSize;
    size_t  stringsCount;
    String* strings;
};

enum StringLibErr
{
    STR_NO_ERRORS  = 0,
    STR_ERR_MEMORY = 1 << 0,
};

bool ReadFile(Text* text, FILE* file);

void ParseFileToLines(Text* text);

void WriteTextToFile(Text* text, const char* fileName);

size_t GetFileSize(FILE* file);

void TextDestructor(Text* text);


#endif
