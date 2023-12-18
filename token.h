#pragma once

#include <unordered_set>

enum TokenType
{
    END,
    WHITESPACE,
    KEYWORD,
    IDENTIFIER,
    INT_LITERAL,
    FLOAT_LITERAL,
    CHAR_LITERAL,
    STRING_LITERAL,
    OPERATOR,
    PUNCTUATOR,
    COMMENT,
    BAD,
};

struct Token
{
    TokenType type;
    size_t index;
};

const char* type_strings[] =
{
    "",
    "whitespace",
    "keyword",
    "identifier",
    "int literal",
    "float literal",
    "char literal",
    "string literal",
    "operator",
    "punctuator",
    "comment",
    "bad"
};

std::unordered_set<std::string> keywords =
{
    "auto", "break", "cases", "char",
    "const", "continue", "default", "do",
    "double", "else", "enum", "extern",
    "float", "for", "goto", "if",
    "int", "long", "register","return",
    "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"
};