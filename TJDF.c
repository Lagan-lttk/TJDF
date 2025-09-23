#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TJDF.h"

#define N 2600000 // Numero base de processos no arquivo
#define TAM_LINHA 2048 // Numero maximo caracteres em cada linha

// Função auxiliar pra ler int (trata vazio como 0)
int lerInt(char *linha) {
    if (linha == NULL || strlen(linha) == 0) {
        return 0;
    }
    return atoi(linha); 
}

// Função auxiliar pra ler string (trata vazio como "")
void lerStr(char *dest, char *linha, size_t tamanho) {
    if (linha == NULL || strlen(linha) == 0) {
        dest[0] = '\0';  // String vazia
        return;
    }
    strncpy(dest, linha, tamanho - 1);
    dest[tamanho - 1] = '\0';  // Garante terminação segura
}

int compararData(const char* d1, const char* d2) {
    return strcmp(d1, d2);
}

Processo* carregarProcessos(const char* arquivo, int* qtd) {
    FILE *fp = fopen(arquivo, "r");
    if (!fp) {
        printf("Arquivo inexistente!\n");
        *qtd = 0;
        return NULL;
    }

    char linha[TAM_LINHA];
    fgets(linha, TAM_LINHA, fp);  // Pula o cabeçalho

    Processo *lista = malloc(N * sizeof(Processo));
    if (!lista) {
        printf("Erro de alocação!\n");
        fclose(fp);
        *qtd = 0;
        return NULL;
    }

    int contador = 0;

    while (fgets(linha, TAM_LINHA, fp)) {
        if (contador >= N) break;

        linha[strcspn(linha, "\n")] = '\0';

        Processo *p = &lista[contador];
        memset(p, 0, sizeof(Processo));

        char *resto = linha;
        int campo_index = 0;

        while (campo_index < 27) {
            char *linha = NULL;
            size_t pos = strcspn(resto, ";");
            
            if (pos == 0) {
                linha = "";
                if (*resto == '\0') break;
                resto++;
            } else {
                linha = malloc(pos + 1);
                if (linha) {
                    strncpy(linha, resto, pos);
                    linha[pos] = '\0';
                } else {
                    linha = "";
                }
                resto += pos + 1;
            }

            switch (campo_index) {
                case 0: lerStr(p->id_processo, linha, sizeof(p->id_processo)); break;
                case 1: lerStr(p->numero_sigilo, linha, sizeof(p->numero_sigilo)); break;
                case 2: lerStr(p->sigla_grau, linha, sizeof(p->sigla_grau)); break;
                case 3: lerStr(p->procedimento, linha, sizeof(p->procedimento)); break;
                case 4: lerStr(p->ramo_justica, linha, sizeof(p->ramo_justica)); break;
                case 5: lerStr(p->sigla_tribunal, linha, sizeof(p->sigla_tribunal)); break;
                case 6: p->id_tribunal = lerInt(linha); break;
                case 7: p->recurso = lerInt(linha); break;
                case 8: lerStr(p->id_ultimo_oj, linha, sizeof(p->id_ultimo_oj)); break;
                case 9: lerStr(p->dt_recebimento, linha, sizeof(p->dt_recebimento)); break;
                case 10: lerStr(p->id_ultima_classe, linha, sizeof(p->id_ultima_classe)); break;
                case 11: p->flag_violencia_domestica = lerInt(linha); break;
                case 12: p->flag_feminicidio = lerInt(linha); break;
                case 13: p->flag_ambiental = lerInt(linha); break;
                case 14: p->flag_quilombolas = lerInt(linha); break;
                case 15: p->flag_indigenas = lerInt(linha); break;
                case 16: p->flag_infancia = lerInt(linha); break;
                case 17: lerStr(p->decisao, linha, sizeof(p->decisao)); break;
                case 18: lerStr(p->dt_resolvido, linha, sizeof(p->dt_resolvido)); break;
                case 19: p->cnm1 = lerInt(linha); break;
                case 20: p->primeirasentm1 = lerInt(linha); break;
                case 21: p->baixm1 = lerInt(linha); break;
                case 22: p->decm1 = lerInt(linha); break;
                case 23: p->mpum1 = lerInt(linha); break;
                case 24: p->julgadom1 = lerInt(linha); break;
                case 25: p->desm1 = lerInt(linha); break;
                case 26: p->susm1 = lerInt(linha); break;
            }

            if (pos != 0 && linha && linha != "") {
                free(linha);
            }

            campo_index++;
        }

        contador++;
    }

    fclose(fp);
    *qtd = contador;
    
    return lista;
}

