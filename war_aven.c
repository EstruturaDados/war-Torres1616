#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ======================================
// Estrutura principal do território
// ======================================
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// ======================================
// Função de ataque
// ======================================
void atacar(struct Territorio *atacante, struct Territorio *defensor) {
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\n Dado do atacante (%s): %d\n", atacante->nome, dadoAtacante);
    printf("Dado do defensor (%s): %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("\n O atacante venceu a batalha!\n");
        strcpy(defensor->cor, atacante->cor); // o defensor muda de dono
        defensor->tropas += atacante->tropas; // atacante pega todas as tropas do defensor
        atacante->tropas += 1; // atacante ganha reforço
    } else {
        printf("\n O defensor resistiu ao ataque!\n");
        if (atacante->tropas > 0) atacante->tropas--; // atacante perde 1 tropa
    }
}

// ======================================
// Função que verifica se há vencedor
// ======================================
int verificarVitoria(struct Territorio *mapa, int n) {
    char corVencedora[10];
    strcpy(corVencedora, mapa[0].cor);

    for (int i = 1; i < n; i++) {
        if (strcmp(mapa[i].cor, corVencedora) != 0) {
            return 0; // ainda há cores diferentes → o jogo continua
        }
    }

    printf("\n FIM DE JOGO! O vencedor é o exército %s!\n", corVencedora);
    return 1; // todas as cores são iguais → vitória!
}

// ======================================
// Função principal
// ======================================
int main() {
    srand(time(NULL));
    int n;

    printf("Quantos territórios deseja cadastrar? ");
    scanf("%d", &n);
    getchar(); // limpa buffer

    struct Territorio *mapa = (struct Territorio*) calloc(n, sizeof(struct Territorio));

    // Cadastro
    for (int i = 0; i < n; i++) {
        printf("\n==================================\n");
        printf("     Cadastro do Território %d\n", i + 1);
        printf("==================================\n");

        printf("Nome: ");
        fgets(mapa[i].nome, 30, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor (dono): ");
        fgets(mapa[i].cor, 10, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar();
    }

    // Loop principal (continua até um jogador dominar tudo)
    while (1) {
        printf("\n\n===== SITUAÇÃO ATUAL =====\n");
        for (int i = 0; i < n; i++) {
            printf("%d) %s | Cor: %s | Tropas: %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
        }

        // Verifica se alguém venceu
        if (verificarVitoria(mapa, n)) break;

        int a, d;
        printf("\nEscolha o número do território atacante: ");
        scanf("%d", &a);
        printf("Escolha o número do território defensor: ");
        scanf("%d", &d);

        // Valida entrada
        if (a < 1 || a > n || d < 1 || d > n) {
            printf("\n Escolha inválida!\n");
            continue;
        }
        if (a == d) {
            printf("\n Um território não pode atacar a si mesmo!\n");
            continue;
        }
        if (strcmp(mapa[a - 1].cor, mapa[d - 1].cor) == 0) {
            printf("\n Não é possível atacar um território da mesma cor!\n");
            continue;
        }

        atacar(&mapa[a - 1], &mapa[d - 1]);
    }

    // Libera memória
    free(mapa);
    printf("\nMemória liberada. Jogo encerrado!\n");
    return 0;
}
