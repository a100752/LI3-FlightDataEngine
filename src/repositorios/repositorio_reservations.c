/* src/repositorios/repositorio_reservations.c */

#include "repositorios/repositorio_reservations.h"
#include <glib.h>
#include <stdlib.h>
#include <string.h>

/* ✅ Validação lógica pedida no enunciado:
   - cada flight existe e é único
   - se houver 2 flights:
       * airline igual
       * destination(primeiro) == origin(segundo)
*/
static bool validar_reservation_flights(Reservation *r, RepositorioFlights *rf) {
    if (!r || !rf || !r->flight_ids) return false;

    char *copy = g_strdup(r->flight_ids);
    size_t len = strlen(copy);
    if (len < 2 || copy[0] != '[' || copy[len - 1] != ']') {
        g_free(copy);
        return false;
    }

    copy[len - 1] = '\0';
    char *p = copy + 1;

    char *tok = strtok(p, ",");
    Flight *first = NULL;
    int count = 0;

    while (tok) {
        while (*tok == ' ') tok++;
        char *end = tok + strlen(tok) - 1;

        if (*tok == '\'' && *end == '\'') {
            tok++;
            *end = '\0';
        }

        /* existe e é único */
        if (repositorio_flights_raw_count(rf, tok) != 1) {
            g_free(copy);
            return false;
        }

        Flight *f = repositorio_flights_get(rf, tok);
        if (!f) {
            g_free(copy);
            return false;
        }

        if (!first) {
            first = f;
        } else {
            /* mesma airline */
            if (strcmp(first->airline, f->airline) != 0) {
                g_free(copy);
                return false;
            }

            /* ✅ regra de ligação entre voos */
            if (strcmp(first->destination, f->origin) != 0) {
                g_free(copy);
                return false;
            }
        }

        count++;
        if (count > 2) {
            g_free(copy);
            return false;
        }

        tok = strtok(NULL, ",");
    }

    g_free(copy);
    return (count == 1 || count == 2);
}

RepositorioReservations *repositorio_reservations_novo(void) {
    RepositorioReservations *r = g_new0(RepositorioReservations, 1);

    r->by_id = g_hash_table_new_full(
        g_str_hash, g_str_equal,
        g_free,
        (GDestroyNotify) reservation_free
    );

    r->list_valid = NULL;
    return r;
}

void repositorio_reservations_free(RepositorioReservations *r) {
    if (!r) return;
    g_hash_table_destroy(r->by_id);
    g_list_free(r->list_valid);
    g_free(r);
}

void repositorio_reservations_adiciona(
    RepositorioReservations *r,
    Reservation *res,
    RepositorioFlights *rf
) {
    if (!r || !res || !rf) return;

    if (!validar_reservation_flights(res, rf))
        return;

    if (g_hash_table_contains(r->by_id, res->id))
        return;

    g_hash_table_insert(r->by_id, g_strdup(res->id), res);
    r->list_valid = g_list_prepend(r->list_valid, res);
}