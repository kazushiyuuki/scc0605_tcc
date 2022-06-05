#include "automatos.h"

// Inicia o vetor de palavras reservadas
char reservadas[NUM_RESERVADAS][MAX_TAMANHO] = {""};

// Preenche o valor dos possiveis operadores
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

// Preenche o valor dos possiveis operadores especiais
char simb_operadores_especiais[] = {':',
                                    '<',
                                    '>'};

/* 
    Funcao que seleciona qual automato deve ser acionado
    Params:
        - str: linha do arquivo de entrada
        - resultado: ponteiro para o arquivo de saída
        - linha: numero da linha do arquivo de entrada
*/
void lexico(char *str, FILE *resultado, int linha)
{
    // Preenche com as palavras reservadas
    salvaReservadas();
    // i indica o caracter da linha do arquivo de entrada
    // numero salva se o caracter eh um numero ou nao
    int i = 0, numero;
    // Loop para percorrer a linha
    while (str[i] != '\0')
    {
        // Consome os espaços nas linhas e o \n caso haja no final
        if (str[i] == ' ' || str[i] == '\n')
        {
            i++;
        }
        // Caso identifique um comentario, aciona o automato de comentario
        else if (str[i] == '{' || str[i] == '}')
        {
            i = comentario(str, i, resultado, linha);
        }
        // Caso identifique um numero, aciona o automato de numero
        else if ((numero = indentificaNumero(str[i])) == 1)
        {
            i = automatoNumeros(str, i, resultado, linha);
        }
        // Caso identifique um operador, aciona o automato de operador
        else if (eh_operador(str[i]) != 0)
        {
            i = operadores(str, i, resultado);
        }
        // Caso nenhum anterior seja acionado, aciona o automato de identificador
        else
        {
            i = automatoIdentificadores(str, i, resultado, linha);
        }
    }
}

