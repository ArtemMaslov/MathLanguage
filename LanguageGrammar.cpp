#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits>


#include "Logs/Logs.h"
#include "LanguageGrammar.h"


///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                          Массив информации о лексемах
/// ! Необходимо один и тот же тип лексем хранить группой для более быстрого поиска.
/// ! Математические операторы (ML_TYPE_MATH_OPERATOR) должны стоять выше, чем специальные символы (ML_TYPE_SPECIAL_SYMBOL),
///   иначе неправильно будет читаться равенство (==).
/// ! В математических операторах (ML_TYPE_MATH_OPERATOR) составные символы сравнения (>=, <=) должны быть выше, чем символы сравнения (>, <).
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

GrammarToken GrammarTokens[] =
{
    
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                             Математические операторы.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
    
    // Бинарные математические операторы
    { ML_MATH_ADDITION,             ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "+"},
    { ML_MATH_SUBTRACTION,          ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "-"},
    { ML_MATH_MULTIPLICATION,       ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "*"},
    { ML_MATH_DIVISION,             ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "/"},
    { ML_MATH_POWER,                ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "^"},
    
    // Короткие бинарные математические операторы
    { ML_MATH_SHORT_ADDITION,       ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "+="},
    { ML_MATH_SHORT_SUBTRACTION,    ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "-="},
    { ML_MATH_SHORT_MULTIPLICATION, ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "*="},
    { ML_MATH_SHORT_DIVISION,       ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "/="},
    { ML_MATH_SHORT_POWER,          ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "^="},

    // Операторы инкремента и декремента.
    { ML_MATH_INCREMENT,            ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "++"},
    { ML_MATH_DECREMENT,            ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "--"},
    
    // Операторы равенства.
    { ML_MATH_EQUAL,                ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "=="},
    
    // Операторы не равенства.            
    { ML_MATH_NOT_EQUAL,            ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "!="},
    { ML_MATH_NOT_EQUAL,            ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "<>"},
    { ML_MATH_NOT_EQUAL,            ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "><"},
    
    // Операторы сравнения.
    // Больше или равно
    { ML_MATH_GREATER_EQUAL,        ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, ">="},
    { ML_MATH_GREATER_EQUAL,        ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "≥"},
    { ML_MATH_GREATER_EQUAL,        ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "≮"},
                    
    // Меньше или равно                
    { ML_MATH_LESS_EQUAL,           ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "<="},
    { ML_MATH_LESS_EQUAL,           ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "≤"},
    { ML_MATH_LESS_EQUAL,           ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "≯"},
                   
    // Больше                
    { ML_MATH_GREATER,              ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, ">"},
    { ML_MATH_GREATER,              ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "≰"},
          
    // Меньше                          
    { ML_MATH_LESS,                 ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "<"},
    { ML_MATH_LESS,                 ML_TYPE_MATH_OPERATOR,  GRAMMAR_ALL, "≱"},
       
    // Логические операторы.
    { ML_MATH_NOT,                  ML_TYPE_MATH_OPERATOR,   GRAMMAR_ALL, "!"},
    { ML_MATH_AND,                  ML_TYPE_MATH_OPERATOR,   GRAMMAR_ALL, "&&"},
    { ML_MATH_OR,                   ML_TYPE_MATH_OPERATOR,   GRAMMAR_ALL, "||"},
                                    
    { ML_MATH_NOT,                  ML_TYPE_MATH_OPERATOR,   GRAMMAR_RU,  "не"},
    { ML_MATH_OR,                   ML_TYPE_MATH_OPERATOR,   GRAMMAR_RU,  "или"},
    { ML_MATH_AND,                  ML_TYPE_MATH_OPERATOR,   GRAMMAR_RU,  "и"},
                                    
    { ML_MATH_NOT,                  ML_TYPE_MATH_OPERATOR,   GRAMMAR_EN,  "not"},
    { ML_MATH_AND,                  ML_TYPE_MATH_OPERATOR,   GRAMMAR_EN,  "and"},
    { ML_MATH_OR,                   ML_TYPE_MATH_OPERATOR,   GRAMMAR_EN,  "or"},

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                               Специальные символы.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

    { ML_SPEC_COLON,            ML_TYPE_SPECIAL_SYMBOL, GRAMMAR_ALL, ":"},
    { ML_SPEC_SEMICOLON,        ML_TYPE_SPECIAL_SYMBOL, GRAMMAR_ALL, ";"},

    { ML_SPEC_POINT,            ML_TYPE_SPECIAL_SYMBOL, GRAMMAR_ALL, "."},
    { ML_SPEC_COMMA,            ML_TYPE_SPECIAL_SYMBOL, GRAMMAR_ALL, ","},

    { ML_SPEC_EQUAL,            ML_TYPE_SPECIAL_SYMBOL, GRAMMAR_ALL, "="},

    { ML_SPEC_SINGLE_QUOTES,    ML_TYPE_SPECIAL_SYMBOL, GRAMMAR_ALL, "\'"},
    { ML_SPEC_DOUBLE_QUOTES,    ML_TYPE_SPECIAL_SYMBOL, GRAMMAR_ALL, "\""},
                                
    { ML_SPEC_L_CURLY_BRACKET,  ML_TYPE_SPECIAL_SYMBOL, GRAMMAR_ALL, "{"},
    { ML_SPEC_R_CURLY_BRACKET,  ML_TYPE_SPECIAL_SYMBOL, GRAMMAR_ALL, "}"},

    { ML_SPEC_L_ROUND_BRACKET,  ML_TYPE_SPECIAL_SYMBOL, GRAMMAR_ALL, "("},
    { ML_SPEC_R_ROUND_BRACKET,  ML_TYPE_SPECIAL_SYMBOL, GRAMMAR_ALL, ")"},

    { ML_SPEC_L_SQUARE_BRACKET, ML_TYPE_SPECIAL_SYMBOL, GRAMMAR_ALL, "["},
    { ML_SPEC_R_SQUARE_BRACKET, ML_TYPE_SPECIAL_SYMBOL, GRAMMAR_ALL, "]"},

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                    Константы.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

    { ML_CONST_EXP,             ML_TYPE_CONSTANT,       GRAMMAR_EN,  "exp"},
    { ML_CONST_EXP,             ML_TYPE_CONSTANT,       GRAMMAR_RU,  "эксп"},
                                
    { ML_CONST_PI,              ML_TYPE_CONSTANT,       GRAMMAR_ALL, "π"},
    { ML_CONST_PI,              ML_TYPE_CONSTANT,       GRAMMAR_EN,  "pi"},
    { ML_CONST_PI,              ML_TYPE_CONSTANT,       GRAMMAR_RU,  "пи"},
    
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                               Языковые конструкции.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                     Русский.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

    { ML_CSTR_PROGRAM_START,    ML_TYPE_LANG_CSTR,      GRAMMAR_RU, "начало программы"},

    { ML_CSTR_FROM,             ML_TYPE_LANG_CSTR,      GRAMMAR_RU, "от"},
    { ML_CSTR_TO,               ML_TYPE_LANG_CSTR,      GRAMMAR_RU, "до"},
    { ML_CSTR_STEP,             ML_TYPE_LANG_CSTR,      GRAMMAR_RU, "по"},
    { ML_CSTR_STEP,             ML_TYPE_LANG_CSTR,      GRAMMAR_RU, "шаг"},
    
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                     English.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
    
    { ML_CSTR_PROGRAM_START,    ML_TYPE_LANG_CSTR,      GRAMMAR_EN, "programm start"},

    { ML_CSTR_FROM,             ML_TYPE_LANG_CSTR,      GRAMMAR_EN, "from"},
    { ML_CSTR_TO,               ML_TYPE_LANG_CSTR,      GRAMMAR_EN, "to"},
    { ML_CSTR_STEP,             ML_TYPE_LANG_CSTR,      GRAMMAR_EN, "step"},

    // ML_CSTR_FUNCTION_DEF
    // ML_CSTR_FUNCTION_CALL
    
    // ML_CSTR_BLOCK_START
    // ML_CSTR_BLOCK_END

    // ML_CSTR_GLOBAL_VAR_DEF
    // ML_CSTR_ARRAY_DEF

    // ML_CSTR_STRING
        
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                             Функциональные операторы.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                     Русский.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

    { ML_OPER_IF,               ML_TYPE_FUNC_OPERATOR,  GRAMMAR_RU, "если"},
    { ML_OPER_ELSE,             ML_TYPE_FUNC_OPERATOR,  GRAMMAR_RU, "иначе"},
    // ML_OPER_ELSEIF           

    { ML_OPER_WHILE,            ML_TYPE_FUNC_OPERATOR,  GRAMMAR_RU, "пока"},
    { ML_OPER_STD_FOR,          ML_TYPE_FUNC_OPERATOR,  GRAMMAR_RU, "для всех"},
    { ML_OPER_FOR_ALL,          ML_TYPE_FUNC_OPERATOR,  GRAMMAR_RU, "для всех"},
    
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                     English.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

    { ML_OPER_IF,               ML_TYPE_FUNC_OPERATOR,  GRAMMAR_EN, "if"},
    { ML_OPER_ELSE,             ML_TYPE_FUNC_OPERATOR,  GRAMMAR_EN, "else"},
    // ML_OPER_ELSEIF           
                                
    { ML_OPER_WHILE,            ML_TYPE_FUNC_OPERATOR,  GRAMMAR_EN, "while"},
    { ML_OPER_STD_FOR,          ML_TYPE_FUNC_OPERATOR,  GRAMMAR_EN, "for"},
    { ML_OPER_FOR_ALL,          ML_TYPE_FUNC_OPERATOR,  GRAMMAR_EN, "for all "},

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                Встроенные функции.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                            Универсальная грамматика.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

    {ML_FUNC_SQRT,              ML_TYPE_STD_FUNCTION,       GRAMMAR_ALL, "√"},
    {ML_FUNC_CBRT,              ML_TYPE_STD_FUNCTION,       GRAMMAR_ALL, "∛"},
    
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                     Русский.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

    {ML_FUNC_SIN,               ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "син"},
    {ML_FUNC_SIN,               ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "синус"},
    {ML_FUNC_COS,               ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "кос"},
    {ML_FUNC_COS,               ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "косинус"},
    {ML_FUNC_TG,                ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "тг"},
    {ML_FUNC_TG,                ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "тангенс"},
    {ML_FUNC_CTG,               ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "ктг"},
    {ML_FUNC_CTG,               ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "котангенс"},
    {ML_FUNC_SH,                ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "сш"},
    {ML_FUNC_SH,                ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "синус_гиперболический"},
    {ML_FUNC_CH,                ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "кш"},
    {ML_FUNC_CH,                ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "косинус_гиперболический"},
                                            
    {ML_FUNC_ARCSIN,            ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "арксин"},
    {ML_FUNC_ARCSIN,            ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "арксинус"},
    {ML_FUNC_ARCCOS,            ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "арккос"},
    {ML_FUNC_ARCCOS,            ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "арккосинус"},
    {ML_FUNC_ARCTG,             ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "арктг"},
    {ML_FUNC_ARCTG,             ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "арктангенс"},
    {ML_FUNC_ARCCTG,            ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "аркктг"},
    {ML_FUNC_ARCCTG,            ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "арккотангенс"},
                                            
    {ML_FUNC_SQRT,              ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "корень"},
    {ML_FUNC_CBRT,              ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "куб_корень"},
                                            
    {ML_FUNC_LN,                ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "лн"},
    {ML_FUNC_LN,                ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "натуральный_логарифм"},
    {ML_FUNC_LG,                ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "лг"},
    {ML_FUNC_LG,                ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "десятичный_логарифм"},
    {ML_FUNC_LOG,               ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "лог"},
    {ML_FUNC_LOG,               ML_TYPE_STD_FUNCTION,       GRAMMAR_RU, "логарифм"},

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                     English.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

    {ML_FUNC_SIN,               ML_TYPE_STD_FUNCTION,       GRAMMAR_EN, "sin"},
    {ML_FUNC_COS,               ML_TYPE_STD_FUNCTION,       GRAMMAR_EN, "cos"},
    {ML_FUNC_TG,                ML_TYPE_STD_FUNCTION,       GRAMMAR_EN, "tg"},
    {ML_FUNC_CTG,               ML_TYPE_STD_FUNCTION,       GRAMMAR_EN, "ctg"},
    {ML_FUNC_SH,                ML_TYPE_STD_FUNCTION,       GRAMMAR_EN, "sh"},
    {ML_FUNC_CH,                ML_TYPE_STD_FUNCTION,       GRAMMAR_EN, "ch"},
                                            
    {ML_FUNC_ARCSIN,            ML_TYPE_STD_FUNCTION,       GRAMMAR_EN, "arcsin"},
    {ML_FUNC_ARCCOS,            ML_TYPE_STD_FUNCTION,       GRAMMAR_EN, "arccos"},
    {ML_FUNC_ARCTG,             ML_TYPE_STD_FUNCTION,       GRAMMAR_EN, "arctg"},
    {ML_FUNC_ARCCTG,            ML_TYPE_STD_FUNCTION,       GRAMMAR_EN, "arcctg"},
                                            
    {ML_FUNC_SQRT,              ML_TYPE_STD_FUNCTION,       GRAMMAR_EN, "sqrt"},
    {ML_FUNC_CBRT,              ML_TYPE_STD_FUNCTION,       GRAMMAR_EN, "cbrt"},
                                            
    {ML_FUNC_LN,                ML_TYPE_STD_FUNCTION,       GRAMMAR_EN, "ln"},
    {ML_FUNC_LG,                ML_TYPE_STD_FUNCTION,       GRAMMAR_EN, "lg"},
    {ML_FUNC_LOG,               ML_TYPE_STD_FUNCTION,       GRAMMAR_EN, "log"},
    
};

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

