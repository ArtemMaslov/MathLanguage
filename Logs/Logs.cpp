#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <assert.h>
#include <stdarg.h>

#include "Logs.h"


#define LOGS_FOLDER "logs\\"

struct LogFile
{
    FILE* file;
    LogSignature sig;
    const char* caption;
    const char* fileName;
};

size_t CreateGraphCallCount;

const int LogFilesSize = 10;
LogFile   LogFiles[LogFilesSize];
bool      LogFilesCreated = false;

int       TextOffset = 0;


static int LogConstructor(const char* logFileName, const char* caption, const LogSignature sig);


int LogsConstructor()
{
    LogFilesCreated = true;
    
    system("rd " LOGS_FOLDER "/s /q");
    system("md " LOGS_FOLDER);

    int status = LOG_NO_ERRORS;

    status |= LogConstructor(LOGS_FOLDER "log_general.html",    "Общий лог программы.",               LOG_SIG_GENERAL);

    status |= LogConstructor(LOGS_FOLDER "log_parser.html",     "Лог работы парсера.",                LOG_SIG_PARSER);
    status |= LogConstructor(LOGS_FOLDER "log_compiler.html",   "Лог работы компилятора.",            LOG_SIG_COMPILER);
    status |= LogConstructor(LOGS_FOLDER "log_stack.html",      "Лог работы стека.",                  LOG_SIG_STACK);
    status |= LogConstructor(LOGS_FOLDER "log_math.html",       "Лог работы математического дерева.", LOG_SIG_MATH_TREE);

    atexit(LogsDestructor);

    return status;
}

static int LogConstructor(const char* logFileName, const char* caption, const LogSignature sig)
{
    assert(logFileName);
    assert(caption);
    assert(LogFilesCreated);

    size_t logIndex = (size_t)sig;

    assert(logIndex < LogFilesSize);

    FILE* file = fopen(logFileName, "w");
    
    if (!file)
    {
        LogLine("", LOG_LVL_ERROR, LOG_SIG_GENERAL, false);
        printf("Ошибка открытия файла с логами. FileName = \"%s\"", logFileName);
        return LOG_ERR_FILE_OPENING;
    }
    
    LogFiles[logIndex].file     = file;
    LogFiles[logIndex].file     = fopen(logFileName, "w");
    LogFiles[logIndex].fileName = logFileName;
    LogFiles[logIndex].caption  = caption;
    LogFiles[logIndex].sig      = sig;
            
    const size_t bufferSize  = 1000;
    const size_t bufferTimeSize = 40;

    char  bufferTime[bufferTimeSize] = "";
    char  buffer[bufferSize] = "";

    time_t rawTime = time(nullptr);
    tm*    curTime = localtime(&rawTime);
    strftime(bufferTime, bufferTimeSize, "%H:%M:%S %d.%m.%Y", curTime);

    //***\\---//***\\-----//***\\---//***\\

    snprintf(buffer, bufferSize, 
        "<html>\n"
        "<head><title>%s</title><style>font {line-height: 1.2;} body {background-color: #303030;  font-size: 20} head {background-color: #303030;}</style></head>\n"
        "<body>\n"
        "<h1><font color=\"99B333\">%s %s.</font></h1>\n", caption, caption, bufferTime);
    
    fputs(buffer, file);

    //***\\---//***\\-----//***\\---//***\\

    TextOffset = ftell(file);

    fputs("</body>\n"
          "</html>\n", file);

    TextOffset -= ftell(file);
    
    fflush(file);
    
    return LOG_NO_ERRORS;
}

/**
 * @brief      Закрывает файлы логов.
*/
void LogsDestructor()
{
    assert(LogFilesCreated);

    for (size_t st = 0; st < LogFilesSize; st++)
    {
        LogFile logFile = LogFiles[st];

        if (logFile.file)
            fclose(logFile.file);
    }
}

