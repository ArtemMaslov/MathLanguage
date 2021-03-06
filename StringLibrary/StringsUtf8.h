#ifndef STRINGS_UTF8_H_
#define STRINGS_UTF8_H_


void Utf8ToLower(char* string);

void Utf8ToUpper(char* string);

void Utf8GetNextChar(char** string);

unsigned int Utf8GetCharCode(const char* ptr);

void Utf8SetCharCode(char* ptr, unsigned int charCode);


#endif