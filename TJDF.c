#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TJDF.h"

#define N 3000000 // Numero base de processos no arquivo
#define TAM_LINHA 2048 // Numero maximo caracteres em cada linha

// Função auxiliar pra ler int (trata vazio como 0)
int lerInt(char *linha) {
    if (linha == NULL || strlen(linha) == 0 || linha[0] == '\0' || linha[0] == '\n') {
        return 0;
    }
    return atoi(linha); //converte uma string para um número inteiro
}

// Função auxiliar pra ler string (trata vazio como "")
void lerStr(char *cedula, char *linha, size_t tamanho) {
    if (linha == NULL || strlen(linha) == 0) {
        cedula[0] = '\0';  // String vazia
        return;
    }
    strncpy(cedula, linha, tamanho - 1);
    cedula[tamanho - 1] = '\0';  // Garante terminação segura
}

int compararData(const char* d1, const char* d2) {
    return strcmp(d1, d2);
}

Processo* carregarProcessos(const char* arquivo, int* qtd) { //Função para Carregar o arquivo.csv
    FILE *fp = fopen(arquivo, "r");
    if (!fp) {
        printf("Arquivo inexistente!\n");
        *qtd = 0;
        return NULL;
    }

    char linha[TAM_LINHA];
    fgets(linha, TAM_LINHA, fp);  // Pula o cabeçalho

    Processo *lista = malloc(N * sizeof(Processo)); //Alocamento dinamico de memoria 
    if (!lista) {
        printf("Erro de alocação!\n");
        fclose(fp);
        *qtd = 0;
        return NULL;
    }

    int contador = 0; //Contador de linhas

    while (fgets(linha, TAM_LINHA, fp)) { //Laço para ler todas as linhas
        if (contador >= N) break;

        linha[strcspn(linha, "\n")] = '\0';//preenche os "espacos" com vazio

        Processo *planilha = &lista[contador];//vetor com o tamanho de acordo ao alocamento de memoria

        char *cedula = linha; // ponteiro que aponta para o começo da linha
        int campo_index = 0; //Colunas do  arquivo csv

        while (campo_index < 27) {
            char *linha = NULL; //Guarda o valor da cedula atual
            int pos = strcspn(cedula, ";");// Encontra onde termina a cedula, retornando a posição
            
            if (pos == 0) {
                linha = "";//preenche a cedula com vazio 
                if (*cedula == '\0') break; //preenche cédulas vazias com /0
                cedula++;
            } else { //se tiver conteudo
                linha = malloc(pos + 1);
                if (linha) {
                    strncpy(linha, cedula, pos);// copia os dados do campo
                    linha[pos] = '\0';// adiciona o fim de string
                } else {
                    linha = "";
                }
                cedula += pos + 1;// pula o campo atual e o `;`
            }

            switch (campo_index) { //switch para preencher cada campo e evitar erros com campos vazios
                case 0: lerStr(planilha->id_processo, linha, sizeof(planilha->id_processo)); break;
                case 1: lerStr(planilha->numero_sigilo, linha, sizeof(planilha->numero_sigilo)); break;
                case 2: lerStr(planilha->sigla_grau, linha, sizeof(planilha->sigla_grau)); break;
                case 3: lerStr(planilha->procedimento, linha, sizeof(planilha->procedimento)); break;
                case 4: lerStr(planilha->ramo_justica, linha, sizeof(planilha->ramo_justica)); break;
                case 5: lerStr(planilha->sigla_tribunal, linha, sizeof(planilha->sigla_tribunal)); break;
                case 6: planilha->id_tribunal = lerInt(linha); break;
                case 7: planilha->recurso = lerInt(linha); break;
                case 8: lerStr(planilha->id_ultimo_oj, linha, sizeof(planilha->id_ultimo_oj)); break;
                case 9: lerStr(planilha->dt_recebimento, linha, sizeof(planilha->dt_recebimento)); break;
                case 10: lerStr(planilha->id_ultima_classe, linha, sizeof(planilha->id_ultima_classe)); break;
                case 11: planilha->flag_violencia_domestica = lerInt(linha); break;
                case 12: planilha->flag_feminicidio = lerInt(linha); break;
                case 13: planilha->flag_ambiental = lerInt(linha); break;
                case 14: planilha->flag_quilombolas = lerInt(linha); break;
                case 15: planilha->flag_indigenas = lerInt(linha); break;
                case 16: planilha->flag_infancia = lerInt(linha); break;
                case 17: lerStr(planilha->decisao, linha, sizeof(planilha->decisao)); break;
                case 18: lerStr(planilha->dt_resolvido, linha, sizeof(planilha->dt_resolvido)); break;
                case 19: planilha->cnm1 = lerInt(linha); break;
                case 20: planilha->primeirasentm1 = lerInt(linha); break;
                case 21: planilha->baixm1 = lerInt(linha); break;
                case 22: planilha->decm1 = lerInt(linha); break;
                case 23: planilha->mpum1 = lerInt(linha); break;
                case 24: planilha->julgadom1 = lerInt(linha); break;
                case 25: planilha->desm1 = lerInt(linha); break;
                case 26: planilha->susm1 = lerInt(linha); break;
            }

            if (pos != 0 && linha && linha != "") { //liberacao de memoria para campos não vazios
                free(linha);
            }

            campo_index++;//Contador para o proximo campo
        }

        contador++;//Contador de linhas
    }

    fclose(fp);
    *qtd = contador;
    
    return lista;
}

