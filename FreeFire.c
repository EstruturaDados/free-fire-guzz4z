#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> // Para tolower

// Definição da estrutura do Componente
#define MAX_COMPONENTES 20
#define TAM_NOME 30
#define TAM_TIPO 20

/**
 * @brief Estrutura que representa um componente da Torre de Resgate.
 * Contém o nome (chave principal para busca), o tipo e o nível de prioridade
 * (1 a 10).
 */
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int prioridade;
} Componente;

// Protótipos das funções
void lerStringSeguro(char *str, int max_len);
void mostrarComponentes(Componente array[], int n);
void entradaDados(Componente componentes[], int *n);
void copiarArray(Componente destino[], const Componente origem[], int n);

// Funções de Ordenação (retornam o número de comparações)
long bubbleSortNome(Componente array[], int n);
long insertionSortTipo(Componente array[], int n);
long selectionSortPrioridade(Componente array[], int n);

// Função de Busca
int buscaBinariaPorNome(Componente array[], int n, const char *chave, long *comparacoes);

// Função auxiliar de Medição de Desempenho
void medirTempoEExecutar(Componente array[], int n, long (*algoritmo)(Componente[], int), const char *nomeAlgoritmo);

// Função principal do menu
void menuPrincipal();

/**
 * @brief Implementa a leitura de strings de forma segura usando fgets e remove o '\n'.
 * @param str Ponteiro para a string de destino.
 * @param max_len Tamanho máximo da string.
 */
void lerStringSeguro(char *str, int max_len) {
    if (fgets(str, max_len, stdin) != NULL) {
        // Encontra a primeira ocorrência de '\n' e substitui por '\0'
        size_t len = strlen(str);
        if (len > 0 && str[len - 1] == '\n') {
            str[len - 1] = '\0';
        } else {
            // Se não encontrou '\n', significa que o buffer é pequeno
            // Limpa o restante do buffer de entrada
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    } else {
        str[0] = '\0'; // Caso de erro na leitura
    }
}

/**
 * @brief Exibe a lista de componentes formatada.
 * @param array Array de componentes a ser exibido.
 * @param n Número de componentes no array.
 */
void mostrarComponentes(Componente array[], int n) {
    if (n == 0) {
        printf("\n[!] Lista de Componentes Vazia.\n");
        return;
    }
    printf("\n*** Componentes da Torre de Resgate (%d itens) ***\n", n);
    printf("------------------------------------------------------------------\n");
    printf("| %-25s | %-15s | %-10s |\n", "Nome", "Tipo", "Prioridade");
    printf("------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("| %-25s | %-15s | %-10d |\n", array[i].nome, array[i].tipo, array[i].prioridade);
    }
    printf("------------------------------------------------------------------\n");
}

/**
 * @brief Permite ao jogador cadastrar os componentes (entrada de dados).
 * @param componentes Array para armazenar os componentes.
 * @param n Ponteiro para o número atual de componentes.
 */
void entradaDados(Componente componentes[], int *n) {
    int i = *n;
    char resposta[10];
    printf("\n--- Cadastro de Componentes ---\n");

    while (i < MAX_COMPONENTES) {
        printf("\nComponente #%d (Limite: %d):\n", i + 1, MAX_COMPONENTES);

        // 1. Nome do componente
        printf("Nome (ex: 'Chip Central'): ");
        lerStringSeguro(componentes[i].nome, TAM_NOME);
        if (strlen(componentes[i].nome) == 0) {
             printf("[!] Nome vazio, abortando cadastro deste item.\n");
             break;
        }

        // 2. Tipo do componente
        printf("Tipo (ex: 'controle', 'suporte', 'propulsao'): ");
        lerStringSeguro(componentes[i].tipo, TAM_TIPO);
        if (strlen(componentes[i].tipo) == 0) {
             printf("[!] Tipo vazio, abortando cadastro deste item.\n");
             break;
        }

        // 3. Prioridade (1 a 10)
        printf("Prioridade (1 a 10): ");
        if (scanf("%d", &componentes[i].prioridade) != 1 || componentes[i].prioridade < 1 || componentes[i].prioridade > 10) {
            printf("[!] Prioridade invalida. Deve ser um numero entre 1 e 10.\n");
            // Limpa o buffer de entrada
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue; // Tenta o item novamente
        }
        // Limpa o buffer de entrada após ler o inteiro
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        i++;
        *n = i;

        if (i < MAX_COMPONENTES) {
            printf("Deseja cadastrar outro componente? (s/n): ");
            lerStringSeguro(resposta, 10);
            if (tolower(resposta[0]) != 's') {
                break;
            }
        }
    }
    printf("\n[✅] Cadastro concluido. Total de %d componentes.\n", *n);
}

/**
 * @brief Copia o conteúdo de um array de Componente para outro.
 * @param destino Array de destino.
 * @param origem Array de origem.
 * @param n Número de elementos a serem copiados.
 */
void copiarArray(Componente destino[], const Componente origem[], int n) {
    for (int i = 0; i < n; i++) {
        destino[i] = origem[i];
    }
}

// ----------------------------------------------------
// IMPLEMENTAÇÃO DOS ALGORITMOS DE ORDENAÇÃO
// ----------------------------------------------------

/**
 * @brief Bubble Sort para ordenar por Nome (string).
 * @param array Array de componentes.
 * @param n Número de componentes.
 * @return long Número de comparações realizadas.
 */
long bubbleSortNome(Componente array[], int n) {
    long comparacoes = 0;
    Componente temp;
    int trocou;

    for (int i = 0; i < n - 1; i++) {
        trocou = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            comparacoes++;
            // Compara as strings de nome
            if (strcmp(array[j].nome, array[j+1].nome) > 0) {
                // Troca os componentes
                temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
                trocou = 1;
            }
        }
        // Se nenhuma troca ocorreu na passagem, o array está ordenado
        if (trocou == 0) break;
    }
    return comparacoes;
}

