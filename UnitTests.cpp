#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


#include "UnitTests.h"
#include "Math\LanguageTree.h"
#include "Parser\Lexer.h"
#include "StringLibrary\StringLibrary.h"
#include "Logs\Logs.h"


#define CLEAR_MEMORY goto clear_memory
#define CLEAR_MEMORY_POINT clear_memory

void LexerUnitTests()
{
    WIN32_FIND_DATA fileData = {};
    HANDLE handle = FindFirstFile(L"..\\Programs\\*.math", &fileData);

    if (handle != INVALID_HANDLE_VALUE)
    {
        LanguageLexer lexer = {};
        Text text = {};
        const size_t bufferSize = 1000;
        wchar_t filePath[bufferSize] = L"";
        char    fileName[bufferSize] = "";
        FILE* file = nullptr;

        do
        {
            if (!WideCharToMultiByte(CP_UTF8, 0, fileData.cFileName, -1, fileName, bufferSize, NULL, NULL))
            {
                fputs("Ошибка преобразования строки в utf8.\n", stderr);

                CLEAR_MEMORY;
            }
            
            fprintf(stdout, "Тестирование лексера на файле: \"%s\"\n", fileName);

            swprintf(filePath, bufferSize, L"..\\Programs\\%s", fileData.cFileName);

            file = _wfopen(filePath, L"r");

            if (file == nullptr)
            {
                fprintf(stderr, "Ошибка открытия файла \"%s\" по пути \"%ws\".\n", fileName, filePath);

                CLEAR_MEMORY;
            }

            if (!ReadFile(&text, file))
            {
                fprintf(stderr, "Ошибка чтения файла \"%s\".\n", fileName);

                CLEAR_MEMORY;
            }

            if (LexerConstructor(&lexer, &text) != LXR_NO_ERRORS)
            {
                fprintf(stderr, "Ошибка лексера. Файл: \"%s\".\n", fileName);

                CLEAR_MEMORY;
            }
            else
            {
                fputs("Тест пройден.\n", stdout);

                Tree tree = {};

                tree.nodesArrayPtr  = (TreeNode*)calloc(lexer.TokenIndex, sizeof(TreeNode));
                tree.nodesArraySize = lexer.TokenIndex;

                if (!tree.nodesArrayPtr)
                {
                    LOG_CALC_ERR("Ошибка выделения памяти");

                    CLEAR_MEMORY;
                }

                for (size_t st = 0; st < lexer.TokenIndex; st++)
                {
                    tree.nodesArrayPtr[st].ChildCount = 0;
                        
                    tree.nodesArrayPtr[st].NodeLeft   = nullptr;
                    tree.nodesArrayPtr[st].NodeRight  = nullptr;
                    tree.nodesArrayPtr[st].Parent     = nullptr;
                        
                    tree.nodesArrayPtr[st].Value = lexer.Tokens[st];
                }

                LexerGraphicDump(&tree);

                TreeDestructor(&tree);
            }

        CLEAR_MEMORY_POINT:

            TextDestructor(&text);
            LexerDestructor(&lexer);

            fputs("Нажмите любую клавишу, чтобы перейти к следующему файлу.\n", stdout);
            fgetc(stdin);
        }
        while (FindNextFile(handle, &fileData));

        FindClose(handle);
    }
    else
        fputs("Ошибка: файлы с кодом не найдены.\n", stderr);
}