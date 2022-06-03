#include <stdlib.h>
#include <stdio.h>

extern char simb_operadores[];
extern char simb_operadores_especiais[];

void lexico(char *str, FILE *resultado);
int comentario(char *str, int i, FILE *resultado);
int eh_operador(char c);
