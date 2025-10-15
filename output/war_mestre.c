#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ==================== CONFIGURAÇÕES ====================
#define MAX_TERRITORIOS 6
#define TAM_MISSAO 100

// ==================== ESTRUTURAS ====================
typedef struct {
    char nome[20];
    char cor[15];
    int tropas;
    int jogador_dono;
} Territorio;

typedef struct {
    char nome[20];
    char *missao;
    int venceu;
} Jogador;

// ==================== VARIÁVEIS GLOBAIS ====================
char* missoes[] = {
    "Conquistar 3 territorios seguidos",
    "Eliminar tropas vermelhas inimigas", 
    "Conquistar 2 territorios com +5 tropas",
    "Ter 10 tropas no total",
    "Controlar todos territorios azuis"
};
int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

// ==================== FUNÇÕES OTIMIZADAS ====================

// Sistema de dados melhorado
int rolarDado() {
    return rand() % 6 + 1;
}

void ordenarDados(int dados[], int quantidade) {
    for(int i = 0; i < quantidade-1; i++) {
        for(int j = i+1; j < quantidade; j++) {
            if(dados[i] < dados[j]) {
                int temp = dados[i];
                dados[i] = dados[j];
                dados[j] = temp;
            }
        }
    }
}

// Combate real como no War original
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n⚔ %s (%d tropas) ataca %s (%d tropas)\n", 
           atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);
    
    // Determinar número de dados
    int dados_atacante = (atacante->tropas >= 3) ? 3 : atacante->tropas;
    int dados_defensor = (defensor->tropas >= 2) ? 2 : defensor->tropas;
    
    if(dados_atacante > 1) dados_atacante--; // Ataque mínimo
    
    // Rolagem de dados
    int dados_a[3], dados_d[2];
    printf("Dados: ");
    
    for(int i = 0; i < dados_atacante; i++) {
        dados_a[i] = rolarDado();
        printf("A%d:%d ", i+1, dados_a[i]);
    }
    
    printf("vs ");
    
    for(int i = 0; i < dados_defensor; i++) {
        dados_d[i] = rolarDado();
        printf("D%d:%d ", i+1, dados_d[i]);
    }
    printf("\n");
    
    // Ordenar dados (maiores primeiro)
    ordenarDados(dados_a, dados_atacante);
    ordenarDados(dados_d, dados_defensor);
    
    // Comparação de dados
    int perdas_atacante = 0, perdas_defensor = 0;
    int comparacoes = (dados_atacante < dados_defensor) ? dados_atacante : dados_defensor;
    
    for(int i = 0; i < comparacoes; i++) {
        if(dados_a[i] > dados_d[i]) {
            perdas_defensor++;
            printf("Ataque vence: %d > %d\n", dados_a[i], dados_d[i]);
        } else {
            perdas_atacante++;
            printf("Defesa vence: %d <= %d\n", dados_a[i], dados_d[i]);
        }
    }
    
    // Aplicar perdas
    defensor->tropas -= perdas_defensor;
    atacante->tropas -= perdas_atacante;
    
    printf("Resultado: Atacante perde %d, Defensor perde %d\n", 
           perdas_atacante, perdas_defensor);
    
    // Conquista de território
    if(defensor->tropas <= 0) {
        printf("CONQUISTA! %s toma %s\n", atacante->nome, defensor->nome);
        defensor->jogador_dono = atacante->jogador_dono;
        defensor->tropas = dados_atacante; // Tropas que atacaram
    }
    
    // Verificar se atacante foi eliminado
    if(atacante->tropas <= 0) {
        atacante->jogador_dono = 0;
        printf("%s ficou sem tropas!\n", atacante->nome);
    }
}

// Inicialização do mapa
void inicializarMapa(Territorio mapa[], int tamanho) {
    char* nomes[] = {"Brasil", "Argentina", "Chile", "Peru", "Colombia", "Venezuela"};
    char* cores[] = {"vermelha", "azul", "verde", "amarela", "azul", "vermelha"};
    
    for(int i = 0; i < tamanho; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        strcpy(mapa[i].cor, cores[i]);
        mapa[i].tropas = 3;
        mapa[i].jogador_dono = 0;
    }
    
    mapa[0].jogador_dono = 1;
    mapa[0].tropas = 5;
    mapa[1].jogador_dono = 2;
    mapa[1].tropas = 5;
}

// Exibição do mapa
void exibirMapa(Territorio mapa[], int tamanho) {
    printf("\n--- MAPA ---\n");
    for(int i = 0; i < tamanho; i++) {
        printf("%d. %-10s | %-8s | %2d tropas | ", 
               i+1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
        
        if(mapa[i].jogador_dono == 0) printf("Neutro\n");
        else printf("Jogador %d\n", mapa[i].jogador_dono);
    }
}

// Atribuição de missões
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    if(destino == NULL) return;
    strcpy(destino, missoes[rand() % totalMissoes]);
}

