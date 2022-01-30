#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits>


#include "Logs/Logs.h"
#include "LanguageGrammar.h"


///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                          Массивы информации о лексемах
/// ! Математические операторы (ML_TYPE_MATH_OPERATOR) должны обрабатываться раньше, чем специальные символы (ML_TYPE_SPECIAL_SYMBOL),
///   иначе неправильно будет читаться равенство (==).
/// ! В математических операторах (ML_TYPE_MATH_OPERATOR) составные символы сравнения (>=, <=) должны быть выше, чем символы сравнения (>, <).
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                             Математические операторы.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

GrammarToken GrammarMathOperatorsTokens[] =
{
    // Бинарные математические операторы
    { ML_MATH_ADDITION,             GRAMMAR_ALL, "+"},
    { ML_MATH_SUBTRACTION,          GRAMMAR_ALL, "-"},
    { ML_MATH_MULTIPLICATION,       GRAMMAR_ALL, "*"},
    { ML_MATH_DIVISION,             GRAMMAR_ALL, "/"},
    { ML_MATH_POWER,                GRAMMAR_ALL, "^"},
    
    // Короткие бинарные математические операторы
    { ML_MATH_SHORT_ADDITION,       GRAMMAR_ALL, "+="},
    { ML_MATH_SHORT_SUBTRACTION,    GRAMMAR_ALL, "-="},
    { ML_MATH_SHORT_MULTIPLICATION, GRAMMAR_ALL, "*="},
    { ML_MATH_SHORT_DIVISION,       GRAMMAR_ALL, "/="},
    { ML_MATH_SHORT_POWER,          GRAMMAR_ALL, "^="},

    // Операторы инкремента и декремента.
    { ML_MATH_INCREMENT,            GRAMMAR_ALL, "++"},
    { ML_MATH_DECREMENT,            GRAMMAR_ALL, "--"},
    
    // Операторы равенства.
    { ML_MATH_EQUAL,                GRAMMAR_ALL, "=="},
    
    // Операторы не равенства.            
    { ML_MATH_NOT_EQUAL,            GRAMMAR_ALL, "!="},
    { ML_MATH_NOT_EQUAL,            GRAMMAR_ALL, "<>"},
    { ML_MATH_NOT_EQUAL,            GRAMMAR_ALL, "><"},
    
    // Операторы сравнения.
    // Больше или равно
    { ML_MATH_GREATER_EQUAL,        GRAMMAR_ALL, ">="},
    { ML_MATH_GREATER_EQUAL,        GRAMMAR_ALL, "≥"},
    { ML_MATH_GREATER_EQUAL,        GRAMMAR_ALL, "≮"},
                    
    // Меньше или равно             
    { ML_MATH_LESS_EQUAL,           GRAMMAR_ALL, "<="},
    { ML_MATH_LESS_EQUAL,           GRAMMAR_ALL, "≤"},
    { ML_MATH_LESS_EQUAL,           GRAMMAR_ALL, "≯"},
                   
    // Больше                
    { ML_MATH_GREATER,              GRAMMAR_ALL, ">"},
    { ML_MATH_GREATER,              GRAMMAR_ALL, "≰"},
          
    // Меньше                       
    { ML_MATH_LESS,                 GRAMMAR_ALL, "<"},
    { ML_MATH_LESS,                 GRAMMAR_ALL, "≱"},
       
    // Логические операторы.
    { ML_MATH_NOT,                  GRAMMAR_ALL, "!"},
    { ML_MATH_AND,                  GRAMMAR_ALL, "&&"},
    { ML_MATH_OR,                   GRAMMAR_ALL, "||"},
                                   
    { ML_MATH_NOT,                  GRAMMAR_RU,  "не"},
    { ML_MATH_OR,                   GRAMMAR_RU,  "или"},
    { ML_MATH_AND,                  GRAMMAR_RU,  "и"},
                                   
    { ML_MATH_NOT,                  GRAMMAR_EN,  "not"},
    { ML_MATH_AND,                  GRAMMAR_EN,  "and"},
    { ML_MATH_OR,                   GRAMMAR_EN,  "or"},

};

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                               Специальные символы.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

