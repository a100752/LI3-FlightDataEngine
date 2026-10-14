#include "repositorios/repositorio_flights.h"
#include <glib.h>
#include <stdlib.h>
#include <string.h>

RepositorioFlights *repositorio_flights_novo(void)
{
    RepositorioFlights *r = g_new0(RepositorioFlights, 1);

    r->by_id = g_hash_table_new_full(
        g_str_hash, g_str_equal,
        g_free,
        (GDestroyNotify) flight_free
    );

    r->raw_id_count = g_hash_table_new_full(
        g_str_hash, g_str_equal,
        g_free,
        g_free
    );

    r->delay_por_companhia = g_hash_table_new_full(
        g_str_hash, g_str_equal,
        g_free,
        g_free
    );

    r->count_por_aircraft = g_hash_table_new_full(
        g_str_hash, g_str_equal,
        g_free,
        g_free
    );

    /* Q3 */
    r->departures_por_data = g_hash_table_new_full(
        g_str_hash,
        g_str_equal,
        g_free,
        (GDestroyNotify) g_hash_table_destroy
    );

    r->list_valid = NULL;
    return r;
}

void repositorio_flights_free(RepositorioFlights *r)
{
    if (!r) return;

    g_hash_table_destroy(r->by_id);
    g_hash_table_destroy(r->raw_id_count);
    g_hash_table_destroy(r->delay_por_companhia);
    g_hash_table_destroy(r->count_por_aircraft);
    g_hash_table_destroy(r->departures_por_data);
    g_list_free(r->list_valid);
    g_free(r);
}

/* ================= RAW ================= */

void repositorio_flights_count_raw(RepositorioFlights *r, const char *id)
{
    if (!r || !id) return;

    int *cnt = g_hash_table_lookup(r->raw_id_count, id);
    if (!cnt) {
        cnt = g_new(int, 1);
        *cnt = 1;
        g_hash_table_insert(r->raw_id_count, g_strdup(id), cnt);
    } else {
        (*cnt)++;
    }
}

int repositorio_flights_raw_count(RepositorioFlights *r, const char *id)
{
    int *cnt = r ? g_hash_table_lookup(r->raw_id_count, id) : NULL;
    return cnt ? *cnt : 0;
}

/* ================= VÁLIDOS ================= */

void repositorio_flights_adiciona_valid(RepositorioFlights *r, Flight *f)
{
    if (!r || !f || !f->id) return;

    if (g_hash_table_contains(r->by_id, f->id)) {
        flight_free(f);
        return;
    }

    g_hash_table_insert(r->by_id, g_strdup(f->id), f);
    r->list_valid = g_list_prepend(r->list_valid, f);

    /* ================= Q2 ================= */
    if (f->aircraft && strcmp(f->status, "Cancelled") != 0) {
        int *cnt =
            g_hash_table_lookup(r->count_por_aircraft, f->aircraft);

        if (!cnt) {
            cnt = g_new0(int, 1);
            *cnt = 1;
            g_hash_table_insert(
                r->count_por_aircraft,
                g_strdup(f->aircraft),
                cnt
            );
        } else {
            (*cnt)++;
        }
    }

    /* ================= Q3 ================= */
    if (strcmp(f->status, "Cancelled") != 0 &&
        f->actual_departure &&
        strcmp(f->actual_departure, "N/A") != 0)
    {
        char date[11];
        strncpy(date, f->actual_departure, 10);
        date[10] = '\0';

        GHashTable *per_airport =
            g_hash_table_lookup(r->departures_por_data, date);

        if (!per_airport) {
            per_airport = g_hash_table_new_full(
                g_str_hash, g_str_equal,
                g_free, g_free
            );
            g_hash_table_insert(
                r->departures_por_data,
                g_strdup(date),
                per_airport
            );
        }

        int *cnt = g_hash_table_lookup(per_airport, f->origin);
        if (!cnt) {
            cnt = g_new0(int, 1);
            *cnt = 1;
            g_hash_table_insert(
                per_airport,
                g_strdup(f->origin),
                cnt
            );
        } else {
            (*cnt)++;
        }
    }

    /* ================= Q5 ================= */
    if (!f->status || strcmp(f->status, "Delayed") != 0)
        return;

    long delay = flight_get_delay_minutes(f);
    if (delay <= 0) return;

    AirlineDelayStats *s =
        g_hash_table_lookup(r->delay_por_companhia, f->airline);

    if (!s) {
        s = g_new0(AirlineDelayStats, 1);
        g_hash_table_insert(
            r->delay_por_companhia,
            g_strdup(f->airline),
            s
        );
    }

    s->count++;
    s->total_delay += delay;
}

Flight *repositorio_flights_get(RepositorioFlights *r, const char *id)
{
    return r ? g_hash_table_lookup(r->by_id, id) : NULL;
}

GList *repositorio_flights_list_all(RepositorioFlights *r)
{
    return r ? r->list_valid : NULL;
}

/* Q5 */
GHashTable *
repositorio_flights_get_delay_por_companhia(RepositorioFlights *r)
{
    return r ? r->delay_por_companhia : NULL;
}

/* Q2 */
GHashTable *
repositorio_flights_get_count_por_aircraft(RepositorioFlights *r)
{
    return r ? r->count_por_aircraft : NULL;
}

/* Q3 */
GHashTable *
repositorio_flights_get_departures_por_data(RepositorioFlights *r)
{
    return r ? r->departures_por_data : NULL;
}
