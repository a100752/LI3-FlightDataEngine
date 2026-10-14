#include <stdio.h>
#include <stdlib.h>

#include "logica/query1.h"
#include "logica/comando.h"
#include "logica/dados.h"
#include "logica/resultado-query.h"
#include "repositorios/repositorio_airports.h"

ResultadoQuery *
gestor_query1_executa(Comando *comando, Dados *dados)
{
    char *iata = comando_get_argumento_por_indice(comando, 0);
    ResultadoQuery *res = resultado_query_novo();

    if (!iata)
        return res;

    Airport *a =
        repositorio_airports_get(
            dados_get_repositorioAirports(dados),
            iata
        );

    if (!a) {
        g_free(iata);
        return res;   /* linha vazia */
    }

    Linha *linha = resultado_query_linha_novo();

    char arr[16], dep[16];
    sprintf(arr, "%d", a->arrival_count);
    sprintf(dep, "%d", a->departure_count);

    resultado_query_linha_adiciona_celula(linha, a->code);
    resultado_query_linha_adiciona_celula(linha, a->name);
    resultado_query_linha_adiciona_celula(linha, a->city);
    resultado_query_linha_adiciona_celula(linha, a->country);
    resultado_query_linha_adiciona_celula(linha, a->type);
    resultado_query_linha_adiciona_celula(linha, arr);
    resultado_query_linha_adiciona_celula(linha, dep);

    resultado_query_adiciona_linha(res, linha);
    g_free(iata);
    return res;
}