GrammarToken GrammarSpecSymbolsTokens[] =
{
    { ML_SPEC_COLON,            GRAMMAR_ALL, ":"},
    { ML_SPEC_SEMICOLON,        GRAMMAR_ALL, ";"},

    { ML_SPEC_POINT,            GRAMMAR_ALL, "."},
    { ML_SPEC_COMMA,            GRAMMAR_ALL, ","},

    { ML_SPEC_EQUAL,            GRAMMAR_ALL, "="},

    { ML_SPEC_SINGLE_QUOTES,    GRAMMAR_ALL, "\'"},
    { ML_SPEC_DOUBLE_QUOTES,    GRAMMAR_ALL, "\""},
                               
    { ML_SPEC_L_CURLY_BRACKET,  GRAMMAR_ALL, "{"},
    { ML_SPEC_R_CURLY_BRACKET,  GRAMMAR_ALL, "}"},

    { ML_SPEC_L_ROUND_BRACKET,  GRAMMAR_ALL, "("},
    { ML_SPEC_R_ROUND_BRACKET,  GRAMMAR_ALL, ")"},

    { ML_SPEC_L_SQUARE_BRACKET, GRAMMAR_ALL, "["},
    { ML_SPEC_R_SQUARE_BRACKET, GRAMMAR_ALL, "]"},
};

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                    Константы.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

GrammarToken GrammarConstantsTokens[] =
{
    { ML_CONST_EXP,             GRAMMAR_EN,  "exp"},
    { ML_CONST_EXP,             GRAMMAR_RU,  "эксп"},
                          
    { ML_CONST_PI,              GRAMMAR_ALL, "π"},
    { ML_CONST_PI,              GRAMMAR_EN,  "pi"},
    { ML_CONST_PI,              GRAMMAR_RU,  "пи"},
};
    
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                               Языковые конструкции.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

GrammarToken GrammarLanguageConstructionsTokens[] =
{

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                     Русский.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

    { ML_CSTR_PROGRAM_START,    GRAMMAR_RU, "начало программы"},

    { ML_CSTR_FROM,             GRAMMAR_RU, "от"},
    { ML_CSTR_TO,               GRAMMAR_RU, "до"},
    { ML_CSTR_STEP,             GRAMMAR_RU, "по"},
    { ML_CSTR_STEP,             GRAMMAR_RU, "шаг"},
    
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                     English.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
    
    { ML_CSTR_PROGRAM_START,    GRAMMAR_EN, "programm start"},

    { ML_CSTR_FROM,             GRAMMAR_EN, "from"},
    { ML_CSTR_TO,               GRAMMAR_EN, "to"},
    { ML_CSTR_STEP,             GRAMMAR_EN, "step"},

    // ML_CSTR_FUNCTION_DEF
    // ML_CSTR_FUNCTION_CALL
    
    // ML_CSTR_BLOCK_START
    // ML_CSTR_BLOCK_END

    // ML_CSTR_GLOBAL_VAR_DEF
    // ML_CSTR_ARRAY_DEF

    // ML_CSTR_STRING

};
        
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                             Функциональные операторы.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

GrammarToken GrammarFunctionalOperatorsTokens[] =
{

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                     Русский.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

    { ML_OPER_IF,               GRAMMAR_RU, "если"},
    { ML_OPER_ELSE,             GRAMMAR_RU, "иначе"},
    // ML_OPER_ELSEIF           
                                
    { ML_OPER_WHILE,            GRAMMAR_RU, "пока"},
    { ML_OPER_STD_FOR,          GRAMMAR_RU, "для всех"},
    { ML_OPER_FOR_ALL,          GRAMMAR_RU, "для всех"},
    
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                     English.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

    { ML_OPER_IF,               GRAMMAR_EN, "if"},
    { ML_OPER_ELSE,             GRAMMAR_EN, "else"},
    // ML_OPER_ELSEIF          
                               
    { ML_OPER_WHILE,            GRAMMAR_EN, "while"},
    { ML_OPER_STD_FOR,          GRAMMAR_EN, "for"},
    { ML_OPER_FOR_ALL,          GRAMMAR_EN, "for all "},
};

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                Встроенные функции.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