// Verificação de missões otimizada
int verificarMissao(char* missao, Territorio mapa[], int tamanho, int jogador) {
    int jogador_id = jogador + 1;
    
    if(strstr(missao, "10 tropas")) {
        int total = 0;
        for(int i = 0; i < tamanho; i++) {
            if(mapa[i].jogador_dono == jogador_id) total += mapa[i].tropas;
        }
        if(total >= 10) {
            printf("MISSÃO: 10+ tropas alcançadas (%d)!\n", total);
            return 1;
        }
    }
    
    if(strstr(missao, "vermelhas")) {
        int vermelhos_inimigos = 0;
        for(int i = 0; i < tamanho; i++) {
            if(strcmp(mapa[i].cor, "vermelha") == 0 && 
               mapa[i].jogador_dono != jogador_id && mapa[i].jogador_dono != 0) {
                vermelhos_inimigos = 1;
                break;
            }
        }
        if(!vermelhos_inimigos) {
            printf("MISSÃO: Nenhum vermelho inimigo!\n");
            return 1;
        }
    }
    
    if(strstr(missao, "2 territorios com +5")) {
        int count = 0;
        for(int i = 0; i < tamanho; i++) {
            if(mapa[i].jogador_dono == jogador_id && mapa[i].tropas > 5) count++;
        }
        if(count >= 2) {
            printf("MISSÃO: 2+ territórios com 5+ tropas!\n");
            return 1;
        }
    }
    
    if(strstr(missao, "3 territorios seguidos")) {
        int consecutivos = 0;
        for(int i = 0; i < tamanho; i++) {
            if(mapa[i].jogador_dono == jogador_id) {
                consecutivos++;
                if(consecutivos >= 3) {
                    printf("MISSÃO: 3 territórios consecutivos!\n");
                    return 1;
                }
            } else {
                consecutivos = 0;
            }
        }
    }
    
    if(strstr(missao, "azuis")) {
        int todos_azuis = 1;
        for(int i = 0; i < tamanho; i++) {
            if(strcmp(mapa[i].cor, "azul") == 0 && mapa[i].jogador_dono != jogador_id) {
                todos_azuis = 0;
                break;
            }
        }
        if(todos_azuis) {
            printf("MISSÃO: Todos azuis conquistados!\n");
            return 1;
        }
    }
    
    return 0;
}

// Verificação de vitória
int verificarVitoria(Jogador jogadores[]) {
    for(int i = 0; i < 2; i++) {
        if(jogadores[i].venceu) return i + 1;
    }
    return 0;
}

// ==================== PROGRAMA PRINCIPAL ====================
int main() {
    srand(time(NULL));
    
    printf("=== GUERRA ESTRATÉGICA ===\n");
    
    // Inicialização
    Territorio mapa[MAX_TERRITORIOS];
    Jogador jogadores[2];
    
    strcpy(jogadores[0].nome, "Jogador 1");
    strcpy(jogadores[1].nome, "Jogador 2");
    jogadores[0].venceu = jogadores[1].venceu = 0;
    
    // Alocar missões
    jogadores[0].missao = malloc(TAM_MISSAO);
    jogadores[1].missao = malloc(TAM_MISSAO);
    
    if(!jogadores[0].missao || !jogadores[1].missao) {
        printf("Erro de memória!\n");
        return 1;
    }
    
    // Configuração inicial
    inicializarMapa(mapa, MAX_TERRITORIOS);
    atribuirMissao(jogadores[0].missao, missoes, totalMissoes);
    atribuirMissao(jogadores[1].missao, missoes, totalMissoes);
    
    printf("\nMissões:\n");
    printf("%s: %s\n", jogadores[0].nome, jogadores[0].missao);
    printf("%s: %s\n", jogadores[1].nome, jogadores[1].missao);
    printf("\nJogador 1: Brasil (5 tropas)\n");
    printf("Jogador 2: Argentina (5 tropas)\n");
    
    // Loop do jogo
    int jogador_atual = 0;
    int turno = 1;
    
    while(turno <= 15) {
        printf("\n--- Turno %d: %s ---\n", turno, jogadores[jogador_atual].nome);
        exibirMapa(mapa, MAX_TERRITORIOS);
        
        int origem, destino;
        printf("\nSua missão: %s\n", jogadores[jogador_atual].missao);
        printf("Atacar de (1-6): "); scanf("%d", &origem);
        printf("Para (1-6): "); scanf("%d", &destino);
        
        origem--; destino--;
        
        if(origem >= 0 && origem < MAX_TERRITORIOS && 
           destino >= 0 && destino < MAX_TERRITORIOS &&
           origem != destino &&
           mapa[origem].jogador_dono == jogador_atual + 1 &&
           mapa[destino].jogador_dono != jogador_atual + 1) {
            
            atacar(&mapa[origem], &mapa[destino]);
            
            // Verificar missão
            if(!jogadores[jogador_atual].venceu) {
                jogadores[jogador_atual].venceu = 
                    verificarMissao(jogadores[jogador_atual].missao, mapa, MAX_TERRITORIOS, jogador_atual);
            }
        } else {
            printf("Ataque inválido!\n");
        }
        
        // Verificar vitória
        int vencedor = verificarVitoria(jogadores);
        if(vencedor) {
            printf("\n*** %s VENCEU O JOGO! ***\n", jogadores[vencedor-1].nome);
            printf("Missão cumprida: %s\n", jogadores[vencedor-1].missao);
            break;
        }
        
        // Próximo jogador
        jogador_atual = !jogador_atual;
        turno++;
    }
    
    if(turno > 15) {
        printf("\n*** JOGO EMPATADO - Limite de turnos ***\n");
    }
    
    // Limpeza
    free(jogadores[0].missao);
    free(jogadores[1].missao);
    
    return 0;
}