#include <glib.h>
#include <stdlib.h>
#include <string.h>

#include "logica/dados.h"

/* repositórios */
#include "repositorios/repositorio_airports.h"
#include "repositorios/repositorio_flights.h"
#include "repositorios/repositorio_reservations.h"
#include "repositorios/repositorio_aircrafts.h"
#include "repositorios/repositorio_passengers.h"

#include "logica/entidades/flight.h"
#include "logica/entidades/airport.h"
#include "logica/entidades/reservation.h"
#include "logica/entidades/passenger.h"

struct Dados {
    RepositorioAirports      *repositorioAirports;
    RepositorioFlights       *repositorioFlights;
    RepositorioReservations  *repositorioReservations;
    RepositorioAircrafts     *repositorioAircrafts;
    RepositorioPassengers    *repositorioPassengers;

    /* Q6: nationality -> (airport_code -> arrival_count) */
    GHashTable *q6_arrivals_por_nacionalidade;
};

Dados *dados_novo(void)
{
    Dados *dados = g_new0(Dados, 1);

    dados->repositorioAirports     = repositorio_airports_novo();
    dados->repositorioFlights      = repositorio_flights_novo();
    dados->repositorioReservations = repositorio_reservations_novo();
    dados->repositorioAircrafts    = repositorio_aircrafts_novo();
    dados->repositorioPassengers   = repositorio_passengers_novo();

    dados->q6_arrivals_por_nacionalidade =
        g_hash_table_new_full(
            g_str_hash,
            g_str_equal,
            g_free,
            (GDestroyNotify) g_hash_table_destroy
        );

    return dados;
}

void dados_destroi(Dados *dados)
{
    if (!dados) return;

    repositorio_airports_free(dados->repositorioAirports);
    repositorio_flights_free(dados->repositorioFlights);
    repositorio_reservations_free(dados->repositorioReservations);
    repositorio_aircrafts_free(dados->repositorioAircrafts);
    repositorio_passengers_free(dados->repositorioPassengers);

    g_hash_table_destroy(dados->q6_arrivals_por_nacionalidade);

    g_free(dados);
}

/* getters */

RepositorioAirports *
dados_get_repositorioAirports(Dados *dados) {
    return dados ? dados->repositorioAirports : NULL;
}

RepositorioFlights *
dados_get_repositorioFlights(Dados *dados) {
    return dados ? dados->repositorioFlights : NULL;
}

RepositorioReservations *
dados_get_repositorioReservations(Dados *dados) {
    return dados ? dados->repositorioReservations : NULL;
}

RepositorioAircrafts *
dados_get_repositorioAircrafts(Dados *dados) {
    return dados ? dados->repositorioAircrafts : NULL;
}

RepositorioPassengers *
dados_get_repositorioPassengers(Dados *dados) {
    return dados ? dados->repositorioPassengers : NULL;
}

GHashTable *
dados_get_q6_arrivals_por_nacionalidade(Dados *dados)
{
    return dados ? dados->q6_arrivals_por_nacionalidade : NULL;
}

/* ========================================================= */
/*                  ADIÇÃO DE RESERVAS                       */
/* ========================================================= */

gboolean dados_tenta_adicionar_reservation(Dados *dados, Reservation *res)
{
    if (!dados || !res) return FALSE;

    Passenger *p =
        repositorio_passengers_get(
            dados->repositorioPassengers,
            res->document_number);

    if (!p) {
        reservation_free(res);
        return FALSE;
    }

    /* inserir reserva (validação lógica já feita) */
    repositorio_reservations_adiciona(
        dados->repositorioReservations,
        res,
        dados->repositorioFlights
    );

    if (!res->flight_ids)
        return TRUE;

    size_t len = strlen(res->flight_ids);
    if (len < 2 || res->flight_ids[0] != '[' ||
        res->flight_ids[len - 1] != ']')
        return TRUE;

    /* copiar sem [ ] */
    char *copy = g_strndup(res->flight_ids + 1, len - 2);
    if (!copy) return TRUE;

    char *tok = strtok(copy, ",");

    while (tok) {
        while (*tok == ' ') tok++;

        char *end = tok + strlen(tok) - 1;
        while (end > tok && (*end == ' ' || *end == '\n' || *end == '\r'))
            *end-- = '\0';

        if (*tok == '\'' && *end == '\'') {
            tok++;
            *end = '\0';
        }

        Flight *f =
            repositorio_flights_get(
                dados->repositorioFlights, tok);

        if (f && strcmp(f->status, "Cancelled") != 0) {

            /* ================= Q1 ================= */

            Airport *orig =
                repositorio_airports_get(
                    dados->repositorioAirports, f->origin);
            if (orig)
                orig->departure_count++;

            Airport *dest =
                repositorio_airports_get(
                    dados->repositorioAirports, f->destination);
            if (dest)
                dest->arrival_count++;

            /* ================= Q6 =================
               CONTAR TODOS OS DESTINOS (INTERMÉDIOS + FINAL)
            */

            GHashTable *by_airport =
                g_hash_table_lookup(
                    dados->q6_arrivals_por_nacionalidade,
                    p->nationality);

            if (!by_airport) {
                by_airport = g_hash_table_new_full(
                    g_str_hash, g_str_equal, g_free, g_free);

                g_hash_table_insert(
                    dados->q6_arrivals_por_nacionalidade,
                    g_strdup(p->nationality),
                    by_airport);
            }

            int *cnt =
                g_hash_table_lookup(by_airport, f->destination);

            if (!cnt) {
                cnt = g_new0(int, 1);
                *cnt = 1;
                g_hash_table_insert(
                    by_airport,
                    g_strdup(f->destination),
                    cnt);
            } else {
                (*cnt)++;
            }
        }

        tok = strtok(NULL, ",");
    }

    g_free(copy);
    return TRUE;
}
