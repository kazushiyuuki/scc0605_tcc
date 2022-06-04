#include "automatos.h"

char reservadas[NUM_RESERVADAS][MAX_TAMANHO] = {""};

char simb_operadores[] = {'+',
                          '-',
                          '*',
                          '/',
                          '.',
                          ',',
                          ';',
                          '=',
                          '(',
                          ')'};

char simb_operadores_especiais[] = {':',
                                    '<',
                                    '>'};

void lexico(char *str, FILE *resultado, int linha)
{
    salvaReservadas();
    int i = 0, numero;
    while (str[i] != '\0')
    {

        if (str[i] == ' ' || str[i] == '\n')
        {
            i++;
        }
        else if (str[i] == '{' || str[i] == '}')
        {
            i = comentario(str, i, resultado, linha);
        }
        // automato numero
        else if ((numero = indentificaNumero(str[i])) == 1)
        {
            i = automatoNumeros(str, i, resultado);
        }
        else if (eh_operador(str[i]) != 0)
        {
            i = operadores(str, i, resultado);
        }
        // automato identificador e reservada
        else
        {
            i = automatoIdentificadores(str, i, resultado);
        }
    }
}

int comentario(char *str, int i, FILE *resultado, int linha)
{
    if (str[i] == '{')
    {
        while (str[i] != '}' && str[i] != '\n' && str[i] != '\0')
            i++;
        if (str[i] != '}')
        {
            fprintf(resultado, "(ERRO, COMENTARIO NAO FINALIZADO) - LINHA %d\n", linha);
        }
    }
    else
    {
        fprintf(resultado, "(ERRO, COMENTARIO NAO ABERTO) - LINHA %d\n", linha);
    }
    i++;
    return i;
}

int operadores(char *str, int i, FILE *resultado)
{
    int operador = eh_operador(str[i]);
    switch (operador)
    {
    case 10:
        fprintf(resultado, "%c -> simb_mais\n", str[i]);
        i++;
        return i;

    case 11:
        fprintf(resultado, "%c -> simb_menos\n", str[i]);
        i++;
        return i;

    case 12:
        fprintf(resultado, "%c -> simb_mult\n", str[i]);
        i++;
        return i;

    case 13:
        fprintf(resultado, "%c -> simb_div\n", str[i]);
        i++;
        return i;

    case 14:
        fprintf(resultado, "%c -> simb_ponto\n", str[i]);
        i++;
        return i;

    case 15:
        fprintf(resultado, "%c -> simb_virgula\n", str[i]);
        i++;
        return i;

    case 16:
        fprintf(resultado, "%c -> simb_pv\n", str[i]);
        i++;
        return i;

    case 17:
        fprintf(resultado, "%c -> simb_igual\n", str[i]);
        i++;
        return i;

    case 18:
        fprintf(resultado, "%c -> simb_par\n", str[i]);
        i++;
        return i;

    case 19:
        fprintf(resultado, "%c -> simb_fpar\n", str[i]);
        i++;
        return i;

    case 20:
        i++;
        if (str[i] == '=')
        {
            fprintf(resultado, "%c= -> simb_atrib\n", str[i - 1]);
            i++;
            return i;
        }
        else
        {
            fprintf(resultado, "%c -> simb_dp\n", str[i - 1]);
            return i;
        }

    case 21:
        i++;
        if (str[i] == '=')
        {
            fprintf(resultado, "%c= -> simb_menor_igual\n", str[i - 1]);
            i++;
            return i;
        }
        else if (str[i] == '>')
        {
            fprintf(resultado, "%c> -> simb_dif\n", str[i - 1]);
            i++;
            return i;
        }
        else
        {
            fprintf(resultado, "%c -> simb_menor\n", str[i - 1]);
            return i;
        }

    case 22:
        i++;
        if (str[i] == '=')
        {
            fprintf(resultado, "%c= -> simb_maior_igual\n", str[i - 1]);
            i++;
            return i;
        }
        else
        {
            fprintf(resultado, "%c -> simb_maior\n", str[i - 1]);
            return i;
        }
    }
}

