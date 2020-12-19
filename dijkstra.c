#include <stdio.h>
#include <stdlib.h>

#define QTD_VERTICES 6
#define INFINITO 999
#define FALSE -1
#define TRUE 1

void leituraArquivo(FILE *FD, int matriz[][QTD_VERTICES])
{
    char *arq = "matriz_adjacencias.txt"; //Nome do arquivo que contém a matriz de adjacências dentro do diretório
    
    //Leitura do arquivo para inserir os dados na matriz
    printf("Realizando leitura do arquivo...\n");

    FD = fopen(arq, "r"); //Abre o arquivo para leitura da matriz de adjacencias.
    if (FD == NULL)
    {
        printf ("Erro na abertura do arquivo.\n");
        exit(1);
    }
    else //Realiza a leitura da matriz de adjacencias pelo arquivo "matriz_adjacencias.txt".
    {
        int apontador_arq = 16;
        char g = 0;
        fseek(FD, apontador_arq, SEEK_SET);

        for (int i = 0; i < QTD_VERTICES; i++)
        {
            for (int j = 0; j < QTD_VERTICES; j++)
            {
                fscanf(FD, "%c", &g);
                matriz[i][j] = g-48; //Armazena na memoria o valor lido do arquivo. "48" representa o inteiro 0 na tabela ASCII
                fseek(FD, 1, SEEK_CUR);
            }
            fseek(FD, 2, SEEK_CUR);
        }
        
    }
    fclose(FD);
    printf("Esta matriz esta' consistente!\n\n");
}

// Exibe o menor caminho
int exibeMenorCaminho(int verticeAnterior[], int anterior) 
{
    if (verticeAnterior[anterior] == FALSE) {
        return 1;
    }
    // Chama recursivamente enquanto o vértice não tiver um anterior
    exibeMenorCaminho(verticeAnterior, verticeAnterior[anterior]);
    printf("[%d]->", verticeAnterior[anterior]);
}

// Retorna o vértice que tem o menor caminho estimado
int verticeComMenorDistancia(int menorDistancia[], int visitados[])
{
    for (int i = 0; i < QTD_VERTICES; i++)
    {
        // Declara o menor caminho encontrado e o vértice que essa distância pertence
        int menorCaminho = INFINITO;
        int vertice;

        // Se o vértice não pertencer ao vetor de visitados e tiver uma distância menor do que a variável menorCaminho
        for (int i = 0; i < QTD_VERTICES; i++)
        {
            if (visitados[i] == FALSE && menorDistancia[i] <= menorCaminho) {
                // O menor caminho passa a ser essa distância e o vértice com menor caminho passa a ser ele
                menorCaminho = menorDistancia[i];
                vertice = i;
            }
        }
        // Retorna o vértice com o menor caminho estimado
        return vertice;
    }
}

// Atualiza a distância estimada para os vértices adjacentes ao passado para a função
void atualizaDistanciaAdjacentes(
    int vertice,
    int matriz[][QTD_VERTICES],
    int *menorDistancia,
    int *visitados,
    int *verticeAnterior
) {
    for (int i = 0; i < QTD_VERTICES; i++)
    {
        if (
            // A distância do vértice passado tem que ser menor que infinito
            menorDistancia[vertice] != INFINITO
            // Possui adjacência com o vértice passado na função
            && matriz[vertice][i] 
            // Vértice adjacente não foi visitado
            && visitados[i] != TRUE
            // A distância através da desse vértice tem que ser menor
            // que a distância atual estimada para o vértice adjacente
            && menorDistancia[vertice] + matriz[vertice][i] < menorDistancia[i]
        ) {
            // A menor distância do vértice adjacente passa a ser a distância
            // estimada através do vértice passado na função
            menorDistancia[i] = menorDistancia[vertice] + matriz[vertice][i];
            // O vértice pai do vértice "i" passa a ser o da função
            verticeAnterior[i] = vertice;
        }
    }
}

int main()
{
    FILE *FD = NULL;

    int adjacencias[QTD_VERTICES][QTD_VERTICES];
    int verticeOrigem = FALSE; // Vértice escolhido para dar início à busca
    int verticeDestino = FALSE; // Vértice escolhido para finalizar à busca
    int visitados[QTD_VERTICES]; //Vertices que foram visitados pelo algoritmo
    int naoVisitados[QTD_VERTICES]; //Vertices que ainda não foram visitados pelo algoritmo
    int menorDistancia[QTD_VERTICES]; //Armazena a menor distância de um vértice em relação ao vértice de origem escolhido
    int verticeAnterior[QTD_VERTICES]; // Armazena o vértice que vem antes do vértice procurado
    int verticeMenorDistancia = FALSE; // Vértice com menor distância da iteração atual


    //Limpeza de buffer da matriz
    for (int i = 0; i < QTD_VERTICES * QTD_VERTICES; i++)
    {
        adjacencias[i/QTD_VERTICES][i%QTD_VERTICES] = FALSE;
    }

    //Inicialização das distâncias desconhecidas dos vértices e indicar que não há vértices ainda visitados pelo algoritmo
    for (int i = 0; i < QTD_VERTICES; i++)
    {
        menorDistancia[i] = INFINITO;
        visitados[i] = FALSE;
        verticeAnterior[i] = FALSE;
    }
    leituraArquivo(FD, adjacencias); //Lê o arquivo que possui as distâncias de cada vértice e armazena em memória primária na matriz de adjacências
    for (int i = 0; i < QTD_VERTICES; i++)
    {
       for (int j = 0; j < QTD_VERTICES; j++)
       {
           printf("[%d]", adjacencias[i][j]);
       }
       printf("\n");
    }
    
    // Lê os vértices de origem e destino
    printf("Escolha o vertice de origem: \n> ");
    scanf("%d", &verticeOrigem);

    printf("Escolha o vertice de destino: \n> ");
    scanf("%d", &verticeDestino);

    // Atribui 0 como a distância do vértice de origem
    menorDistancia[verticeOrigem] = 0;
    
    // Enquanto não chegar ao vértice de destino
    while (verticeMenorDistancia != verticeDestino) {
        // Busca o vértice com menor distância que ainda não foi visitado
        verticeMenorDistancia = verticeComMenorDistancia(menorDistancia, visitados);
        // O vértice com menor distância recebe o status de visitado
        visitados[verticeMenorDistancia] = TRUE;
        if (verticeMenorDistancia == verticeDestino) 
            break;
        // Atualiza o caminho estimado para os vértices adjacentes ao visitado
        atualizaDistanciaAdjacentes(
            verticeMenorDistancia,
            adjacencias,
            menorDistancia,
            visitados,
            verticeAnterior
        );
    }

    printf("O menor caminho eh: \n");
    exibeMenorCaminho(verticeAnterior, verticeDestino);
    printf("[%d]\n", verticeDestino);

    return 0;
}