/*
    Funcao que implementa o automato de comentario
    Params:
        - str: linha do arquivo de entrada
        - i: indica o caracter atual de str a ser lido
        - resultado: ponteiro para o arquivo de saida
        - linha: numero da linha do arquivo de entrada
    Return:
        Inteiro com a posicao do proximo caracter a ser consumido
*/
int comentario(char *str, int i, FILE *resultado, int linha)
{
    // Testa se o comentario foi aberto
    if (str[i] == '{')
    {
        // Consome todos os caracteres ate chegar no fim da linha ou do comentario
        while (str[i] != '}' && str[i] != '\n' && str[i] != '\0')
            i++;
        // Caso nao haja um fecha chaves para finalizar o comentario
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

/*
    Funcao que implementa o automato de operadores
    Params:
        - str: linha do arquivo de entrada
        - i: indica o caracter atual de str a ser lido
        - resultado: ponteiro para o arquivo de saida
    Return:
        Inteiro com a posicao do proximo caracter a ser consumido
*/
int operadores(char *str, int i, FILE *resultado)
{
    // Verifica se eh um operador
    // De 10 a 19, operador comum
    // De 20 a 22, operador especial
    int operador = eh_operador(str[i]);
    
    // Identifica o caracter
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
        // Verifica o proximo caracter para saber se eh um operador composto por dois caracteres
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

/*
    Funcao que implementa o automato de identificadores
    Params:
        - palavra: linha do arquivo de entrada
        - i: indica o caracter atual de str a ser lido
        - ponteiro_saida: ponteiro para o arquivo de saida
        - linha: numero da linha do arquivo de entrada
    Return:
        Inteiro com a posicao do proximo caracter a ser consumido
*/
int automatoIdentificadores(char palavra[], int i, FILE *ponteiro_saida, int linha)
{
    // inicio salva a posicao do caracter inicial para saber onde comeca o identificador
    int inicio = i;
    /* 
        - j: variavel para usar nos "for"
        - flag: guarda o estado do automato
            0 - estado inicial
            1 - caso a primeira letra esteja correta
            2 - caso tenha um caracter no meio do indetificador que seja invalido
            3 - caracter invalido logo no inicio
        - flag_reservada: armazena se a palavra eh reservada 
        - cont_vet: serve para copiar o identificador
    */
    int j, flag = 0, flag_reservada = 0, cont_vet = 0;
    // tipo palavra armazena o identificador
    char tipo_palavra[MAX_TAMANHO] = "";
    // c recebe o char a ser consumido pelo automato
    char c = palavra[i];

    // Loop que para de ler o identificador
    while (c != ' ' && c != '\n' && c != '\0')
    {
        // Identifica o estado 
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
            i--; // retrocede
            break;

        case 3: // primeiro errado, imprime erro
            i--;
            fprintf(ponteiro_saida, "%c -> (ERRO, CARACTER INVALIDO) - LINHA %d\n", palavra[i], linha);
            i++;
            return i;

        default:
            break;
        }
        // Consome o resto caso nao tenha erro 
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

    // Copia o identificador para tipo_palavra
    for (j = inicio; j < i; j++)
    {
        tipo_palavra[cont_vet++] = palavra[j];
    }
    tipo_palavra[cont_vet] = '\0';

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
    // Caso haja erro
    else
    {   
        char msg_erro[50];
        snprintf(msg_erro, sizeof(msg_erro), " -> (ERRO, PALAVRA INVALIDA) - LINHA %d\n", linha);
        strcat(tipo_palavra, msg_erro);
    }
    fputs(tipo_palavra, ponteiro_saida);

    return i;
}

/*
    Funcao que implementa o automato de numeros
    Params:
        - palavra: linha do arquivo de entrada
        - i: indica o caracter atual de str a ser lido
        - ponteiro_saida: ponteiro para o arquivo de saida
        - linha: numero da linha do arquivo de entrada
    Return:
        Inteiro com a posicao do proximo caracter a ser consumido
*/
int automatoNumeros(char palavra[], int i, FILE *ponteiro_saida, int linha)
{

     /* 
        - j: variavel para usar nos "for"
        - inicio: salva a posicao do caracter inicial para saber onde comeca o numero
        - cont_vet: serve para copiar o identificador
        - flag: guarda o estado do automato
            0 - estado inicial
            1 - caracter nao eh um numero
            2 - primero caracter eh um numero inicial
            3 - possui parte decimal
        - flag_operador: armazena se o caracter eh um operador
        - c: caracter atual do numero
        - tipo_palavra: armazena o numero inteiro
    */
    int j, inicio = i, cont_vet = 0;
    int flag = 0, flag_operador = 0;
    char c = palavra[i];
    char tipo_palavra[MAX_TAMANHO];

    // Loop enquanto não chegar no fim do numero ou identificar um operador
    while (c != ' ' && c != '\n' && c != '\0' && (flag_operador == 0))
    {
        switch (flag)
        {
        // primeiro caracter
        case 0:
            if (c >= '0' && c <= '9')
            {
                flag = 2; // primeiro numero ok
            }
            else
            {
                flag = 1; // caracter invalido
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
        // caso seja um caracter invalido, verifica se eh um identificador
        // se nao for, continua lendo o numero
        // se for para o automato para que o o automato de operador assuma
        if (flag == 1 && eh_operador(c) != 0)
        {
            flag_operador = 1;
            // muda flag para identificar o numero antes do operador
            flag = -1;
        }
        else
        {
            i++;
            c = palavra[i];
        }
    }

    //  Copia o numero para tipo_palavra
    for (j = inicio; j < i; j++)
    {
        tipo_palavra[cont_vet++] = palavra[j];
    }
    tipo_palavra[cont_vet] = '\0';

    // Testa se eh uma numero invalido
    if (flag == 1)
    {
        char msg_erro[50];
        snprintf(msg_erro, sizeof(msg_erro), " -> (ERRO, NUMERO INVALIDO) - LINHA %d\n", linha);
        strcat(tipo_palavra, msg_erro);
        fputs(tipo_palavra, ponteiro_saida);
    }
    else
    {
        strcat(tipo_palavra, " -> simb_num\n");
        fputs(tipo_palavra, ponteiro_saida);
    }
    return i;
}

/* 
    Funcao que verifica se um caracacter eh um operador
    Params:
        - c: char a ser verificado
    Return:
        Inteiro da seguinte forma:
            0 - nao eh um operador
            10 a 19 - operador comum
            20 a 22 - operador especial que pode ser formado por dois caracteres
*/
int eh_operador(char c)
{
    // Verifica se o caracter esta no vetor de operadores comuns
    for (int i = 0; i < 10; i++)
    {
        if (c == simb_operadores[i])
            return 10 + i;
    }
    // Verifica se o caracter esta no vetor de operadores especiais
    for (int i = 0; i < 3; i++)
    {
        if (c == simb_operadores_especiais[i])
            return 20 + i;
    }
    // Caso nao seja um operador
    return 0;
}

/*
    Funcao que identifica se um caracter eh um numero
    Params:
        - c: caracter a ser verificado
    Return:
        Inteiro da seguinte forma:
            1 - eh um numero
            0 - nao eh um numero
*/
int indentificaNumero(char c)
{
    // Verifica se eh um numero de acordo com o decimal da tabela ascii
    for (int i = 48; i < 58; i++)
    {
        if (c == i)
            return 1;
    }
    return 0;
}

// Funcao que armazena as palavras reservadas
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