int automatoIdentificadores(char palavra[], int i, FILE *ponteiro_saida)
{
    // Variaveis
    int inicio = i, verifica;
    int j, flag = 0, flag_reservada = 0, cont_vet = 0;
    char tipo_palavra[MAX_TAMANHO] = "";
    char c = palavra[i];

    // Comeca o processo de ler a palavra
    while (c != ' ' && c != '\n' && c != '\0')
    {
        switch (flag)
        {
        case 0: // verifica a primeira letra
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            {
                flag = 1; // caso a primeira letra esteja ok
            }
            else
            {
                flag = 3; // caso seja invalido
            }
            break;

        case 1: // verifica o restante das letras
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_'))
            {

                flag = 1; // caso o simbolo esteja OK, continua
            }
            else
            {
                flag = 2; // caso seja invalido
            }
            break;
        case 2: // ERRO
            i--;
            break;

        case 3: // primeiro errado
            i--;
            fprintf(ponteiro_saida, "%c -> (ERRO, CARACTER INVALIDO)\n", palavra[i]);
            i++;
            return i;

        default:
            break;
        }
        if (flag != 2)
        {
            i++;
            c = palavra[i];
        }
        else
        {
            flag = 1;
            break;
        }
    }
    for (j = inicio; j < i; j++)
    {
        tipo_palavra[cont_vet++] = palavra[j];
    }

    tipo_palavra[i] = '\0';

    // Verifica se e reservada ou nao
    if (flag != 2 && flag != 3)
    {
        for (j = 0; j < NUM_RESERVADAS; j++)
        {
            if (strcmp(tipo_palavra, reservadas[j]) == 0)
            {
                flag_reservada = 1;
                strcat(tipo_palavra, " -> simb_");
                strcat(tipo_palavra, reservadas[j]);
                strcat(tipo_palavra, "\n");
            }
        }
        if (flag_reservada != 1)
        {
            strcat(tipo_palavra, " -> id\n");
        }
    }
    else
    {
        strcat(tipo_palavra, " -> (ERRO, PALAVRA INVALIDA)\n");
    }
    fputs(tipo_palavra, ponteiro_saida);

    return i;
}

int automatoNumeros(char palavra[], int i, FILE *ponteiro_saida)
{

    // Variaveis
    int j, inicio = i, cont_vet = 0;
    int flag = 0, flag_operador = 0;
    char c = palavra[i];
    char tipo_palavra[MAX_TAMANHO];

    while (c != ' ' && c != '\n' && c != '\0' && (flag_operador == 0))
    {
        switch (flag)
        {
        // primeiro caracter
        case 0:
            if (c == '+' || c == '-' || (c >= '0' && c <= '9'))
            {
                flag = 2;
            }
            else
            {
                flag = 1;
            }

            break;

        // caracter invalido
        case 1:
            flag = 1;
            break;

        // caracter numero ou inicio da parte decimal
        case 2:
            if ((c >= '0' && c <= '9'))
                flag = 2;
            else if (c == '.')
                flag = 3;
            else
                flag = 1;
            break;

        // parte decimal
        case 3:
            if ((c >= '0' && c <= '9'))
                flag = 3;
            else
                flag = 1;
            break;

        default:
            break;
        }
        if (flag == 1 && eh_operador(c) != 0)
        {
            flag_operador = 1;
            flag = -1;
        }
        else
        {
            i++;
            c = palavra[i];
        }
    }

    // TODO: implementar a string de saida
    for (j = inicio; j < i; j++)
    {
        tipo_palavra[cont_vet++] = palavra[j];
    }

    tipo_palavra[cont_vet] = '\0';
    if (flag == 1)
    {
        strcat(tipo_palavra, " -> (ERRO, NUMERO INVALIDO)\n");
        fputs(tipo_palavra, ponteiro_saida);
    }
    else
    {
        strcat(tipo_palavra, " -> simb_num\n");
        fputs(tipo_palavra, ponteiro_saida);
    }
    return i;
}

int eh_operador(char c)
{
    for (int i = 0; i < 10; i++)
    {
        if (c == simb_operadores[i])
            return 10 + i;
    }
    for (int i = 0; i < 3; i++)
    {
        if (c == simb_operadores_especiais[i])
            return 20 + i;
    }
    return 0;
}

int indentificaNumero(char c)
{
    for (int i = 48; i < 58; i++)
    {
        if (c == i)
            return 1;
    }
    return 0;
}
int identificaCaracterValido(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c != '_'))
        return 1;
    return 0;
}

void salvaReservadas()
{
    strcpy(reservadas[0], "program");
    strcpy(reservadas[1], "var");
    strcpy(reservadas[2], "integer");
    strcpy(reservadas[3], "real");
    strcpy(reservadas[4], "begin");
    strcpy(reservadas[5], "end");
    strcpy(reservadas[6], "if");
    strcpy(reservadas[7], "else");
    strcpy(reservadas[8], "then");
    strcpy(reservadas[9], "for");
    strcpy(reservadas[10], "do");
    strcpy(reservadas[11], "to");
    strcpy(reservadas[12], "while");
    strcpy(reservadas[13], "write");
    strcpy(reservadas[14], "read");
    strcpy(reservadas[15], "const");
    strcpy(reservadas[16], "procedure");
}