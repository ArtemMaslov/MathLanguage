#ifndef LANGUAGE_GRAMMAR_H_
#define LANGUAGE_GRAMMAR_H_


#include "StringLibrary/StringLibrary.h"

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

enum Constants
{
    // Изменить значение констант можно в файле Config.h.
    ML_CONST_PI      = 0,          // Число пи = 3.14159265358979323846.
    ML_CONST_EXP     = 1,          // Число е  = 2.71828182845904523536.
};

enum SpecialSymbols
{
    ML_SPEC_L_ROUND_BRACKET  = 0,    // '('
    ML_SPEC_R_ROUND_BRACKET  = 1,    // ')'
                                     
    ML_SPEC_L_SQUARE_BRACKET = 2,    // '['
    ML_SPEC_R_SQUARE_BRACKET = 3,    // ']'
                                     
    ML_SPEC_L_CURLY_BRACKET  = 4,    // '{'
    ML_SPEC_R_CURLY_BRACKET  = 5,    // '}'
                                     
    ML_SPEC_POINT            = 6,    // '.'
    ML_SPEC_COMMA            = 12,   // ','
                                     
    ML_SPEC_EQUAL            = 7,    // '='
    ML_SPEC_COLON            = 8,    // ':'
    ML_SPEC_SEMICOLON        = 9,    // ';'
                                     
    ML_SPEC_SINGLE_QUOTES    = 10,   // '\''
    ML_SPEC_DOUBLE_QUOTES    = 11,   // '\"'
};

enum MathOperators
{
    // Бинарные математические операторы.
    ML_MATH_ADDITION               = 0,    // '+' Сложение.
    ML_MATH_SUBTRACTION            = 1,    // '-' Вычитание.
    ML_MATH_MULTIPLICATION         = 2,    // '*' Умножение.
    ML_MATH_DIVISION               = 3,    // '/' Деление.
    ML_MATH_POWER                  = 4,    // '^' Возведение в степень.
    
    // Короткие бинарные математические операторы.
    ML_MATH_SHORT_ADDITION         = 30,    // a += b эквивалентно a = a + b.
    ML_MATH_SHORT_SUBTRACTION      = 31,    // a -= b эквивалентно a = a - b.
    ML_MATH_SHORT_MULTIPLICATION   = 32,    // a *= b эквивалентно a = a * b.
    ML_MATH_SHORT_DIVISION         = 33,    // a /= b эквивалентно a = a / b.
    ML_MATH_SHORT_POWER            = 34,    // a ^= b эквивалентно a = a ^ b.

    // Операторы инкремента и декремента.
    ML_MATH_INCREMENT              = 41,    // Инкремент, увеличивает значение переменной на 1. Но пока неизвестно какой: префиксный или постфиксный?
    ML_MATH_DECREMENT              = 42,    // Декремент, уменьшает значение переменной на 1.   Но пока неизвестно какой: префиксный или постфиксный?
    // Сначала в выражение подставляется значение переменной, и только после вычисления выражения, изменяется значение переменной.
    ML_MATH_POST_INCREMENT         = 41,    // a++ эквивалентно a = a + 1.
    ML_MATH_POST_DECREMENT         = 42,    // a-- эквивалентно a = a - 1.
    // Сначала изменяется значение переменной и только после этого новое значение подставляется в выражение.
    ML_MATH_PRE_INCREMENT          = 43,    // ++a эквивалентно a = a + 1.
    ML_MATH_PRE_DECREMENT          = 44,    // --a эквивалентно a = a - 1.

    // Операторы сравнения.
    ML_MATH_EQUAL                  = 10,   // '=='
    ML_MATH_NOT_EQUAL              = 11,   // '!='
                                   
    ML_MATH_GREATER                = 12,   // '>'
    ML_MATH_LESS                   = 13,   // '<'
    ML_MATH_GREATER_EQUAL          = 14,   // '>='
    ML_MATH_LESS_EQUAL             = 15,   // '<='
                                   
    // Логические операторы.       
    ML_MATH_NOT                    = 20,   // Логическое не.  '!'
    ML_MATH_AND                    = 21,   // Логическое и.   '&&'
    ML_MATH_OR                     = 22,   // Логическое или. '||'
};

enum FunctionalOperators
{
    ML_OPER_IF             = 0,      // Условный оператор. "если".
    ML_OPER_ELSE           = 1,      // Условный оператор. "иначе".
    ML_OPER_ELSEIF         = 2,      // Условный оператор. "иначе если".

    ML_OPER_WHILE          = 10,     // Оператор цикла. "пока".
    ML_OPER_STD_FOR        = 11,     // Оператор цикла. "для всех {инициализация}, {условие окончания}, {шаг}":
    ML_OPER_FOR_ALL        = 12,     // Оператор цикла. "для всех {переменная} от {начальное значение} до {конечное значение} по {шаг}":
};

enum LanguageConstructions
{
    ML_CSTR_PROGRAM_START  = 0,      // "начало программы". Функция, с которой программа начинает своё исполнение.
                                     
    ML_CSTR_FUNCTION_DEF   = 1,      // Определение функции.
    ML_CSTR_FUNCTION_CALL  = 2,      // Вызов функции.
                                     
    ML_CSTR_GLOBAL_VAR_DEF = 3,      // Определение глобальной переменной.
                                     
    ML_CSTR_BLOCK_START    = 4,      // Начало блока кода.
    ML_CSTR_BLOCK_END      = 5,      // Конец блока кода.

