#pragma once

#include <cctype>

#include "token.h"

enum State
{
    INIT,
    IN_SPACE,
    IN_IDENT,

    IN_COMMENT,
    IN_MCOMMENT,
    MAYBE_COM_END,
    COM_END,

    IN_ZERO,
    IN_DEC,
    IN_OCTAL,
    MAYBE_HEX,
    IN_HEX,
    MAYBE_BIN,
    IN_BIN,

    AFTER_DP,
    IN_EXP,
    IN_EXP_SIGN,
    EXP_END,

    IN_STRING,
    IN_ESCAPE,
    STRING_END,

    IN_CHAR,
    IN_CHAR_ESCAPE,
    CHAR_END,

    IN_OPERATOR,
    IN_PUNCTUATOR,

    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MODULO,
    AND,
    OR,
    XOR,
    LEFT_SHIFT,
    RIGHT_SHIFT,
    LESS,
    GREATER,
    EQUAL,
    NOT,
    DOT,
    DOTDOT,
};

State current;

void reset_state()
{
    current = INIT;
}

bool process_char(char c)
{
    switch (current)
    {
    case INIT:
    {
        if (isspace(c))
        {
            current = IN_SPACE;
            return true;
        }

        if (isalpha(c) || c == '_')
        {
            current = IN_IDENT;
            return true;
        }

        switch (c)
        {
        case '0':           current = IN_ZERO;          return true;
        case '1' ... '9':   current = IN_DEC;           return true;
        case '"':           current = IN_STRING;        return true;
        case '\'':          current = IN_CHAR;          return true;

        case '~':
        case '?':           current = IN_OPERATOR;      return true;

        case '+':           current = PLUS;             return true;
        case '-':           current = MINUS;            return true;
        case '*':           current = MULTIPLY;         return true;
        case '/':           current = DIVIDE;           return true;
        case '%':           current = MODULO;           return true;
        case '&':           current = AND;              return true;
        case '|':           current = OR;               return true;
        case '^':           current = XOR;              return true;
        case '<':           current = LESS;             return true;
        case '>':           current = GREATER;          return true;
        case '!':           current = NOT;              return true;
        case '=':           current = EQUAL;            return true;
        case '.':           current = DOT;              return true;

        case '(':
        case ')':
        case '[':
        case ']':
        case '{':
        case '}':
        case ':':
        case ';':
        case ',':           current = IN_PUNCTUATOR;    return true;

        default:                                        return false;
        }
    }
    case PLUS:
    {
        if (c == '+' || c == '=')
        {
            current = IN_OPERATOR;
            return true;
        }

        return false;
    }
    case MINUS:
    {
        if (c == '-' || c == '=')
        {
            current = IN_OPERATOR;
            return true;
        }

        if (c == '>')
        {
            current = IN_PUNCTUATOR;
            return true;
        }

        return false;
    }
    case LEFT_SHIFT:
    case RIGHT_SHIFT:
    case NOT:
    case EQUAL:
    case XOR:
    case MODULO:
    case MULTIPLY:
    {
        if (c == '=')
        {
            current = IN_OPERATOR;
            return true;
        }

        return false;
    }
    case DIVIDE:
    {
        switch (c)
        {
        case '=':       current = IN_OPERATOR;  return true;
        case '/':       current = IN_COMMENT;   return true;
        case '*':       current = IN_MCOMMENT;  return true;
        default:                                return false;
        }
    }
    case AND:
    {
        if (c == '&' || c == '=')
        {
            current = IN_OPERATOR;
            return true;
        }

        return false;
    }
    case OR:
    {
        if (c == '|' || c == '=')
        {
            current = IN_OPERATOR;
            return true;
        }

        return false;
    }
    case LESS:
    {
        if (c == '=')
        {
            current = IN_OPERATOR;
            return true;
        }

        if (c == '<')
        {
            current = LEFT_SHIFT;
            return true;
        }

        return false;
    }
    case GREATER:
    {
        if (c == '=')
        {
            current = IN_OPERATOR;
            return true;
        }

        if (c == '>')
        {
            current = RIGHT_SHIFT;
            return true;
        }

        return false;
    }
    case DOT:
    {
        if (c == '.')
        {
            current = DOTDOT;
            return true;
        }

        if (isdigit(c))
        {
            current = AFTER_DP;
            return true;
        }

        return false;
    }
    case DOTDOT:
    {
        if (c == '.')
        {
            current = IN_PUNCTUATOR;
            return true;
        }

        return false;
    }
    case AFTER_DP:
    {
        if (isdigit(c))
            return true;

        if (c == 'e' || c == 'E')
        {
            current = IN_EXP;
            return true;
        }

        return false;
    }
    case IN_EXP:
    {
        if (isdigit(c))
        {
            current = EXP_END;
            return true;
        }

        if (c == '+' || c == '-')
        {
            current = IN_EXP_SIGN;
            return true;
        }

        return false;
    }
    case IN_EXP_SIGN:
    {
        if (isdigit(c))
        {
            current = EXP_END;
            return true;
        }

        return false;
    }
    case EXP_END:
    {
        return isdigit(c);
    }
    case IN_SPACE:
    {
        return isspace(c);
    }
    case IN_IDENT:
    {
        return isalnum(c) || c == '_';
    }
    case IN_STRING:
    {
        switch (c)
        {
        case '\\':      current = IN_ESCAPE;            return true;
        case '"':       current = STRING_END;           return true;
        case '\n':                                      return false;
        default:                                        return true;
        }
    }
    case IN_ESCAPE:
    {
        current = IN_STRING;
        return true;
    }
    case IN_CHAR:
    {
        switch (c)
        {
        case '\\':      current = IN_CHAR_ESCAPE;       return true;
        case '\'':      current = CHAR_END;             return true;
        case '\n':                                      return false;
        default:                                        return true;
        }
    }
    case IN_CHAR_ESCAPE:
    {
        current = IN_CHAR;
        return true;
    }
    case IN_COMMENT:
    {
        return c != '\n';
    }
    case IN_MCOMMENT:
    {
        if (c == '*')
            current = MAYBE_COM_END;

        return true;
    }
    case MAYBE_COM_END:
    {
        if (c == '/')
            current = COM_END;
        else if (c != '*')
            current = IN_MCOMMENT;

        return true;
    }
    case IN_ZERO:
    {
        switch (c)
        {
        case '0' ... '7':       current = IN_OCTAL;     return true;
        case 'x':
        case 'X':               current = MAYBE_HEX;    return true;
        case 'b':
        case 'B':               current = MAYBE_BIN;    return true;
        case 'e':
        case 'E':               current = IN_EXP;       return true;
        case '.':               current = AFTER_DP;     return true;
        default:                                        return false;
        }
    }
    case IN_DEC:
    {
        if (c == 'e' || c == 'E')
        {
            current = IN_EXP;
            return true;
        }

        if (c == '.')
        {
            current = AFTER_DP;
            return true;
        }

        return isdigit(c);
    }
    case IN_OCTAL:
    {
        return c >= '0' && c <= '7';
    }
    case MAYBE_HEX:
    {
        if (isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
        {
            current = IN_HEX;
            return true;
        }

        return false;
    }
    case IN_HEX:
    {
        return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
    }
    case MAYBE_BIN:
    {
        if (c == '0' || c == '1')
        {
            current = IN_BIN;
            return true;
        }

        return false;
    }
    case IN_BIN:
    {
        return c == '0' || c == '1';
    }
    default:
        return false;
    }
}

TokenType get_token_type()
{
    switch (current)
    {
    case IN_SPACE:          return WHITESPACE;
    case IN_IDENT:          return IDENTIFIER;

    case IN_ZERO:
    case IN_DEC:
    case IN_OCTAL:
    case IN_HEX:
    case IN_BIN:            return INT_LITERAL;

    case AFTER_DP:
    case EXP_END:           return FLOAT_LITERAL;

    case CHAR_END:          return CHAR_LITERAL;
    case STRING_END:        return STRING_LITERAL;

    case PLUS:
    case MINUS:
    case MULTIPLY:
    case DIVIDE:
    case MODULO:
    case AND:
    case OR:
    case XOR:
    case LEFT_SHIFT:
    case RIGHT_SHIFT:
    case LESS:
    case GREATER:
    case EQUAL:
    case NOT:
    case IN_OPERATOR:       return OPERATOR;

    case DOT:
    case IN_PUNCTUATOR:     return PUNCTUATOR;
    case COM_END:
    case IN_COMMENT:        return COMMENT;

    default:                return BAD;
    }
}