#include "automatos.h"

char simb_operadores[] = {'+',
                          '-',
                          '*',
                          '/',
                          '.',
                          ',',
                          ';',
                          '='};

char simb_operadores_especiais[] = {':',
                                    '<',
                                    '>'};

void lexico(char *str, FILE *resultado)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == ' ' || str[i] == '\n')
        {
            i++;
        }
        else if (str[i] == '{' || str[i] == '}')
        {
            i = comentario(str, i, resultado);
        }
        // automato numero
        // automato identificador e reservada
        // automato operadores
        // automato comentario
    }
}

int comentario(char *str, int i, FILE *resultado)
{
    if (str[i] == '{')
    {
        while (str[i] != '}' && str[i] != '\n' && str[i] != '\0')
            i++;
        if (str[i] == '}')
        {
            fprintf(resultado, "Deu bom!\n");
        }
        else
        {
            fprintf(resultado, "Erro! Comentário não finalizado!\n");
        }
    }
    else
    {
        fprintf(resultado, "Erro! Comentário não aberto!\n");
    }
    i++;
    return i;
}

int operadores(char *str, int i, FILE *resultado)
{
    if (eh_operador(str[i]) != 0)
    {
        if (eh_operador(str[i]) == 1)
        {
        }
    }
}

int eh_operador(char c)
{
    for (int i = 0; i < 8; i++)
    {
        if (c == simb_operadores[i])
            return 1;
    }
    for (int i = 0; i < 3; i++)
    {
        if (c == simb_operadores_especiais[i])
            return 2;
    }
    return 0;
}