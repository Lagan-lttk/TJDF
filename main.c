#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TJDF.h"

int main() {

    int qtd;
    Processo *processos = carregarProcessos("./TJDFT_filtrado.csv", &qtd);
    if (!processos) return 1;

    printf("\nTotal de processos lidos: %d\n", qtd);

    free(processos);
    return 0;
}