char VariableSeparators[] = 
{
    '!',
    '@',
    '#',
    '$',
    '%',
    '^',
    '&',
    '*',
    '(',
    ')',
    '+',
    '|',
    '{',
    '}',
    ':',
    '"',
    '<',
    '>',
    '?',
    '-',
    '=',
    '\\',
    '[',
    ']',
    ';',
    '\'',
    ',',
    '.',
    '/',
    '`',
    '~',
    ' ',
    '\r',
    '\n',
    '\t',
    '\v',
};

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

$GrammarInfo_S_ GrammarInfo = {};

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

void ConfigGrammarTokens()
{
    GrammarInfo.GrammarTokensLength      = sizeof(GrammarTokens)      / sizeof(GrammarTokens[0]);
    GrammarInfo.VariableSeparatorsLength = sizeof(VariableSeparators) / sizeof(VariableSeparators[0]);

    for (size_t st = 0; st < GrammarInfo.GrammarTokensLength; st++)
    {
        GrammarTokens[st].TokenSize = strlen(GrammarTokens[st].TokenName);

        switch (GrammarTokens[st].TokenType)
        {
            case ML_TYPE_CONSTANT:
                if (GrammarInfo.ConstantTokensLength == 0)
                    GrammarInfo.ConstantTokensIndex = st;

                GrammarInfo.ConstantTokensLength++;
                break;

            case ML_TYPE_FUNC_OPERATOR:
                if (GrammarInfo.FuncOperatorsLength == 0)
                    GrammarInfo.FuncOperatorsIndex = st;

                GrammarInfo.FuncOperatorsLength++;
                break;

            case ML_TYPE_LANG_CSTR:
                if (GrammarInfo.LangConstructionsLength == 0)
                    GrammarInfo.LangConstructionsIndex = st;

                GrammarInfo.LangConstructionsLength++;
                break;

            case ML_TYPE_MATH_OPERATOR:
                if (GrammarInfo.MathOperatorsLength == 0)
                    GrammarInfo.MathOperatorsIndex = st;

                GrammarInfo.MathOperatorsLength++;
                break;

            case ML_TYPE_SPECIAL_SYMBOL:
                if (GrammarInfo.SpecSymbolsLength == 0)
                    GrammarInfo.SpecSymbolsIndex = st;

                GrammarInfo.SpecSymbolsLength++;
                break;

            case ML_TYPE_STD_FUNCTION:
                if (GrammarInfo.StdFunctionsLength == 0)
                    GrammarInfo.StdFunctionsIndex = st;

                GrammarInfo.StdFunctionsLength++;
                break;
                
            case ML_TYPE_IDENTIFIER:
            case ML_TYPE_NUMBER:
            case ML_TYPE_UNKNOWN:
            default:
                assert(!"Неизвестный тип лексемы");
                break;
        }
    }
}