/**
 * @brief Insertion Sort para ordenar por Tipo (string).
 * @param array Array de componentes.
 * @param n Número de componentes.
 * @return long Número de comparações realizadas.
 */
long insertionSortTipo(Componente array[], int n) {
    long comparacoes = 0;
    int i, j;
    Componente chave;

    for (i = 1; i < n; i++) {
        chave = array[i];
        j = i - 1;

        // Compara e move elementos maiores que a chave para a direita
        while (j >= 0) {
            comparacoes++;
            if (strcmp(array[j].tipo, chave.tipo) > 0) {
                array[j+1] = array[j];
                j = j - 1;
            } else {
                break; // A chave está na posição correta
            }
        }
        array[j+1] = chave;
    }
    return comparacoes;
}

/**
 * @brief Selection Sort para ordenar por Prioridade (int).
 * @param array Array de componentes.
 * @param n Número de componentes.
 * @return long Número de comparações realizadas.
 */
long selectionSortPrioridade(Componente array[], int n) {
    long comparacoes = 0;
    int i, j, indice_min;
    Componente temp;

    for (i = 0; i < n - 1; i++) {
        indice_min = i;
        for (j = i + 1; j < n; j++) {
            comparacoes++;
            // Compara os valores inteiros de prioridade
            if (array[j].prioridade < array[indice_min].prioridade) {
                indice_min = j;
            }
        }
        // Troca o elemento mínimo encontrado com o primeiro elemento não ordenado
        if (indice_min != i) {
            temp = array[i];
            array[i] = array[indice_min];
            array[indice_min] = temp;
        }
    }
    return comparacoes;
}

// ----------------------------------------------------
// IMPLEMENTAÇÃO DA BUSCA BINÁRIA
// ----------------------------------------------------

/**
 * @brief Busca Binária para localizar um componente pelo Nome (string).
 * Aplicável apenas em array ordenado por nome.
 * @param array Array de componentes (DEVE ESTAR ORDENADO POR NOME).
 * @param n Número de componentes.
 * @param chave String do nome a ser buscado.
 * @param comparacoes Ponteiro para armazenar o número de comparações.
 * @return int Índice do componente encontrado, ou -1 se não encontrado.
 */
int buscaBinariaPorNome(Componente array[], int n, const char *chave, long *comparacoes) {
    int inicio = 0;
    int fim = n - 1;
    *comparacoes = 0;
    int resultado_comparacao;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        (*comparacoes)++;
        
        // Compara a chave com o nome do componente no meio
        resultado_comparacao = strcmp(chave, array[meio].nome);

        if (resultado_comparacao == 0) {
            return meio; // Componente encontrado
        } else if (resultado_comparacao < 0) {
            fim = meio - 1; // Busca na metade esquerda
        } else {
            inicio = meio + 1; // Busca na metade direita
        }
    }
    return -1; // Componente não encontrado
}

// ----------------------------------------------------
// FUNÇÃO DE MEDIÇÃO DE DESEMPENHO E EXECUÇÃO
// ----------------------------------------------------

/**
 * @brief Função auxiliar para medir o tempo e as comparações de um algoritmo de ordenação.
 * @param array Array de componentes a ser ordenado.
 * @param n Número de componentes.
 * @param algoritmo Ponteiro para a função do algoritmo de ordenação (ex: bubbleSortNome).
 * @param nomeAlgoritmo Nome amigável do algoritmo para exibição.
 */
void medirTempoEExecutar(Componente array[], int n, long (*algoritmo)(Componente[], int), const char *nomeAlgoritmo) {
    if (n == 0) {
        printf("\n[!] Lista de componentes vazia. Cadastre os itens primeiro.\n");
        return;
    }

    // Marca o tempo inicial
    clock_t inicio = clock();

    // Executa o algoritmo e conta as comparações
    long comparacoes = algoritmo(array, n);

    // Marca o tempo final
    clock_t fim = clock();

    // Calcula o tempo de execução em segundos
    double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\n*** Relatorio de Desempenho - %s ***\n", nomeAlgoritmo);
    printf("------------------------------------------------------------------\n");
    printf("| Metrica                | Valor                                   |\n");
    printf("------------------------------------------------------------------\n");
    printf("| Algoritmo de Ordenacao | %s                                     |\n", nomeAlgoritmo);
    printf("| Numero de Comparações  | %ld                                     |\n", comparacoes);
    printf("| Tempo de Execucao (s)  | %f                                |\n", tempo_execucao);
    printf("------------------------------------------------------------------\n");

    printf("\n[✅] Apos ordenacao:\n");
    mostrarComponentes(array, n);
}

