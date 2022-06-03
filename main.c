#include "automatos.h"

int main(int argc, char const *argv[])
{
    FILE *pont;
    pont = fopen("exemplo.txt", "r");

    if (pont == NULL)
    {
        printf("nao foi possivel abrir o arquivo\n");
        return 1;
    }

    FILE *resultado;
    resultado = fopen("resultado.txt", "w");

    if(resultado == NULL)
    {
        printf("nao foi possivel criar o arquivo\n");
        return 1;
    }

    char buffer[120];
    while (fgets(buffer, 120, pont))
    {
        lexico(buffer, resultado);
    }

    fclose(resultado);
    fclose(pont);

    return 0;
}