void ExpressionDestructor(Expression* expr)
{
    assert(expr);

    if (expr->Type == ML_TYPE_IDENTIFIER)
    {
        free(expr->Identifier);
    }
}

void PrintExpression(const Expression* expr, FILE* file)
{
    assert(expr);
    assert(file);

    int tokenIndex = 0;

    switch (expr->Type)
    {
        case ML_TYPE_NUMBER:
            fprintf(file, "%.6lg", expr->Number);
            break;
            
        case ML_TYPE_IDENTIFIER:
            fputs(expr->Identifier, file);
            break;

        case ML_TYPE_CONSTANT:
        case ML_TYPE_MATH_OPERATOR:
        case ML_TYPE_FUNC_OPERATOR:
        case ML_TYPE_LANG_CSTR:
        case ML_TYPE_SPECIAL_SYMBOL:
        case ML_TYPE_STD_FUNCTION:

            tokenIndex = FindGrammarToken(expr, GRAMMAR_ALL);
            
            assert(tokenIndex >= 0);

            fputs(GrammarTokens[tokenIndex].TokenName, file);
            break;

        case ML_TYPE_UNKNOWN:
        default:
            LogLine("Неизвестный тип математического выражения.", LOG_LVL_ERROR, LOG_SIG_PARSER, true);
            break;
    }
}

