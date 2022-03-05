#ifndef PARSER_DEFINES_H__
#define PARSER_DEFINES_H__

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

#define TOKEN_TYPE(type) (parser->Tokens[parser->TokenIndex].Type == (type))
#define TOKEN_TYPE1(type, index) (parser->Tokens[parser->TokenIndex + index].Type == (type))

#define TOKEN             parser->Tokens[parser->TokenIndex]
#define TOKEN1(index)     parser->Tokens[parser->TokenIndex + index]

#define TOKEN_SPEC(value, index) (parser->Tokens[parser->TokenIndex].SpecSymbol == (value))
#define TOKEN_SPEC1(value, index) (parser->Tokens[parser->TokenIndex + index].SpecSymbol == (value))

#define TOKEN_CSTR        parser->Tokens[parser->TokenIndex].Construction

#define ASSERT_NODE(node)                                                           \
    if (!(node))                                                                    \
    {                                                                               \
        /* #log    */                                                               \
        /* #err    */                                                               \
        return nullptr;                                                             \
    }

#define CSTR_NODE(node)                                                             \
    node->Value.Type = ML_TYPE_LANG_CSTR;                                           \
    node->Value.Construction = ML_CSTR;

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

#endif PARSER_DEFINES_H__