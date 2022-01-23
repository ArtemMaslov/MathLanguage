#include <stdio.h>


#include "..\Logs\Logs.h"

int main(int argc, char* argv[])
{
    if (LogsConstructor() != LOG_NO_ERRORS)
    {
        fputs("Ошибка создания файла логов", stderr);
        return false;
    }

    LogsDestructor();
}