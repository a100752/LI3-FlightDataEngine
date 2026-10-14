#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "logica/query3.h"
#include "logica/comando.h"
#include "logica/dados.h"
#include "logica/resultado-query.h"

#include "logica/entidades/airport.h"
#include "repositorios/repositorio_flights.h"
#include "repositorios/repositorio_airports.h"

ResultadoQuery *gestor_query3_executa(Comando *comando, Dados *dados)
{
    char *start = comando_get_argumento_por_indice(comando, 0);
    char *end   = comando_get_argumento_por_indice(comando, 1);

    if (!start || !end)
        return resultado_query_novo();

    RepositorioFlights  *r_fl =
        dados_get_repositorioFlights(dados);
    RepositorioAirports *r_ap =
        dados_get_repositorioAirports(dados);

    GHashTable *by_date =
        repositorio_flights_get_departures_por_data(r_fl);

    GHashTable *total =
        g_hash_table_new_full(
            g_str_hash, g_str_equal,
            g_free, g_free
        );

    GHashTableIter it_date;
    gpointer key_date, val_airports;

    g_hash_table_iter_init(&it_date, by_date);
    while (g_hash_table_iter_next(&it_date, &key_date, &val_airports)) {
        const char *date = key_date;

        if (strcmp(date, start) < 0 || strcmp(date, end) > 0)
            continue;

        GHashTable *per_airport = val_airports;

        GHashTableIter it_ap;
        gpointer key_ap, val_cnt;

        g_hash_table_iter_init(&it_ap, per_airport);
        while (g_hash_table_iter_next(&it_ap, &key_ap, &val_cnt)) {
            int *sum = g_hash_table_lookup(total, key_ap);
            if (!sum) {
                sum = g_new0(int, 1);
                *sum = *(int *)val_cnt;
                g_hash_table_insert(
                    total,
                    g_strdup(key_ap),
                    sum
                );
            } else {
                *sum += *(int *)val_cnt;
            }
        }
    }

    char *best_code = NULL;
    int best_count = -1;

    GHashTableIter it;
    gpointer key, value;
    g_hash_table_iter_init(&it, total);

    while (g_hash_table_iter_next(&it, &key, &value)) {
        char *code = key;
        int cnt = *(int *)value;

        if (cnt > best_count ||
            (cnt == best_count && strcmp(code, best_code) < 0)) {
            best_code = code;
            best_count = cnt;
        }
    }

    ResultadoQuery *res = resultado_query_novo();

    if (best_code) {
        Airport *a = repositorio_airports_get(r_ap, best_code);
        if (a) {
            Linha *linha = resultado_query_linha_novo();
            char buf[16];
            snprintf(buf, sizeof(buf), "%d", best_count);

            resultado_query_linha_adiciona_celula(linha, a->code);
            resultado_query_linha_adiciona_celula(linha, a->name);
            resultado_query_linha_adiciona_celula(linha, a->city);
            resultado_query_linha_adiciona_celula(linha, a->country);
            resultado_query_linha_adiciona_celula(linha, buf);

            resultado_query_adiciona_linha(res, linha);
        }
    }

    g_hash_table_destroy(total);
    g_free(start);
    g_free(end);

    return res;
}