int contarProcessos(const Processo* lista, int qtd) {
    printf("Carregados %d processos com sucesso.\n", qtd);
    return qtd;
}

const char* buscarUltimoOJ(const Processo* lista, int qtd, const char* id_processo) {
    for (int i = 0; i < qtd; i++) { 
        if (strcmp(lista[i].id_processo, id_processo) == 0) {
            return lista[i].id_ultimo_oj;
        }   
    }
    return NULL;
}

const char* processoMaisAntigo(const Processo* lista, int qtd) {
    if (qtd <= 0) return NULL;

    int idxMaisAntigo = -1;
    for (int i = 0; i < qtd; i++) {
        if (strlen(lista[i].dt_recebimento) == 0) continue;  // Pula vazias

        if (idxMaisAntigo == -1 || compararData(lista[i].dt_recebimento, lista[idxMaisAntigo].dt_recebimento) < 0) {
            idxMaisAntigo = i;
        }
    }

    if (idxMaisAntigo == -1) return NULL;  // Nenhuma data válida
    return lista[idxMaisAntigo].dt_recebimento;
}

int contarViolenciaDomestica(const Processo* lista, int qtd) {
    int contador = 0;
    for (int i = 0; i < qtd; i++) {
        if (lista[i].flag_violencia_domestica == 1) contador++;
    }
    return contador;
}

int contarFeminicidio(const Processo* lista, int qtd) {
    int contador = 0;
    for (int i = 0; i < qtd; i++) {
        if (lista[i].flag_feminicidio == 1) contador++;
    }
    return contador;
}

int contarAmbiental(const Processo* lista, int qtd) {
    int contador = 0;
    for (int i = 0; i < qtd; i++) {
        if (lista[i].flag_ambiental == 1) contador++;
    }
    return contador;
}

int contarQuilombolas(const Processo* lista, int qtd) {
    int contador = 0;
    for (int i = 0; i < qtd; i++) {
        if (lista[i].flag_quilombolas == 1) contador++;
    }
    return contador;
}

int contarIndigenas(const Processo* lista, int qtd) {
    int contador = 0;
    for (int i = 0; i < qtd; i++) {
        if (lista[i].flag_indigenas == 1) contador++;
    }
    return contador;
}

int contarInfanciaeJuventude(const Processo* lista, int qtd) {
    int contador = 0;
    for (int i = 0; i < qtd; i++) {
        if (lista[i].flag_infancia == 1) contador++;
    }
    return contador;
}

