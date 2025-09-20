#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TJDF.h"

#define N 2600000
#define TAM_LINHA 2048
#define ERRO -1

Processo* carregarProcessos(const char* arquivo, int* qtd) {

    FILE *fp = fopen(arquivo, "r");
    if (!fp) {
        printf("Arquivo inexistente!\n");
        return NULL;
    }

    char linha[TAM_LINHA];
    fgets(linha, TAM_LINHA, fp); // pula o cabeçalho

    Processo *lista = malloc(N * sizeof(Processo));
    if (!lista) {
        printf("Erro de alocação!\n");
        fclose(fp);
        return NULL;
    }

    int contador = 0;
    while (fgets(linha, TAM_LINHA, fp)) {
        if (contador >= N) break;

        // Remove \n ou \r\n
        linha[strcspn(linha, "\r\n")] = 0;

        Processo *p = &lista[contador];

        char *token;
        int campo = 0;

        // Inicializa todos os campos numéricos com 0
        p->id_tribunal = 0;
        p->recurso = 0;
        p->flag_violencia_domestica = 0;
        p->flag_feminicidio = 0;
        p->flag_ambiental = 0;
        p->flag_quilombolas = 0;
        p->flag_indigenas = 0;
        p->flag_infancia = 0;
        p->cnm1 = 0;
        p->primeirasentm1 = 0;
        p->baixm1 = 0;
        p->decm1 = 0;
        p->mpum1 = 0;
        p->julgadom1 = 0;
        p->desm1 = 0;
        p->susm1 = 0;

        token = strtok(linha, ";");
        while (token) {
            switch(campo) {
                case 0: strncpy(p->id_processo, token, sizeof(p->id_processo)-1); break;
                case 1: strncpy(p->numero_sigilo, token, sizeof(p->numero_sigilo)-1); break;
                case 2: strncpy(p->sigla_grau, token, sizeof(p->sigla_grau)-1); break;
                case 3: strncpy(p->procedimento, token, sizeof(p->procedimento)-1); break;
                case 4: strncpy(p->ramo_justica, token, sizeof(p->ramo_justica)-1); break;
                case 5: strncpy(p->sigla_tribunal, token, sizeof(p->sigla_tribunal)-1); break;
                case 6: p->id_tribunal = atoi(token); break;
                case 7: p->recurso = atoi(token); break;
                case 8: strncpy(p->id_ultimo_oj, token, sizeof(p->id_ultimo_oj)-1); break;
                case 9: strncpy(p->dt_recebimento, token, sizeof(p->dt_recebimento)-1); break;
                case 10: strncpy(p->id_ultima_classe, token, sizeof(p->id_ultima_classe)-1); break;
                case 11: p->flag_violencia_domestica = atoi(token); break;
                case 12: p->flag_feminicidio = atoi(token); break;
                case 13: p->flag_ambiental = atoi(token); break;
                case 14: p->flag_quilombolas = atoi(token); break;
                case 15: p->flag_indigenas = atoi(token); break;
                case 16: p->flag_infancia = atoi(token); break;
                case 17: strncpy(p->decisao, token, sizeof(p->decisao)-1); break;
                case 18: strncpy(p->dt_resolvido, token, sizeof(p->dt_resolvido)-1); break;
                case 19: p->cnm1 = atoi(token); break;
                case 20: p->primeirasentm1 = atoi(token); break;
                case 21: p->baixm1 = atoi(token); break;
                case 22: p->decm1 = atoi(token); break;
                case 23: p->mpum1 = atoi(token); break;
                case 24: p->julgadom1 = atoi(token); break;
                case 25: p->desm1 = atoi(token); break;
                case 26: p->susm1 = atoi(token); break;
            }

            campo++;
            token = strtok(NULL, ";");
        }

        // Print do processo lido
      
        contador++;
    }

    fclose(fp);
    *qtd = contador;
    return lista;
}





  /*
Processo * carregarProcessos(char * arquivo, int * qtd) {

    Processo p;
    int contador;

    FILE * fp = fopen("./TJDFT_amostra.csv", "r");

    if (fp == NULL) {
        printf("Arquivo inexistente!");
        return 1;
    }   
    
    char Cabecalho[TAM_LINHA];
    fscanf(fp, "%s", Cabecalho); //Desconsiderar o cabeçalho
    while (fscanf(fp,
        "%63[^;];%63[^;];%7[^;];%127[^;];%7[^;];%15[^;];%d;"
        "%d;%63[^;];%19[^;];%63[^;];%d;%d;%d;%d;%d;%d;"
        "%127[^;];%19[^;];%d;%d;%d;%d;%d;%d;%d;%d",
        p.id_processo,
        p.numero_sigilo,
        p.sigla_grau,
        p.procedimento,
        p.ramo_justica,
        p.sigla_tribunal,
        &p.id_tribunal,
        &p.recurso,
        p.id_ultimo_oj,
        p.dt_recebimento,
        p.id_ultima_classe,
        &p.flag_violencia_domestica,
        &p.flag_feminicidio,
        &p.flag_ambiental,
        &p.flag_quilombolas,
        &p.flag_indigenas,
        &p.flag_infancia,
        p.decisao,
        p.dt_resolvido,
        &p.cnm1,
        &p.primeirasentm1,
        &p.baixm1,
        &p.decm1,
        &p.mpum1,
        &p.julgadom1,
        &p.desm1,
        &p.susm1
    ) == 26) {
        printf("ID=%s | Sigilo=%s | Grau=%s | Procedimento=%s\n",
               p.id_processo, p.numero_sigilo, p.sigla_grau, p.procedimento);
    }
    
   
  

     
    
    

    fclose(fp);
    return 0;   
}
*/

