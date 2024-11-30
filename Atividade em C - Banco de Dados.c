#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CARACTERES 520
#define MAX_TITULO 100
#define ARQUIVO_NOTAS "notas.txt"  // Nome do arquivo onde as notas ser�o armazenadas

// Fun��o para adicionar uma nova nota
void adicionarNota() {
    FILE *arquivo = fopen(ARQUIVO_NOTAS, "a");  // Abrir o arquivo em modo append (acrescenta ao final)
    
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char data[11], titulo[MAX_TITULO + 1], descricao[MAX_CARACTERES + 1];

    printf("Qual a data de hoje? (DD/MM/AAAA): ");
    scanf("%s", data);

    printf("Qual o t�tulo de hoje? (at� 100 caracteres): ");
    getchar();  // Para consumir o \n remanescente
    fgets(titulo, sizeof(titulo), stdin);
    titulo[strcspn(titulo, "\n")] = 0; // Remove o \n extra da string

    printf("Como foi seu dia? (at� 520 caracteres): ");
    fgets(descricao, sizeof(descricao), stdin);
    descricao[strcspn(descricao, "\n")] = 0; // Remove o \n extra da string

    // Escrever a nota no arquivo
    fprintf(arquivo, "%s|%s|%s\n", data, titulo, descricao);
    printf("Nota adicionada com sucesso!\n\n");

    fclose(arquivo);  // Fechar o arquivo
}

// Fun��o para listar as notas armazenadas no arquivo
void listarNotas() {
    FILE *arquivo = fopen(ARQUIVO_NOTAS, "r");  // Abrir o arquivo em modo leitura
    
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[650];  // Tamanho para armazenar linha (data + t�tulo + descri��o)
    printf("===== Notas Armazenadas =====\n");

    while (fgets(linha, sizeof(linha), arquivo)) {
        char *data = strtok(linha, "|");
        char *titulo = strtok(NULL, "|");
        char *descricao = strtok(NULL, "|");

        printf("Data: %s\n", data);
        printf("T�tulo: %s\n", titulo);
        printf("Descri��o: %s\n", descricao);
        printf("=============================\n");
    }

    fclose(arquivo);  // Fechar o arquivo
}

// Fun��o para excluir uma nota pelo t�tulo ou data
void excluirNota() {
    FILE *arquivo = fopen(ARQUIVO_NOTAS, "r");  // Abrir o arquivo em modo leitura
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    FILE *tempArquivo = fopen("temp.txt", "w");  // Arquivo tempor�rio para armazenar as notas que n�o ser�o exclu�das
    if (tempArquivo == NULL) {
        printf("Erro ao criar o arquivo tempor�rio.\n");
        fclose(arquivo);
        return;
    }

    char linha[650], dataExclusao[11];
    int encontrado = 0;

    // Pergunta ao usu�rio a data da nota a ser exclu�da
    printf("Digite a data da nota que deseja excluir (DD/MM/AAAA): ");
    scanf("%s", dataExclusao);

    // Percorre todas as notas no arquivo
    while (fgets(linha, sizeof(linha), arquivo)) {
        char *data = strtok(linha, "|");

        if (strcmp(data, dataExclusao) != 0) {
            // Se a data da nota n�o for igual � data de exclus�o, escreve a nota no arquivo tempor�rio
            fputs(linha, tempArquivo);
        } else {
            encontrado = 1;  // Se encontrar a nota, n�o escreve no arquivo tempor�rio
        }
    }

    fclose(arquivo);
    fclose(tempArquivo);

    if (encontrado) {
        // Substitui o arquivo original pelo arquivo tempor�rio
        remove(ARQUIVO_NOTAS);
        rename("temp.txt", ARQUIVO_NOTAS);
        printf("Nota exclu�da com sucesso!\n");
    } else {
        printf("Nota n�o encontrada para a data fornecida.\n");
        remove("temp.txt");  // Se n�o encontrar, remove o arquivo tempor�rio
    }
}

int main() {
    int escolha;

    // Menu de op��es
    do {
        printf("===== MENU =====\n");
        printf("1. Nova Nota\n");
        printf("2. Listar Notas\n");
        printf("3. Excluir Nota\n");
        printf("4. Sair\n");
        printf("================\n");
        printf("Escolha uma op��o: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                adicionarNota();  // Adicionar nova nota
                break;
            case 2:
                listarNotas();  // Listar notas existentes
                break;
            case 3:
                excluirNota();  // Excluir nota
                break;
            case 4:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Op��o inv�lida! Tente novamente.\n");
        }
    } while (escolha != 4);  // O programa continua at� o usu�rio escolher a op��o 4 para sair

    return 0;
}