int DataParaDias (int Ano, int Mes, int dia ){
    
    int Meses[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    
    int verificarBisexto = ( (Ano % 4 == 0 && Ano % 100 != 0) || (Ano % 400 == 0) );

    int MaxDias = Meses[Mes - 1];
    
    if (Mes == 2 && verificarBisexto) 
    {
        MaxDias = 29;
    }
    
    if (dia > MaxDias) 
    {
        return -1;
    }

    int AnosCompletos = Ano - 1;
    int Diasbissextos = AnosCompletos / 4 - AnosCompletos / 100 + AnosCompletos / 400;
    int DiasAnos = AnosCompletos * 365 + Diasbissextos;

    int DiasMes = 0;
    
    for (int i = 0; i < Mes - 1; i++){
        DiasMes += Meses[i];
    } 
    if (verificarBisexto && Mes > 2) DiasMes += 1;

    int total = DiasAnos + DiasMes + dia;
    return total;
}

int diferencaDeDias(const Processo* lista, int qtd, const char* id_processo) {
    for (int i = 0; i < qtd; i++) {
        if (strcmp(lista[i].id_processo, id_processo) == 0) {
            int dia,mes,ano,dia2,mes2,ano2;
            sscanf(lista[i].dt_recebimento, "%d-%d-%d", &ano, &mes, &dia);
            sscanf(lista[i].dt_resolvido, "%d-%d-%d", &ano2, &mes2, &dia2);

            int Data1 = DataParaDias(ano,mes,dia);
            int Data2 = DataParaDias(ano2,mes2,dia2);
            int Diferenca = Data2 - Data1;

            return Diferenca;
        }
    }
    return -1;  // Processo não encontrado
}

float cumprimentoMeta (Processo* lista, int qtd){
    if (!lista || qtd <= 0) return 0.0f;

    const int anoReferencia = 2025; // altere se quiser parametrizar/usar outro ano

    int cnm1 = 0;
    int julgadom1 = 0;
    int desm1 = 0;
    int susm1 = 0;

    for (int i = 0; i < qtd; i++) {
        if (lista[i].dt_recebimento[0] == '\0') continue; // pula vazios

        int ano = -1, mes = -1, dia = -1;
        // Tenta formatos comuns: "YYYY-MM-DD" ou "DD/MM/YYYY"
        if (sscanf(lista[i].dt_recebimento, "%d-%d-%d", &ano, &mes, &dia) != 3) {
            // tenta dd/mm/yyyy
            if (sscanf(lista[i].dt_recebimento, "%d/%d/%d", &dia, &mes, &ano) != 3) {
                // não conseguiu parsear -> ignora este registro
                continue;
            }
        }
        if (ano != anoReferencia) continue;

        // Conta valores usando os campos inteiros já presentes na struct
        // Assumimos que cnm1, julgadom1, desm1, susm1 já estão preenchidos como 0/1 ou valores numéricos por processo.
        // Se o CSV possui 0/1 por processo, soma direta funciona.
        cnm1 += lista[i].cnm1;
        julgadom1 += lista[i].julgadom1;
        desm1 += lista[i].desm1;
        susm1 += lista[i].susm1;
    }

    int denominador = cnm1 + desm1 - susm1;
    if (denominador <= 0) {
        // Evita divisão por zero e casos estranhos; retorna 0.0f (pode ajustar se preferir NaN ou 100%)
        return 0.0f;
    }

    float cumprimento = ((float)julgadom1 / (float)denominador) * 100.0f;
    return cumprimento;
} 

void processosJulgados(Processo *lista, int qtd, const char *arquivo){
FILE *filtrado = fopen(arquivo, "w");
    if (!filtrado) {
        perror("Erro ao abrir arquivo de saída");
        return;
    }

    // Cabeçalho
    fprintf(filtrado,
            "id_processo;numero_sigilo;sigla_grau;procedimento;ramo_justica;"
            "sigla_tribunal;id_tribunal;recurso;id_ultimo_oj;dt_recebimento;"
            "id_ultima_classe;flag_violencia_domestica;flag_feminicidio;"
            "flag_ambiental;flag_quilombolas;flag_indigenas;flag_infancia;"
            "decisao;dt_resolvido;cnm1;primeirasentm1;baixm1;decm1;mpum1;"
            "julgadom1;desm1;susm1\n");

    int contador = 0;

    for (int i = 0; i < qtd; i++) {
        
        if (lista[i].julgadom1 > 0) {
            fprintf(filtrado, 
                "%d;%s;%s;%s;%s;%s;%d;%d;%d;%s;%d;%d;%d;%d;%d;%d;%d;"
                "%s;%s;%d;%d;%d;%d;%d;%d;%d;%d\n",

                lista[i].id_processo,
                lista[i].numero_sigilo,
                lista[i].sigla_grau,
                lista[i].procedimento,
                lista[i].ramo_justica,
                lista[i].sigla_tribunal,
                lista[i].id_tribunal,
                lista[i].recurso,
                lista[i].id_ultimo_oj,
                lista[i].dt_recebimento,
                lista[i].id_ultima_classe,
                lista[i].flag_violencia_domestica,
                lista[i].flag_feminicidio,
                lista[i].flag_ambiental,
                lista[i].flag_quilombolas,
                lista[i].flag_indigenas,
                lista[i].flag_infancia,
                lista[i].decisao,
                lista[i].dt_resolvido,
                lista[i].cnm1,
                lista[i].primeirasentm1,
                lista[i].baixm1,
                lista[i].decm1,
                lista[i].mpum1,
                lista[i].julgadom1,
                lista[i].desm1,
                lista[i].susm1);
                    
            contador++;
        }
    }

    fclose(filtrado);
    printf("Arquivo '%s' gerado com %d processos julgados (Meta 1).\n", arquivo, contador);
}