#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TJDF.h"

int main() {
    
    //índice quantidade de processos
    int qtd; 

    //vetor para a lista de arquivos
    Processo *processos = carregarProcessos("./TJDFT_filtrado.csv", &qtd);

    //Caso o vetor não exista retorna 1
    if(!processos) return 1;
    
    //Id para exemplificação das funções
    const char* idBusca = "325359695";

    //Variaveis criadas para armazenar o valor das funções, juntamente de condicionais para tratar erros

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
    
    const int diferenca = diferencaDeDias(processos, qtd, idBusca);

    //Tabela com os valores filtrados.

    printf("========== Filtro TJDF ==========\n");
    contarProcessos(processos, qtd);
    printf("- Ultimo Orgao Julgador a partir do ID - 323961063: %d;\n", ultimoOJ);
    printf("- Data do Processo mais antigo: %s;\n", maisAntigo);
    printf("- Diferenca de Recebimento para Cumprimento do ID - 323961063: %d dias;\n", diferenca);
    
    //Tabela com os valores de cada Flag

    printf("\n========== Flags ==========\n");
    const int contadorViolenciaDomestica = contarViolenciaDomestica(processos,qtd);
    printf("- Violencia Domestiva: %d\n", contadorViolenciaDomestica);
    
    const int contadorFeminicidio = contarFeminicidio(processos,qtd);
    printf("- Feminicidio: %d\n", contadorFeminicidio);

    const int contadorAmbiental = contarAmbiental(processos,qtd);
    printf("- Materia ambiental: %d\n", contadorAmbiental);
    
    const int contadorQuilombola = contarQuilombolas(processos,qtd);
    printf("- Comunidades quilombolas: %d\n", contadorQuilombola);
    
    const int contadorIndigena = contarIndigenas(processos,qtd);
    printf("- Povos indigenas: %d\n", contadorIndigena);
    
    const int contadorInfanciaeJuventude = contarInfanciaeJuventude(processos,qtd);
    printf("- ECA: %d\n", contadorInfanciaeJuventude);
    
    //Tabela com relação ao calculo e criação do arquivo com base na meta
    
    printf("\n========== Meta ==========\n");
    processosJulgados(processos,qtd,"filtrado.csv");
    printf(" - Meta 01: %f\n", cumprimentoMeta(processos, qtd));
}