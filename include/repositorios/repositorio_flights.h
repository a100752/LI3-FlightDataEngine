#ifndef REPOSITORIO_FLIGHTS_H
#define REPOSITORIO_FLIGHTS_H

#include <glib.h>
#include "logica/entidades/flight.h"

/* ================= Estruturas ================= */

typedef struct {
    int count;
    long total_delay;
} AirlineDelayStats;

typedef struct RepositorioFlights {
    GHashTable *by_id;
    GHashTable *raw_id_count;

    /* Q5 */
    GHashTable *delay_por_companhia;

    /* Q2 */
    GHashTable *count_por_aircraft;

    /* Q3: date -> (airport -> count) */
    GHashTable *departures_por_data;

    GList *list_valid;
} RepositorioFlights;

/* ================= API ================= */

RepositorioFlights *repositorio_flights_novo(void);
void repositorio_flights_free(RepositorioFlights *r);

/* RAW */
void repositorio_flights_count_raw(RepositorioFlights *r, const char *id);
int repositorio_flights_raw_count(RepositorioFlights *r, const char *id);

/* VÁLIDOS */
void repositorio_flights_adiciona_valid(RepositorioFlights *r, Flight *f);
Flight *repositorio_flights_get(RepositorioFlights *r, const char *id);
GList *repositorio_flights_list_all(RepositorioFlights *r);

/* Q5 */
GHashTable *repositorio_flights_get_delay_por_companhia(RepositorioFlights *r);

/* Q2 */
GHashTable *repositorio_flights_get_count_por_aircraft(RepositorioFlights *r);

/* Q3 */
GHashTable *repositorio_flights_get_departures_por_data(RepositorioFlights *r);

#endif