//Função para imprimir e retornar a quantidade de processos
int contarProcessos(const Processo* lista, int qtd) { 
    printf("- Carregados %d processos com sucesso.\n", qtd);
    return qtd;
}

//Função que busca o ultimo Orgão julgador a partir de um ID
const char* buscarUltimoOJ(const Processo* lista, int qtd, const char* id_processo) {
    for (int i = 0; i < qtd; i++) { 
        if (strcmp(lista[i].id_processo, id_processo) == 0) {//Procura o id no arquivo
            return lista[i].id_ultimo_oj;
        }   
    }
    return NULL;//Retorna NULL se não encontrar
}

//Encontra o processo mais antigo do csv
const char* processoMaisAntigo(const Processo* lista, int qtd) {
    if (qtd <= 0) return NULL;//Retorna NULL se não houver linhas

    int idxMaisAntigo = -1;
    for (int i = 0; i < qtd; i++) {
        if (strlen(lista[i].dt_recebimento) == 0) continue;  // Pula datas vazias

        //Comparação entre as datas
        if (idxMaisAntigo == -1 || compararData(lista[i].dt_recebimento, lista[idxMaisAntigo].dt_recebimento) < 0) { 
            idxMaisAntigo = i; //Atualiza o indice do processo mais antigo
        }
    }

    if (idxMaisAntigo == -1) return NULL;  // Nenhuma data válida
    return lista[idxMaisAntigo].dt_recebimento; 
}

//Contador de flags para Violencia Domestica
int contarViolenciaDomestica(const Processo* lista, int qtd) {
    int contador = 0;
    for (int i = 0; i < qtd; i++) { 
        if (lista[i].flag_violencia_domestica == 1) contador++; 
    }
    return contador;
}

//Contador de flags para Feminicidio
int contarFeminicidio(const Processo* lista, int qtd) {
    int contador = 0;
    for (int i = 0; i < qtd; i++) {
        if (lista[i].flag_feminicidio == 1) contador++;
    }
    return contador;
}

//Contador de flags para Materia ambiental
int contarAmbiental(const Processo* lista, int qtd) {
    int contador = 0;
    for (int i = 0; i < qtd; i++) {
        if (lista[i].flag_ambiental == 1) contador++;
    }
    return contador;
}

//Contador de flags para Comunidades quilombolas
int contarQuilombolas(const Processo* lista, int qtd) {
    int contador = 0;
    for (int i = 0; i < qtd; i++) {
        if (lista[i].flag_quilombolas == 1) contador++;
    }
    return contador;
}

//Contador de flags para Povos indigenas
int contarIndigenas(const Processo* lista, int qtd) {
    int contador = 0;
    for (int i = 0; i < qtd; i++) {
        if (lista[i].flag_indigenas == 1) contador++;
    }
    return contador;
}

//Contador de flags para ECA
int contarInfanciaeJuventude(const Processo* lista, int qtd) {
    int contador = 0;
    for (int i = 0; i < qtd; i++) {
        if (lista[i].flag_infancia == 1) contador++;
    }
    return contador;
}