/**
 * @brief                    Добавляет строку в файл логов.
 * @param file               Указатель на поток вывода.
 * @param message            Строка, которую необходимо добавить.
 * @param dublicateToConsole Если true, то дублирует сообщения в консоль. По умолчанию false
*/
void $LogLine__(const char* message, LogLevel logLevel, const LogSignature sig, bool dublicateToConsole,
                const char* funcName, const char* fileName, int logLine)
{
    assert(message);
    assert(funcName);
    assert(fileName);
    assert(LogFilesCreated);

    int status = LOG_NO_ERRORS;

    static unsigned long long callCounter = 0;
    callCounter++;

    const size_t bufferSize  = 1000;
    const size_t bufferTimeSize = 40;

    char  bufferTime[bufferTimeSize] = "";
    char  buffer[bufferSize] = "";

    const char* logColor     = nullptr;
    
    size_t logIndex = (size_t)sig;
    assert(logIndex < LogFilesSize);

    switch (logLevel)
    {
        case LOG_LVL_DEBUG:
            logColor = "FFA000";
            break;
        case LOG_LVL_WARNING:
            logColor = "FF4D00";
            break;
        case LOG_LVL_ERROR:
            logColor = "D53032";
            break;
        default:
            logColor = "EAE6CA";
            break;
    }
    
    //***\\---//***\\-----//***\\---//***\\

    fseek(LogFiles[logIndex].file, TextOffset, SEEK_END);

    time_t rawTime = time(nullptr);
    tm*    curTime = localtime(&rawTime);
    strftime(bufferTime, bufferTimeSize, "%H:%M:%S", curTime);
        
    //***\\---//***\\-----//***\\---//***\\
    
    snprintf(buffer, bufferSize, "<pre>"
            "<font color=\"%s\">%-.8s [%-.8s] > </font>"
            "<font color = \"D5DED9\">%s</font>"
            "<font color = \"B0B7C6\"> in %s</font>"
            "<font color = \"FFD700\"> at %d line</font>\n"
            "<font color = \"ADDFAD\">\t %s</font>"
            "</pre>\n",
            logColor, bufferTime, LogLevelMessages[(int)logLevel],
            funcName,
            fileName,
            logLine,
            message);

    fputs(buffer, LogFiles[logIndex].file);

    //***\\---//***\\-----//***\\---//***\\

    if (dublicateToConsole)
        fputs(message, stderr);

    fputs("</body>\n"
          "</html>\n", LogFiles[logIndex].file);

    fflush(LogFiles[logIndex].file);
}

void $LogFLine__(LogLevel logLevel, const LogSignature sig, bool dublicateToConsole,
                const char* funcName, const char* fileName, int logLine, const char* format, ...)
{
    assert(format);
    assert(funcName);
    assert(fileName);
    assert(LogFilesCreated);

    static unsigned long long callCounter = 0;
    callCounter++;

    const size_t bufferSize  = 1000;
    char  buffer[bufferSize] = "";

    const char* logColor     = nullptr;
    
    size_t logIndex = (size_t)sig;
    assert(logIndex < LogFilesSize);

    switch (logLevel)
    {
        case LOG_LVL_DEBUG:
            logColor = "FFA000";
            break;
        case LOG_LVL_WARNING:
            logColor = "FF4D00";
            break;
        case LOG_LVL_ERROR:
            logColor = "D53032";
            break;
        default:
            logColor = "EAE6CA";
            break;
    }
    
    //***\\---//***\\-----//***\\---//***\\
    
    fseek(LogFiles[logIndex].file, TextOffset, SEEK_END);

    time_t rawTime = time(nullptr);
    tm*    curTime = localtime(&rawTime);

    strftime(buffer, bufferSize, "%H:%M:%S ", curTime);
    
    fputs(buffer, LogFiles[logIndex].file);

    //***\\---//***\\-----//***\\---//***\\

    snprintf(buffer, bufferSize, "<pre>"
            "<font color=\"%s\">[%-.8s] > </font>"
            "<font color = \"BDDA57\">%150s</font>"
            "<font color = \"B0B7C6\"> in %50s</font>"
            "<font color = \"FFD700\"> at %d line</font>\n"
            "<font color = \"ADDFAD\">",
            logColor, LogLevelMessages[(int)logLevel],
            funcName,
            fileName,
            logLine);
    
    fputs(buffer, LogFiles[logIndex].file);
    
    //***\\---//***\\-----//***\\---//***\\

    va_list args;
	va_start(args, format);
    vsnprintf(buffer, bufferSize, format, args);
    va_end(args);
    
    if (dublicateToConsole)
        fputs(buffer, stderr);
    
    fputs(buffer, LogFiles[logIndex].file);
    
    //***\\---//***\\-----//***\\---//***\\

    fputs("</font></pre>\n"
          "</body>\n"
          "</html>\n", LogFiles[logIndex].file);

    fflush(LogFiles[logIndex].file);
}


//void CreateHtmlGraphicLog(const char* imagesName)
//{
//    assert(imagesName);
//
//    FILE* file = fopen(GRAPH_LOG_FOLDER "GraphicLog.html", "w");
//    if (!file)
//    {
//        puts("Ошибка открытия файла.");
//        return;
//    }
//
//    const char* pattern = "<html>\n"
//                          "<head><title>Лог программы \"Дифференциатор\".</title><style>font{line - height: 0.8; } body{background - color: #404040; } head{background - color: #404040; }</style></head>\n"
//                          "<body>\n"
//                          "<h1><font color = \"99B333\">Лог программы \"Дифференциатор\".</font></h1>\n";
//
//    const char* ending  = "</body>\n</html>";
//
//    fputs(pattern, file);
//
//    for (int st = 1; st <= CreateGraphCallCount; st++)
//        fprintf(file, "<img src = \"%s%d.png\">\n", imagesName, st);
//
//    fputs(ending, file);
//
//    fclose(file);
//}


#undef LOGS_FOLDER