#include <stdio.h>
#include <string.h>
#include <windows.h>


#include "Config.h"
#include "UnitTests.h"
#include "LanguageGrammar.h"
#include "Logs\Logs.h"
#include "MyStdLib.h"

//#pragma execution_character_set( "utf-8" )

int main(int argc, char* argv[])
{
    //SetConsoleCP(CP_UTF8);
    SetConsoleCP(CP_WIN_CYRILLIC);
    SetConsoleOutputCP(CP_UTF8);

    if (LogsConstructor() != LOG_NO_ERRORS)
    {
        fputs("Ошибка создания файла логов", stderr);
        return false;
    }

    ConfigGrammarTokens();

    LexerUnitTests();

    LogsDestructor();
}