    ML_CSTR                = 6,
                                     
    ML_CSTR_ARRAY_DEF      = 20,     // Определение массива.
    ML_CSTR_STRING         = 21,     // Определение строки-литерала. Пример: "это строка".

    ML_CSTR_FROM           = 31,     // "от". Начальное инициализатор цикла "для всех".
    ML_CSTR_TO             = 32,     // "до". Конечное значение цикла "для всех".
    ML_CSTR_STEP           = 33,     // "по". Шаг цикла "для всех".


};

enum StdFunctions
{
    ML_FUNC_SIN     = 100,           // Синус.
    ML_FUNC_COS     = 101,           // Косинус.
    ML_FUNC_TG      = 102,           // Тангенс.
    ML_FUNC_CTG     = 103,           // Котангенс.
    ML_FUNC_SH      = 104,           // Гиперболический синус.
    ML_FUNC_CH      = 105,           // Гиперболический косинус.
                                     
    ML_FUNC_ARCSIN  = 120,           // Арксинус.
    ML_FUNC_ARCCOS  = 121,           // Арккосинус.
    ML_FUNC_ARCTG   = 122,           // Арктангенс.
    ML_FUNC_ARCCTG  = 123,           // Арккотангенс.
                                     
    ML_FUNC_SQRT    = 0,             // Квадратный корень.
    ML_FUNC_CBRT    = 1,             // Кубический корень.
                                     
    ML_FUNC_LN      = 20,            // Натуральный логарифм.
    ML_FUNC_LG      = 21,            // Десятичный логарифм.
    ML_FUNC_LOG     = 22,            // Логарифм. "log_{a}^{b}"
};

enum ExpressionTypes
{
    ML_TYPE_UNKNOWN            = -1,
                               
    ML_TYPE_NUMBER             = 0,
    ML_TYPE_CONSTANT           = 1,
                               
    ML_TYPE_IDENTIFIER         = 2,
                               
    ML_TYPE_MATH_OPERATOR      = 3,
    ML_TYPE_FUNC_OPERATOR      = 4,
                               
    ML_TYPE_STD_FUNCTION       = 5,
                               
    ML_TYPE_SPECIAL_SYMBOL     = 6,

    ML_TYPE_LANG_CSTR          = 7,
};

/*
    switch (ExpressionType)
    {
        case ML_TYPE_CONSTANT:
            break;
           
        case ML_TYPE_FUNC_OPERATOR:
            break;

        case ML_TYPE_IDENTIFIER:
            break;
            
        case ML_TYPE_LANG_CSTR:
            break;

        case ML_TYPE_MATH_OPERATOR:
            break;

        case ML_TYPE_NUMBER:
            break;

        case ML_TYPE_SPECIAL_SYMBOL:
            break;

        case ML_TYPE_STD_FUNCTION:
            break;

        case ML_TYPE_UNKNOWN:
        default:
            break;
    }
*/

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

struct Expression
{
    union
    {
        double                 Number;

        char*                  Identifier;

        Constants              Constant;

        FunctionalOperators    FuncOperator;
        MathOperators          MathOperator;

        StdFunctions           StdFunction;

        SpecialSymbols         SpecSymbol;

        LanguageConstructions  Construction;

        int                    EnumValue;
    };
    ExpressionTypes Type;
};

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

enum GrammarType
{
    GRAMMAR_ALL = 1,// 0000 0001
    GRAMMAR_RU  = 3,// 0000 0011
    GRAMMAR_EN  = 5,// 0000 0101
};

struct GrammarToken
{
    /// Значение лексемы в перечислениях.
    int             TokenCode;
    /// Тип грамматики (Общая, русская, английская).
    GrammarType     GrammarType;
    /// Название лексемы
    const char*     TokenName;
    /// Имя лексемы, используемое в дереве
    const char*     TreeName;
    /// TokenNameSize == strlen(TokenName); Длина строки имени лексемы.
    size_t          TokenNameSize;
    /// TreeNameSize  == strlen(TreeName);  Длина строки имени лексемы.
    size_t          TreeNameSize;
};

struct GrammarTokensClass
{
    /// Количество лексем
    const size_t          TokensLength;
    /// Тип лексемы.
    const ExpressionTypes TokensType;
    /// Массив лексем
    GrammarToken* const   Tokens;
};

// Всего восемь типов выражений ExpressionTypes. Но массивов стандартных лексем 6, так как числа и идентификаторы не имеют массивов.
const size_t GrammarTokensTypesCount = 6;

extern GrammarTokensClass* GrammarTokens[];

extern GrammarTokensClass  GrammarStandartFunctionsClass;
extern GrammarTokensClass  GrammarFunctionalOperatorsClass;
extern GrammarTokensClass  GrammarLanguageConstructionsClass;
extern GrammarTokensClass  GrammarConstantsClass;
extern GrammarTokensClass  GrammarSpecSymbolsClass;
extern GrammarTokensClass  GrammarMathOperatorsClass;

extern char VariableSeparators[];
extern const size_t VariableSeparatorsLength;

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

void ConfigGrammarTokens();

void ExpressionDestructor(Expression* expr);

void PrintExpression(const Expression* expr, FILE* file);

GrammarToken* FindGrammarToken(const Expression* expr, const GrammarType grammarType);

GrammarToken* FindConstructionToken(const LanguageConstructions cstr, const GrammarType grammarType);

bool ExpressionsEqual(Expression* expr1, Expression* expr2);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\


#endif