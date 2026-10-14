#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "logica/query2.h"
#include "logica/comando.h"
#include "logica/dados.h"
#include "logica/resultado-query.h"

#include "logica/entidades/aircraft.h"
#include "repositorios/repositorio_aircrafts.h"
#include "repositorios/repositorio_flights.h"

/* ========================================================= */
/* Estrutura auxiliar                                        */
/* ========================================================= */

typedef struct {
    Aircraft *aircraft;
    int count;
} AircraftCount;

/* ========================================================= */
/* Comparador (enunciado)                                    */
/* ========================================================= */

static int cmp_aircraft(const void *a, const void *b)
{
    const AircraftCount *x = a;
    const AircraftCount *y = b;

    if (x->count != y->count)
        return y->count - x->count;

    return strcmp(x->aircraft->identifier, y->aircraft->identifier);
}

/* ========================================================= */
/* QUERY 2 (VERSÃO EFICIENTE)                                 */
/* ========================================================= */

ResultadoQuery *
gestor_query2_executa(Comando *comando, Dados *dados)
{
    ResultadoQuery *res = resultado_query_novo();

    char *n_str = comando_get_argumento_por_indice(comando, 0);
    char *manufacturer = comando_get_argumento_por_indice(comando, 1);

    if (!n_str)
        return res;

    int N = atoi(n_str);
    g_free(n_str);

    if (N <= 0) {
        g_free(manufacturer);
        return res;
    }

    RepositorioFlights *r_fl =
        dados_get_repositorioFlights(dados);
    RepositorioAircrafts *r_ac =
        dados_get_repositorioAircrafts(dados);

    GHashTable *count =
        repositorio_flights_get_count_por_aircraft(r_fl);

    if (!count || g_hash_table_size(count) == 0) {
        g_free(manufacturer);
        return res;
    }

    AircraftCount *arr =
        g_new0(AircraftCount, g_hash_table_size(count));

    GHashTableIter it;
    gpointer key, value;
    int i = 0;

    g_hash_table_iter_init(&it, count);
    while (g_hash_table_iter_next(&it, &key, &value)) {
        Aircraft *a =
            repositorio_aircrafts_get(r_ac, key);
        if (!a) continue;

        if (manufacturer &&
            strcmp(a->manufacturer, manufacturer) != 0)
            continue;

        arr[i].aircraft = a;
        arr[i].count = *(int *)value;
        i++;
    }

    if (i == 0) {
        g_free(arr);
        g_free(manufacturer);
        return res;
    }

    qsort(arr, i, sizeof(AircraftCount), cmp_aircraft);

    for (int j = 0; j < i && j < N; j++) {
        Linha *linha = resultado_query_linha_novo();

        char total[16];
        snprintf(total, sizeof(total), "%d", arr[j].count);

        resultado_query_linha_adiciona_celula(
            linha, arr[j].aircraft->identifier);
        resultado_query_linha_adiciona_celula(
            linha, arr[j].aircraft->manufacturer);
        resultado_query_linha_adiciona_celula(
            linha, arr[j].aircraft->model);
        resultado_query_linha_adiciona_celula(
            linha, total);

        resultado_query_adiciona_linha(res, linha);
    }

    g_free(arr);
    g_free(manufacturer);
    return res;
}
