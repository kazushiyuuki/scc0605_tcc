#include "automatos.h"

/*
    Breve descrição da estrutura do programa:
    main.c
        Abre o arquivo de entrada, cria um arquivo de saída, lê linha por linha do arquivo entrada e
        passa para a função "lexico" contida em automatos.c.
    automatos.h
        Header para declarar as funções de automatos.c, algumas variaveis globais e algumas constantes.
    automatos.c
        Contém a função "lexico" que decide qual autômato acionar e as funções que implementam cada autômato:
        comentário, operadores, números e identificadores.
*/
int main(int argc, char const *argv[])
{
    // Abre o arquivo de entrada
    FILE *pont;
    // Se for passado o nome do arquivo como parametro, abre esse arquivo
    if (argc >= 2)
        pont = fopen(argv[1], "r");
    // Caso contrario, abre-se o "exemplo.txt"
    else
        pont = fopen("exemplo.txt", "r");

    if (pont == NULL)
    {
        printf("nao foi possivel abrir o arquivo\n");
        return 1;
    }

    // Cria o arquivo de saida
    FILE *resultado;
    resultado = fopen("resultado.txt", "w");

    if (resultado == NULL)
    {
        printf("nao foi possivel criar o arquivo\n");
        return 1;
    }

    // Le linha por linha e passa para a funcao "lexico()" em automatos.c
    char buffer[120];
    int contador = 1;
    while (fgets(buffer, 120, pont))
    {
        lexico(buffer, resultado, contador);
        contador++;
    }

    fclose(resultado);
    fclose(pont);

    return 0;
}