GrammarToken GrammarStandartFunctionsTokens[] = 
{

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                            Универсальная грамматика.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

    {ML_FUNC_SQRT,              GRAMMAR_ALL, "√"},
    {ML_FUNC_CBRT,              GRAMMAR_ALL, "∛"},
    
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                     Русский.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

    {ML_FUNC_SIN,               GRAMMAR_RU, "син"},
    {ML_FUNC_SIN,               GRAMMAR_RU, "синус"},
    {ML_FUNC_COS,               GRAMMAR_RU, "кос"},
    {ML_FUNC_COS,               GRAMMAR_RU, "косинус"},
    {ML_FUNC_TG,                GRAMMAR_RU, "тг"},
    {ML_FUNC_TG,                GRAMMAR_RU, "тангенс"},
    {ML_FUNC_CTG,               GRAMMAR_RU, "ктг"},
    {ML_FUNC_CTG,               GRAMMAR_RU, "котангенс"},
    {ML_FUNC_SH,                GRAMMAR_RU, "сш"},
    {ML_FUNC_SH,                GRAMMAR_RU, "синус_гиперболический"},
    {ML_FUNC_CH,                GRAMMAR_RU, "кш"},
    {ML_FUNC_CH,                GRAMMAR_RU, "косинус_гиперболический"},
                               
    {ML_FUNC_ARCSIN,            GRAMMAR_RU, "арксин"},
    {ML_FUNC_ARCSIN,            GRAMMAR_RU, "арксинус"},
    {ML_FUNC_ARCCOS,            GRAMMAR_RU, "арккос"},
    {ML_FUNC_ARCCOS,            GRAMMAR_RU, "арккосинус"},
    {ML_FUNC_ARCTG,             GRAMMAR_RU, "арктг"},
    {ML_FUNC_ARCTG,             GRAMMAR_RU, "арктангенс"},
    {ML_FUNC_ARCCTG,            GRAMMAR_RU, "аркктг"},
    {ML_FUNC_ARCCTG,            GRAMMAR_RU, "арккотангенс"},
                               
    {ML_FUNC_SQRT,              GRAMMAR_RU, "корень"},
    {ML_FUNC_CBRT,              GRAMMAR_RU, "куб_корень"},
                               
    {ML_FUNC_LN,                GRAMMAR_RU, "лн"},
    {ML_FUNC_LN,                GRAMMAR_RU, "натуральный_логарифм"},
    {ML_FUNC_LG,                GRAMMAR_RU, "лг"},
    {ML_FUNC_LG,                GRAMMAR_RU, "десятичный_логарифм"},
    {ML_FUNC_LOG,               GRAMMAR_RU, "лог"},
    {ML_FUNC_LOG,               GRAMMAR_RU, "логарифм"},

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                                     English.
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

    {ML_FUNC_SIN,               GRAMMAR_EN, "sin"},
    {ML_FUNC_COS,               GRAMMAR_EN, "cos"},
    {ML_FUNC_TG,                GRAMMAR_EN, "tg"},
    {ML_FUNC_CTG,               GRAMMAR_EN, "ctg"},
    {ML_FUNC_SH,                GRAMMAR_EN, "sh"},
    {ML_FUNC_CH,                GRAMMAR_EN, "ch"},
                                
    {ML_FUNC_ARCSIN,            GRAMMAR_EN, "arcsin"},
    {ML_FUNC_ARCCOS,            GRAMMAR_EN, "arccos"},
    {ML_FUNC_ARCTG,             GRAMMAR_EN, "arctg"},
    {ML_FUNC_ARCCTG,            GRAMMAR_EN, "arcctg"},
                                
    {ML_FUNC_SQRT,              GRAMMAR_EN, "sqrt"},
    {ML_FUNC_CBRT,              GRAMMAR_EN, "cbrt"},
                                
    {ML_FUNC_LN,                GRAMMAR_EN, "ln"},
    {ML_FUNC_LG,                GRAMMAR_EN, "lg"},
    {ML_FUNC_LOG,               GRAMMAR_EN, "log"},
};

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

GrammarTokensClass GrammarMathOperatorsClass =
{
    // Количество элементов массива
    sizeof(GrammarMathOperatorsTokens) / sizeof(GrammarMathOperatorsTokens[0]), 
    // Тип лексемы.
    ML_TYPE_MATH_OPERATOR,
    GrammarMathOperatorsTokens
};

