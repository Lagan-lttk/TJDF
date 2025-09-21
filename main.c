#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TJDF.h"

int main() {

    int qtd;
    Processo *processos = carregarProcessos("./TJDFT_amostra.csv", &qtd);
    if(!processos) return 1;
    
    const char* idBusca = "323961063";
    const char* ultimoOJ = buscarUltimoOJ(processos, qtd, idBusca);

    if (ultimoOJ) {
        printf("Último OJ do processo %s: %s\n", idBusca, ultimoOJ);
    } else {
        printf("Processo %s não encontrado.\n", idBusca);
    }

    const char* maisAntigo = processoMaisAntigo(processos, qtd);
     if (maisAntigo) {
        printf("Processo mais antigo: %s\n", maisAntigo);
    } else {
        printf("Nenhum processo encontrado.\n");
    }
    
    const int contadorViolenciaDomestica = contarViolenciaDomestica(processos,qtd);
    printf("-- %d\n", contadorViolenciaDomestica);

    const int contadorFeminicidio = contarFeminicidio(processos,qtd);
    printf("-- %d\n", contadorFeminicidio);

    const int contadorAmbiental = contarAmbiental(processos,qtd);
    printf("-- %d\n", contadorAmbiental);

    const int contadorQuilombola = contarQuilombolas(processos,qtd);
    printf("-- %d\n", contadorQuilombola);

    const int contadorIndigena = contarIndigenas(processos,qtd);
    printf("-- %d\n", contadorIndigena);

    const int contadorInfanciaeJuventude = contarInfanciaeJuventude(processos,qtd);
    printf("-- %d\n", contadorInfanciaeJuventude);


    const int diferenca = diferencaDeDias(processos,qtd,idBusca);
    printf("-- %d\n", diferenca);

    //printf("\nTotal de processos lidos: %d\n", qtd);

    //free(processos);
}
