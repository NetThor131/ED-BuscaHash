#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct
{
    char nome[100];
} Arquivo;

typedef struct
{
    Arquivo *arquivos;
    int32_t qtd_de_arquivos;
} Servidor;

int checksum(char vlr[])
{
    int aux = 0;
    for (int i = 0; i < strlen(vlr); i++)
    {
        if (vlr[i] == ' ')
        {
        }
        else
        {
            aux = aux ^ vlr[i];
        }
    }
    return aux;
}

int32_t hash1(char vlr[], int32_t y)
{
    return (7919 * checksum(vlr));
}

int32_t hash2(char vlr[], int32_t y)
{
    return (104729 * checksum(vlr) + 123);
}

int32_t main(int32_t argc, char *argv[])
{
    int32_t servidor, capacidade, qtdtextos, tmnh, vlrhash;
    char nome[100];

    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    fscanf(input, "%i %i", &servidor, &capacidade);
    Servidor *servidores;
    servidores = (Servidor *)malloc(servidor * sizeof(Servidor));
    for (int32_t i = 0; i < servidor; i++)
    {
        servidores[i].arquivos = (Arquivo *)malloc(capacidade * (sizeof(Arquivo)));
        servidores[i].qtd_de_arquivos = 0;
    }

    fscanf(input, "%i", &qtdtextos);

    for (int x = 0; x < qtdtextos; x++)
    {
        fscanf(input, "%i %[^\n]s", &tmnh, nome);
        int32_t aux = 0;
        vlrhash = hash1(nome, servidor) % servidor;
        printf("%i\n", vlrhash);
        if (servidores[vlrhash].qtd_de_arquivos < capacidade)
        {
            strcpy(servidores[vlrhash].arquivos[servidores[vlrhash].qtd_de_arquivos].nome, nome);
            servidores[vlrhash].qtd_de_arquivos++;
            fprintf(output, "[S%i]", vlrhash);
            fprintf(output, " %s", servidores[vlrhash].arquivos[0].nome);
            for (int32_t i = 1; i < servidores[vlrhash].qtd_de_arquivos; i++)
            {
                fprintf(output, ", %s", servidores[vlrhash].arquivos[i].nome);
            }
            fprintf(output, "\n");
        }
        else
        {
            fprintf(output, "S%i->", vlrhash);
            while (aux <= servidor)
            {
                aux++;
                int32_t h1 = hash1(nome, servidor);
                int32_t h2 = hash2(nome, servidor);
                vlrhash = (h1 + (aux * h2)) % servidor;
                if (servidores[vlrhash].qtd_de_arquivos < capacidade && vlrhash < servidor)
                {
                    fprintf(output, "S%i\n", vlrhash);
                    strcpy(servidores[vlrhash].arquivos[servidores[vlrhash].qtd_de_arquivos].nome, nome);
                    servidores[vlrhash].qtd_de_arquivos++;
                    fprintf(output, "[S%i]", vlrhash);
                    fprintf(output, " %s", servidores[vlrhash].arquivos[0].nome);
                    for (int32_t j = 1; j < servidores[vlrhash].qtd_de_arquivos; j++)
                    {
                        fprintf(output, ", %s", servidores[vlrhash].arquivos[j].nome);
                    }
                    fprintf(output, "\n");
                    break;
                }
            }
        }
    }

    fclose(input);
    fclose(output);
    
    for (int32_t i = 0; i < servidor; i++)
    {
        free(servidores[i].arquivos);
    }

    return 0;
}