// ----------------------------------------------------
// FUNÇÃO PRINCIPAL
// ----------------------------------------------------

void menuPrincipal() {
    Componente componentes_originais[MAX_COMPONENTES];
    Componente componentes_operacao[MAX_COMPONENTES];
    int n = 0; // Número atual de componentes
    int ordenado_por_nome = 0; // Flag para busca binária

    int opcao;
    char chave_busca[TAM_NOME];
    int indice_encontrado;
    long comp_busca;

    printf("========================================================\n");
    printf("        🚀 MODULO AVANÇADO DE ORGANIZAÇÃO (SAFE ZONE)     \n");
    printf("========================================================\n");

    do {
        printf("\n--- Escolha sua Estrategia ---\n");
        printf("1. 📝 Cadastrar/Revisar Componentes (%d/%d)\n", n, MAX_COMPONENTES);
        printf("2. 🔍 Buscar Componente-Chave (Apos ordenacao por nome)\n");
        printf("3. ➡️ Ordenar por Nome (Bubble Sort) e Analisar\n");
        printf("4. ➡️ Ordenar por Tipo (Insertion Sort) e Analisar\n");
        printf("5. ➡️ Ordenar por Prioridade (Selection Sort) e Analisar\n");
        printf("0. 🚪 SAIR e Iniciar a Montagem da Torre\n");
        printf("----------------------------\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("[!] Entrada invalida. Tente novamente.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF); // Limpa o buffer
            continue;
        }
        // Limpa o buffer de entrada após ler o inteiro
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        // Se n > 0, o array de componentes originais existe
        if (opcao >= 2 && opcao <= 5 && n == 0) {
             printf("\n[!] E preciso cadastrar componentes (Opcao 1) antes de ordenar ou buscar.\n");
             continue;
        }

        switch (opcao) {
            case 1:
                entradaDados(componentes_originais, &n);
                // Reseta a flag de ordenação ao mudar os dados
                ordenado_por_nome = 0;
                break;
            
            case 2: // Busca Binária
                if (!ordenado_por_nome) {
                    printf("\n[!] ERRO: A Busca Binaria so pode ser aplicada apos a ordenacao por Nome (Opcao 3).\n");
                } else {
                    printf("\n--- Busca Binaria por Nome ---\n");
                    printf("Qual o nome do componente-chave a buscar? (Ex: 'Chip Central'): ");
                    lerStringSeguro(chave_busca, TAM_NOME);

                    indice_encontrado = buscaBinariaPorNome(componentes_operacao, n, chave_busca, &comp_busca);

                    if (indice_encontrado != -1) {
                        printf("\n[✅] CHAVE ENCONTRADA! O componente '%s' foi localizado no indice %d.\n", chave_busca, indice_encontrado);
                        printf("   Tipo: %s | Prioridade: %d\n", componentes_operacao[indice_encontrado].tipo, componentes_operacao[indice_encontrado].prioridade);
                        printf("   Comparacoes na Busca Binaria: %ld\n", comp_busca);
                    } else {
                        printf("\n[❌] COMPONENTE NAO ENCONTRADO! '%s' nao esta na lista.\n", chave_busca);
                        printf("   Comparacoes na Busca Binaria: %ld\n", comp_busca);
                    }
                }
                break;

            case 3: // Bubble Sort por Nome
                copiarArray(componentes_operacao, componentes_originais, n);
                medirTempoEExecutar(componentes_operacao, n, bubbleSortNome, "Bubble Sort (Nome)");
                // Seta a flag de ordenação por nome para permitir a busca
                ordenado_por_nome = 1;
                break;

            case 4: // Insertion Sort por Tipo
                copiarArray(componentes_operacao, componentes_originais, n);
                medirTempoEExecutar(componentes_operacao, n, insertionSortTipo, "Insertion Sort (Tipo)");
                ordenado_por_nome = 0;
                break;

            case 5: // Selection Sort por Prioridade
                copiarArray(componentes_operacao, componentes_originais, n);
                medirTempoEExecutar(componentes_operacao, n, selectionSortPrioridade, "Selection Sort (Prioridade)");
                ordenado_por_nome = 0;
                break;

            case 0:
                printf("\n[👋] Encerrando o modulo. Iniciando a montagem da Torre de Resgate...\n");
                break;

            default:
                printf("\n[!] Opcao invalida. Por favor, escolha uma opcao do menu.\n");
                break;
        }
    } while (opcao != 0);
}

int main() {
    menuPrincipal();
    return 0;
}