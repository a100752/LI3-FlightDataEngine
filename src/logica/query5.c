#include "logica/gestor-queries.h"
#include "logica/dados.h"
#include "logica/resultado-query.h"
#include "logica/comando.h"

#include "repositorios/repositorio_flights.h"

#include <math.h>
#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* ===============================
   Estrutura auxiliar para ordenar
   =============================== */
typedef struct {
    char   *airline;
    int     delayed_count;
    double  avg_delay;
} Q5Tmp;

/* ===============================
   Comparador: média desc, nome asc
   =============================== */
static int cmp_q5(const void *a, const void *b)
{
    const Q5Tmp *x = (const Q5Tmp *) a;
    const Q5Tmp *y = (const Q5Tmp *) b;

    long rx = lround(x->avg_delay * 1000.0);
    long ry = lround(y->avg_delay * 1000.0);

    if (rx < ry) return 1;   /* descending */
    if (rx > ry) return -1;

    /* tie → alphabetical */
    return strcmp(x->airline, y->airline);
}

ResultadoQuery *
gestor_query5_executa(Comando *comando, Dados *dados)
{
    /* ===============================
       Ler argumento N
       =============================== */
    char *nStr = comando_get_argumento_por_indice(comando, 0);
    if (!nStr) return NULL;

    int n = atoi(nStr);
    g_free(nStr);
    if (n <= 0) return NULL;

    ResultadoQuery *resultado = resultado_query_novo();

    /* ===============================
       Obter estatísticas do repositório
       =============================== */
    RepositorioFlights *rf = dados_get_repositorioFlights(dados);
    GHashTable *stats = repositorio_flights_get_delay_por_companhia(rf);

    if (!stats || g_hash_table_size(stats) == 0)
        return resultado;

    /* =====================================================
       DEBUG ABSOLUTO — TEM DE PRINTAR
       ===================================================== */
    {
        GHashTableIter it_dbg;
        gpointer k_dbg, v_dbg;

        g_hash_table_iter_init(&it_dbg, stats);
        while (g_hash_table_iter_next(&it_dbg, &k_dbg, &v_dbg)) {
            AirlineDelayStats *s = (AirlineDelayStats *) v_dbg;
            (void) s; /* evita -Wunused-variable com -Werror */
        }
        fflush(stdout);
    }
    /* ===================================================== */

    int size = g_hash_table_size(stats);
    Q5Tmp *arr = g_new0(Q5Tmp, size);

    /* ===============================
       Hash → array
       =============================== */
    GHashTableIter it;
    gpointer key, value;
    int i = 0;

    g_hash_table_iter_init(&it, stats);
    while (g_hash_table_iter_next(&it, &key, &value)) {
        AirlineDelayStats *s = (AirlineDelayStats *) value;

        if (s->count <= 0)
            continue;

        arr[i].airline = (char *) key;
        arr[i].delayed_count = s->count;
        arr[i].avg_delay = (double) s->total_delay / s->count;
        i++;
    }

    /* ===============================
       Ordenar
       =============================== */
    qsort(arr, i, sizeof(Q5Tmp), cmp_q5);

    /* ===============================
       Output
       =============================== */
    for (int j = 0; j < n && j < i; j++) {
        Linha *linha = resultado_query_linha_novo();

        resultado_query_linha_adiciona_celula(linha, arr[j].airline);

        char buf1[32];
        snprintf(buf1, sizeof(buf1), "%d", arr[j].delayed_count);
        resultado_query_linha_adiciona_celula(linha, buf1);

        char buf2[32];
        snprintf(buf2, sizeof(buf2), "%.3f", arr[j].avg_delay);
        resultado_query_linha_adiciona_celula(linha, buf2);

        resultado_query_adiciona_linha(resultado, linha);
    }

    g_free(arr);
    return resultado;
}