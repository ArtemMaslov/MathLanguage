#ifndef LOGS_H_
#define LOGS_H_

#include <stdio.h>

extern int    TextOffset;
extern size_t CreateGraphCallCount;

enum LogError
{
    LOG_NO_ERRORS           = 0,
    LOG_ERR_FILE_OPENING    = 1 << 0,
};

enum LogLevel
{
    LOG_LVL_DEBUG   = 0,
    LOG_LVL_WARNING = 1,
    LOG_LVL_ERROR   = 2
};

const char LogLevelMessages[][8] = 
{
    "DEBUG",
    "WARNING",
    "ERROR"
};

const size_t LogFileMaxSigSize = 4;

/**
 * @brief Сигнатура файла позволяет определить, в какой файл будут записано сообщение.
 *        Значением является индекс в массиве файлов, поэтому 0 <= индекс < LogFilesSize
*/
enum LogSignature
{
    LOG_SIG_GENERAL    = 0,
    LOG_SIG_CALCULATOR = 1,
    LOG_SIG_STACK      = 2,
    LOG_SIG_MATH_TREE  = 3,
    LOG_SIG_PARSER     = 4,
    LOG_SIG_COMPILER   = 5,
};

#define LOG_CALC_ERR(message)                                                       \
    LogLine(message, LOG_LVL_ERROR, LOG_SIG_CALCULATOR, true)

#define LOG_MATH_TREE_ERR(message)                                                  \
    LogLine(message, LOG_LVL_ERROR, LOG_SIG_MATH_TREE,  true)

#define LOG_STACK_ERR(message)                                                      \
    LogLine(message, LOG_LVL_DEBUG, LOG_SIG_STACK,      true)


#define LOG_CALC_ERR_MEMORY                                                         \
    LOG_CALC_ERR("Ошибка выделения памяти")

#define LOG_MATH_TREE_ERR_MEMORY                                                    \
    LOG_MATH_TREE_ERR("Ошибка выделения памяти")

#define LOG_STACK_ERR_MEMORY                                                        \
    LOG_STACK_ERR("Ошибка выделения памяти")

#define LOG_CALC_DBG(message)                                                       \
    LogLine(message, LOG_LVL_DEBUG, LOG_SIG_CALCULATOR, false)

#define LOG_MATH_TREE_DBG(message)                                                  \
    LogLine(message, LOG_LVL_DEBUG, LOG_SIG_MATH_TREE,  false)

#define LOG_STACK_DBG(message)                                                      \
    LogLine(message, LOG_LVL_DEBUG, LOG_SIG_STACK,      false)


int LogsConstructor();

/**
 * @brief      Закрывает файл логов.
*/
void LogsDestructor();

/**
 * @brief Внутренняя функция. Используйте LogLine взамен.
*/
void $LogLine__(const char* message, LogLevel logLevel, const LogSignature sig, bool dublicateToConsole,
                const char* funcName, const char* fileName, int logLine);

/**
 * @brief Внутренняя функция. Используйте LogFLine взамен.
*/
void $LogFLine__(LogLevel logLevel, const LogSignature sig, bool dublicateToConsole,
                const char* funcName, const char* fileName, int logLine, const char* format, ...);

#define LogLine(message, logLevel, sig, dublicateToConsole)                                                 \
    $LogLine__(message, logLevel, sig, dublicateToConsole, __FUNCSIG__, __FILE__, __LINE__)

#define LogFLine(logLevel, sig, dublicateToConsole, format, ...)                                            \
    $LogFLine__(logLevel, sig, dublicateToConsole, __FUNCSIG__, __FILE__, __LINE__, format, __VA_ARGS__)


#endif // !LOGS_H_
