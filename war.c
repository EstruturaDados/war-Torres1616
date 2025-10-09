#include <stdio.h>
#include <string.h>

// Estrutura que representa um país
struct territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

int main() {
    struct territorio pais[5]; // Vetor para armazenar 5 países

    for (int i = 0; i < 5; i++) {
        printf("\n==================================\n");
        printf("       Anexar Paises\n");
        printf("==================================\n");

        // Lê o nome do país
        printf("Digite o nome do pais %d: ", i + 1);
        fgets(pais[i].nome, sizeof(pais[i].nome), stdin);
        pais[i].nome[strcspn(pais[i].nome, "\n")] = '\0'; // Remove o Enter

        // Lê a cor do país
        printf("Digite a cor do pais %d: ", i + 1);
        fgets(pais[i].cor, sizeof(pais[i].cor), stdin);
        pais[i].cor[strcspn(pais[i].cor, "\n")] = '\0';

        // Lê a quantidade de tropas
        printf("Digite a quantidade de tropas do pais %d: ", i + 1);
        scanf("%d", &pais[i].tropas);
        getchar(); // Limpa o Enter deixado pelo scanf
    }

    // Mostra os países cadastrados
    printf("\n==================================\n");
    printf("       Seus Paises\n");
    printf("==================================\n");

    for (int i = 0; i < 5; i++) {
        printf("\nPais %d\n", i + 1);
        printf("Nome: %s\n", pais[i].nome);
        printf("Cor: %s\n", pais[i].cor);
        printf("Tropas: %d\n", pais[i].tropas);
    }

    return 0;
}