//Função para converter datas em dias, usada para fazer comparação do inveralo entre datas.
int DataParaDias (int Ano, int Mes, int dia ){
    
    int Meses[] = {31,28,31,30,31,30,31,31,30,31,30,31}; //Array com a quantidade de dias de cada mes
    
    //Verificação de anos bisextos, usando a regra de ser divisivel por 4 e não por 100, ou divisivel por 400
    int verificarBisexto = ( (Ano % 4 == 0 && Ano % 100 != 0) || (Ano % 400 == 0) ); 

    //Maximo de dias de um mês
    int MaxDias = Meses[Mes - 1];
    
    //Atualiza a quantidade de dias de fevereiro caso o ano seja Bisexto
    if (Mes == 2 && verificarBisexto) 
    {
        MaxDias = 29;
    }
    
    //Se a quantidade de dias for maior que a quantidade de dias do mês, retorna -1.
    if (dia > MaxDias) 
    {
        return -1;
    }

    int AnosCompletos = Ano - 1; //desconsidera o ano atual

    //Calculo para os dias bisextos
    int Diasbissextos = AnosCompletos / 4 - AnosCompletos / 100 + AnosCompletos / 400;
    int DiasAnos = AnosCompletos * 365 + Diasbissextos;//

    int DiasMes = 0;
    
    for (int i = 0; i < Mes - 1; i++){ //Quantidade de dias até o mes do ano atual
        DiasMes += Meses[i];
    } 
    // verifica se o ano atual é bisexto e adiciona um dia em fevereiro
    if (verificarBisexto && Mes > 2) DiasMes += 1;

    int total = DiasAnos + DiasMes + dia;
    return total;
}

//Calcula o intervalo de dias entre o recebimento e a resolução do processo
int diferencaDeDias(const Processo* lista, int qtd, const char* id_processo) {
    for (int i = 0; i < qtd; i++) {
        if (strcmp(lista[i].id_processo, id_processo) == 0) {
            int dia,mes,ano,dia2,mes2,ano2;

            //separa a data para ser usada a função de data para dias
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

float cumprimentoMeta (Processo* lista, int qtd) {
    if (!lista || qtd <= 0) return 0.0f; //retorna 0 caso o arquivo não exista ou não tenha conteudo

    const int anoReferencia = 2025; // ano de referencia para a meta

    long cnm1 = 0;
    long julgadom1 = 0;
    long desm1 = 0;
    long susm1 = 0;

    for (int i = 0; i < qtd; i++) {
        if (lista[i].dt_recebimento[0] == '\0') continue; // pula vazios

        //Filtro para o ano de 2025!!!!
        /*int ano = -1, mes = -1, dia = -1;//Iniciadas com -1 para a verificação do sscanf
        
        //só os processos com data de recebimento no ano de 2025 entram nos cálculos
        if (sscanf(lista[i].dt_recebimento, "%d-%d-%d", &ano, &mes, &dia) != 3) 
        { 
                continue;
        }
        if (ano != anoReferencia) continue;
        */
        
        cnm1 += lista[i].cnm1;
        julgadom1 += lista[i].julgadom1;
        desm1 += lista[i].desm1;
        susm1 += lista[i].susm1;
    }
    
    int denominador = cnm1 + desm1 - susm1; //calculo de denominador 
    
    //Evitar erros com divisão por 0
    if (denominador <= 0) {
        return 0.0f;
    }

    float cumprimento = ((float)julgadom1 / (float)denominador) * 100.0f; //calculo da meta
    return cumprimento;
} 

//Gera o arquivo csv com os processos ja julgados na Meta01
void processosJulgados(Processo *lista, int qtd, const char *arquivo){
FILE *filtrado = fopen(arquivo, "w"); 
    if (!filtrado) { //Retorna uma mensagem caso o arquivo não seja aberto
        perror("Erro ao abrir arquivo de saída");
        return;
    }

    // Cabeçalho para escrita
    fprintf(filtrado,
            "id_processo;numero_sigilo;sigla_grau;procedimento;ramo_justica;"
            "sigla_tribunal;id_tribunal;recurso;id_ultimo_oj;dt_recebimento;"
            "id_ultima_classe;flag_violencia_domestica;flag_feminicidio;"
            "flag_ambiental;flag_quilombolas;flag_indigenas;flag_infancia;"
            "decisao;dt_resolvido;cnm1;primeirasentm1;baixm1;decm1;mpum1;"
            "julgadom1;desm1;susm1\n");

    int contador = 0; //Contador de processos filtrados

    for (int i = 0; i < qtd; i++) { //Laço para preenchimento dos campos
        
        if (lista[i].julgadom1 > 0) {
            fprintf(filtrado, 
                "%s;%s;%s;%s;%s;%s;%d;%d;%d;%s;%d;%d;%d;%d;%d;%d;%d;"
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

    fclose(filtrado); //fecha o arquivo depois de escrito

    //Exibe a quantidade de processos
    printf("Arquivo '%s' gerado com %d processos julgados (Meta 1).\n", arquivo, contador);
}