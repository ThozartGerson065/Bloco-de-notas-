#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CARACTERES 520
#define MAX_TITULO 100
#define ARQUIVO_NOTAS "notas.txt"  // Nome do arquivo onde as notas serão armazenadas

// Função para adicionar uma nova nota
void adicionarNota() {
    FILE *arquivo = fopen(ARQUIVO_NOTAS, "a");  // Abrir o arquivo em modo append (acrescenta ao final)
    
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char data[11], titulo[MAX_TITULO + 1], descricao[MAX_CARACTERES + 1];

    printf("Qual a data de hoje? (DD/MM/AAAA): ");
    scanf("%s", data);

    printf("Qual o título de hoje? (até 100 caracteres): ");
    getchar();  // Para consumir o \n remanescente
    fgets(titulo, sizeof(titulo), stdin);
    titulo[strcspn(titulo, "\n")] = 0; // Remove o \n extra da string

    printf("Como foi seu dia? (até 520 caracteres): ");
    fgets(descricao, sizeof(descricao), stdin);
    descricao[strcspn(descricao, "\n")] = 0; // Remove o \n extra da string

    // Escrever a nota no arquivo
    fprintf(arquivo, "%s|%s|%s\n", data, titulo, descricao);
    printf("Nota adicionada com sucesso!\n\n");

    fclose(arquivo);  // Fechar o arquivo
}

// Função para listar as notas armazenadas no arquivo
void listarNotas() {
    FILE *arquivo = fopen(ARQUIVO_NOTAS, "r");  // Abrir o arquivo em modo leitura
    
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[650];  // Tamanho para armazenar linha (data + título + descrição)
    printf("===== Notas Armazenadas =====\n");

    while (fgets(linha, sizeof(linha), arquivo)) {
        char *data = strtok(linha, "|");
        char *titulo = strtok(NULL, "|");
        char *descricao = strtok(NULL, "|");

        printf("Data: %s\n", data);
        printf("Título: %s\n", titulo);
        printf("Descrição: %s\n", descricao);
        printf("=============================\n");
    }

    fclose(arquivo);  // Fechar o arquivo
}

// Função para excluir uma nota pelo título ou data
void excluirNota() {
    FILE *arquivo = fopen(ARQUIVO_NOTAS, "r");  // Abrir o arquivo em modo leitura
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    FILE *tempArquivo = fopen("temp.txt", "w");  // Arquivo temporário para armazenar as notas que não serão excluídas
    if (tempArquivo == NULL) {
        printf("Erro ao criar o arquivo temporário.\n");
        fclose(arquivo);
        return;
    }

    char linha[650], dataExclusao[11];
    int encontrado = 0;

    // Pergunta ao usuário a data da nota a ser excluída
    printf("Digite a data da nota que deseja excluir (DD/MM/AAAA): ");
    scanf("%s", dataExclusao);

    // Percorre todas as notas no arquivo
    while (fgets(linha, sizeof(linha), arquivo)) {
        char *data = strtok(linha, "|");

        if (strcmp(data, dataExclusao) != 0) {
            // Se a data da nota não for igual à data de exclusão, escreve a nota no arquivo temporário
            fputs(linha, tempArquivo);
        } else {
            encontrado = 1;  // Se encontrar a nota, não escreve no arquivo temporário
        }
    }

    fclose(arquivo);
    fclose(tempArquivo);

    if (encontrado) {
        // Substitui o arquivo original pelo arquivo temporário
        remove(ARQUIVO_NOTAS);
        rename("temp.txt", ARQUIVO_NOTAS);
        printf("Nota excluída com sucesso!\n");
    } else {
        printf("Nota não encontrada para a data fornecida.\n");
        remove("temp.txt");  // Se não encontrar, remove o arquivo temporário
    }
}

int main() {
    int escolha;

    // Menu de opções
    do {
        printf("===== MENU =====\n");
        printf("1. Nova Nota\n");
        printf("2. Listar Notas\n");
        printf("3. Excluir Nota\n");
        printf("4. Sair\n");
        printf("================\n");
        printf("Escolha uma opção: ");
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
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (escolha != 4);  // O programa continua até o usuário escolher a opção 4 para sair

    return 0;
}