GrammarTokensClass GrammarSpecSymbolsClass =
{
    // Количество элементов массива
    sizeof(GrammarSpecSymbolsTokens) / sizeof(GrammarSpecSymbolsTokens[0]), 
    // Тип лексемы.
    ML_TYPE_SPECIAL_SYMBOL,
    GrammarSpecSymbolsTokens
};

GrammarTokensClass GrammarConstantsClass =
{
    // Количество элементов массива
    sizeof(GrammarConstantsTokens) / sizeof(GrammarConstantsTokens[0]), 
    // Тип лексемы.
    ML_TYPE_CONSTANT,
    GrammarConstantsTokens
};

GrammarTokensClass GrammarLanguageConstructionsClass =
{
    // Количество элементов массива
    sizeof(GrammarLanguageConstructionsTokens) / sizeof(GrammarLanguageConstructionsTokens[0]), 
    // Тип лексемы.
    ML_TYPE_LANG_CSTR,
    GrammarLanguageConstructionsTokens
};

GrammarTokensClass GrammarFunctionalOperatorsClass =
{
    // Количество элементов массива
    sizeof(GrammarFunctionalOperatorsTokens) / sizeof(GrammarFunctionalOperatorsTokens[0]), 
    // Тип лексемы.
    ML_TYPE_FUNC_OPERATOR,
    GrammarFunctionalOperatorsTokens
};

GrammarTokensClass GrammarStandartFunctionsClass =
{
    // Количество элементов массива
    sizeof(GrammarStandartFunctionsTokens) / sizeof(GrammarStandartFunctionsTokens[0]), 
    // Тип лексемы.
    ML_TYPE_STD_FUNCTION,
    GrammarStandartFunctionsTokens
};

GrammarTokensClass* GrammarTokens[GrammarTokensTypesCount] =
{
    &GrammarMathOperatorsClass,
    &GrammarSpecSymbolsClass,
    &GrammarConstantsClass,
    &GrammarLanguageConstructionsClass,
    &GrammarFunctionalOperatorsClass,
    &GrammarStandartFunctionsClass
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

const size_t VariableSeparatorsLength = sizeof(VariableSeparators) / sizeof(VariableSeparators[0]);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

void ConfigGrammarTokens()
{
    for (size_t grammarTypeIndex = 0; grammarTypeIndex < GrammarTokensTypesCount; grammarTypeIndex++)
    {
        GrammarToken* tokens      = GrammarTokens[grammarTypeIndex]->Tokens;
        size_t        tokensCount = GrammarTokens[grammarTypeIndex]->TokensLength;

        for (size_t tokenIndex = 0; tokenIndex < tokensCount; tokenIndex++)
        {
            tokens[tokenIndex].TokenSize = strlen(tokens[tokenIndex].TokenName);
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

    GrammarToken* token = nullptr;

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

            token = FindGrammarToken(expr, GRAMMAR_ALL);

            if (token)
                fputs(token->TokenName, file);
            break;

        case ML_TYPE_UNKNOWN:
        default:
            LogLine("Неизвестный тип математического выражения.", LOG_LVL_ERROR, LOG_SIG_PARSER, true);
            break;
    }
}

GrammarToken* FindGrammarToken(const Expression* expr, const GrammarType grammarType)
{
    assert(expr);

    GrammarToken* tokens       = nullptr; 
    size_t        tokensLength = 0;

    for (size_t grammarTypeIndex = 0; grammarTypeIndex < GrammarTokensTypesCount; grammarTypeIndex++)
    {
        if (GrammarTokens[grammarTypeIndex]->TokensType == expr->Type)
        {
            tokens       = GrammarTokens[grammarTypeIndex]->Tokens;
            tokensLength = GrammarTokens[grammarTypeIndex]->TokensLength;

            for (size_t tokenIndex = 0; tokenIndex < tokensLength; tokenIndex++)
            {
                if (tokens[tokenIndex].TokenCode == expr->EnumValue &&
                   (tokens[tokenIndex].GrammarType & grammarType) > 0)
                {
                    return tokens + tokenIndex;
                }
            }

            break;
        }
    }

    assert(!"Лексема из такой грамматики не найдена");

    return nullptr;
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