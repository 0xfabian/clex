#include <cstdio>

#include "lexer.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "%s: \e[91merror:\e[0m no input files\n", argv[0]);
        return 1;
    }

    bool multiple_files = argc > 2;
    int exit_status = 0;

    for (int i = 1; i < argc; i++)
    {
        FILE* file = fopen(argv[i], "r");

        if (!file)
        {
            perror(argv[i]);
            exit_status = 1;
            continue;
        }

        if (multiple_files)
            printf("%s:\n", argv[i]);

        Lexer lex(file);
        Token tok;

        while ((tok = lex.get_next()).type != END)
        {
            if (multiple_files)
                printf("\t");

            if (tok.type == BAD)
                printf("\e[91m");

            printf("%-*s\e[0m%s\n", 20, type_strings[tok.type], get_str(tok.index));
        }

        fclose(file);
    }

    return exit_status;
}