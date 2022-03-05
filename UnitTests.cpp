#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


#include "UnitTests.h"
#include "Math\LanguageTree.h"
#include "Parser\Lexer.h"
#include "StringLibrary\StringLibrary.h"
#include "Logs\Logs.h"


#define CLEAR_MEMORY       goto clear_memory
#define CLEAR_MEMORY_POINT clear_memory
#define CODE_DIRECTORY     "Programs\\Simple syntax"

const size_t bufferSize = 1000;

static void __unitTests(void (*testFunction)(FILE * file));

static void __lexerTestFunction(FILE* file);

static void __parserTestFunction(FILE * file);


void LexerUnitTests()
{
    __unitTests(__lexerTestFunction);
}

void ParserUnitTests()
{
    __unitTests(__parserTestFunction);
}

static void __parserTestFunction(FILE* file)
{
    LanguageLexer lexer = {};
    Text text = {};

    if (!ReadFile(&text, file))
    {
        fprintf(stderr, "Ошибка чтения файла.\n");

        CLEAR_MEMORY;
    }

    if (LexerConstructor(&lexer) == LXR_NO_ERRORS &&
        LexerGetTokens(&lexer, &text) == LXR_NO_ERRORS)
    {
        //fputs("Тест пройден.\n", stdout);
        Tree tree = {};

        if (!tree.NodesArrayPtr)
        {
            LOG_CALC_ERR("Ошибка выделения памяти");

            CLEAR_MEMORY;
        }

        LexerGraphicDump(&tree);

        TreeDestructor(&tree);
    }
    else
    {
        fprintf(stderr, "Ошибка лексера. Подробнее смотри в дебагере :(\n");

        CLEAR_MEMORY;
    }

CLEAR_MEMORY_POINT:

    TextDestructor(&text);
    LexerDestructor(&lexer);
}

static void __lexerTestFunction(FILE* file)
{
    LanguageLexer lexer = {};
    Text text = {};

    if (!ReadFile(&text, file))
    {
        fprintf(stderr, "Ошибка чтения файла.\n");

        CLEAR_MEMORY;
    }

    if (LexerConstructor(&lexer) == LXR_NO_ERRORS &&
        LexerGetTokens(&lexer, &text) == LXR_NO_ERRORS)
    {
        fputs("Тест пройден.\n", stdout);

        Tree tree = {};

        tree.NodesArrayPtr = (TreeNode*)calloc(lexer.TokenIndex, sizeof(TreeNode));
        tree.NodesArrayCurSize = lexer.TokenIndex;

        if (!tree.NodesArrayPtr)
        {
            LOG_CALC_ERR("Ошибка выделения памяти");

            CLEAR_MEMORY;
        }

        for (size_t st = 0; st < lexer.TokenIndex; st++)
        {
            tree.NodesArrayPtr[st].ChildCount = 0;

            tree.NodesArrayPtr[st].NodeLeft = nullptr;
            tree.NodesArrayPtr[st].NodeRight = nullptr;
            tree.NodesArrayPtr[st].Parent = nullptr;

            tree.NodesArrayPtr[st].Value = lexer.Tokens[st];
        }

        LexerGraphicDump(&tree);

        TreeDestructor(&tree);
    }
    else
    {
        fprintf(stderr, "Ошибка лексера. Подробнее смотри в дебагере :(\n");

        CLEAR_MEMORY;
    }

CLEAR_MEMORY_POINT:

    TextDestructor(&text);
    LexerDestructor(&lexer);
}

static void __unitTests(void (*testFunction)(FILE* file))
{
    WIN32_FIND_DATA fileData = {};
    HANDLE handle = FindFirstFile(L"..\\" CODE_DIRECTORY "\\*.math", &fileData);

    if (handle != INVALID_HANDLE_VALUE)
    {
        char    fileName[bufferSize] = "";
        wchar_t filePath[bufferSize] = L"";
        FILE* file = nullptr;

        do
        {
            if (!WideCharToMultiByte(CP_UTF8, 0, fileData.cFileName, -1, fileName, bufferSize, NULL, NULL))
            {
                fputs("Ошибка преобразования строки в utf8.\n", stderr);

                CLEAR_MEMORY;
            }

            fprintf(stdout, "Тестирование функции на файле: \"%s\"\n", fileName);

            swprintf(filePath, bufferSize, L"..\\" CODE_DIRECTORY "\\%s", fileData.cFileName);

            file = _wfopen(filePath, L"r");

            if (file == nullptr)
            {
                fprintf(stderr, "Ошибка открытия файла \"%s\" по пути \"%ws\".\n", fileName, filePath);

                CLEAR_MEMORY;
            }

            testFunction(file);

            CLEAR_MEMORY_POINT:

            fputs("\nНажмите любую клавишу, чтобы перейти к следующему файлу.\n", stdout);
            fgetc(stdin);
        } while (FindNextFile(handle, &fileData));

        FindClose(handle);
    }
    else
        fputs("Ошибка: файлы с кодом не найдены.\n", stderr);
}