int FindGrammarToken(const Expression* expr, const GrammarType grammarType)
{
    assert(expr);

    size_t length   = 0;
    size_t index    = 0;
    int    exprCode = 0;

    switch (expr->Type)
    {
        case ML_TYPE_CONSTANT:
            index    = GrammarInfo.ConstantTokensIndex;
            length   = GrammarInfo.ConstantTokensLength;
            exprCode = expr->Constant;
            break;

        case ML_TYPE_FUNC_OPERATOR:
            index    = GrammarInfo.FuncOperatorsIndex;
            length   = GrammarInfo.FuncOperatorsLength;
            exprCode = expr->FuncOperator;
            break;

        case ML_TYPE_LANG_CSTR:
            index    = GrammarInfo.LangConstructionsIndex;
            length   = GrammarInfo.LangConstructionsLength;
            exprCode = expr->Construction;
            break;

        case ML_TYPE_MATH_OPERATOR:
            index    = GrammarInfo.MathOperatorsIndex;
            length   = GrammarInfo.MathOperatorsLength;
            exprCode = expr->MathOperator;
            break;

        case ML_TYPE_SPECIAL_SYMBOL:
            index    = GrammarInfo.SpecSymbolsIndex;
            length   = GrammarInfo.SpecSymbolsLength;
            exprCode = expr->SpecSymbol;
            break;

        case ML_TYPE_STD_FUNCTION:
            index    = GrammarInfo.StdFunctionsIndex;
            length   = GrammarInfo.StdFunctionsLength; 
            exprCode = expr->StdFunction;
            break;

        case ML_TYPE_IDENTIFIER:
        case ML_TYPE_NUMBER:
        case ML_TYPE_UNKNOWN:
        default:
            assert(!"Неизвестный тип лексемы");
            break;
    }

    for (size_t st = 0; st < length; st++, index++)
    {
        assert(GrammarTokens[index].TokenType == expr->Type);

        if (GrammarTokens[index].TokenCode == exprCode &&
           (GrammarTokens[index].GrammarType & grammarType) > 0)
        {
            return (int)(index);
        }
    }

    assert(!"Лексема из такой грамматики не найдена");

    return -1;
}

bool ExpressionsEqual(Expression* expr1, Expression* expr2)
{
    assert(expr1);
    assert(expr2);

    if (expr1->Type == ML_TYPE_NUMBER && expr2->Type == ML_TYPE_NUMBER)
    {
        if (fabs(expr1->Number - expr2->Number) < DBL_EPSILON)
            return true;
        return false;
    }

    return memcmp(expr1, expr2, sizeof(Expression)) == 0;
}