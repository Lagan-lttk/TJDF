#include <stdio.h>
#include <stdlib.h>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct {

    char id_processo[64];
    char numero_sigilo[64];
    char sigla_grau[8];
    char procedimento[128];
    char ramo_justica[8];
    char sigla_tribunal[16];
    int id_tribunal;
    int recurso;
    char id_ultimo_oj[64];
    char dt_recebimento[20];
    char id_ultima_classe[64];
    int flag_violencia_domestica;
    int flag_feminicidio;
    int flag_ambiental;
    int flag_quilombolas;
    int flag_indigenas;
    int flag_infancia;
    char decisao[128];
    char dt_resolvido[20];
    int cnm1;
    int primeirasentm1;
    int baixm1;
    int decm1;
    int mpum1;
    int julgadom1;
    int desm1;
    int susm1;

} Processo;

Processo* carregarProcessos(const char* arquivo, int* qtd);
int contarProcessos(const Processo* lista, int qtd);
const char* buscarUltimoOJ(const Processo* lista, int qtd, const char* id_processo);
const char* processoMaisAntigo(const Processo* lista, int qtd);
int contarViolenciaDomestica(const Processo* lista, int qtd);
int contarFeminicidio(const Processo* lista, int qtd);
int contarAmbiental(const Processo* lista, int qtd);
int contarQuilombolas (const Processo* lista, int qtd);
int contarIndigenas (const Processo* lista, int qtd);
int contarInfanciaeJuventude (const Processo* lista, int qtd);
int compararData(const char* d1, const char* d2);
int diferencaDeDias(const Processo* lista,int qtd, const char* id_processo);



#endif