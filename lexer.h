#pragma once

#include <cstdio>

#include "dfa.h"
#include "string_table.h"

struct Lexer
{
    FILE* file;

    Lexer(FILE* _file) : file(_file) {}

    Token create_token(const std::string& str)
    {
        TokenType type = get_token_type();

        if (type == IDENTIFIER && keywords.find(str) != keywords.end())
            type = KEYWORD;

        return (Token) { type, get_index(str) };
    }

    Token __get_next()
    {
        reset_state();

        std::string str = "";

        while (true)
        {
            int c = fgetc(file);

            if (c == EOF)
            {
                if (str.empty())
                    return (Token) { END, 0 };

                return create_token(str);
            }

            bool p = process_char(c);

            if (!process_char(c))
            {
                if (str.empty())
                    str += c;
                else
                    fseek(file, -1, SEEK_CUR);

                return create_token(str);
            }

            str += c;
        }
    }

    Token get_next()
    {
        Token tok = __get_next();

        while (tok.type == WHITESPACE || tok.type == COMMENT)
            tok = __get_next();

        return tok;
    }
};