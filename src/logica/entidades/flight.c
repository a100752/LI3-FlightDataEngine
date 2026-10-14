#include "logica/entidades/flight.h"
#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

/* ===================== */
/* CONSTRUTOR / DESTRUTOR */
/* ===================== */

Flight *flight_new(const char *id,
                   const char *departure,
                   const char *actual_departure,
                   const char *arrival,
                   const char *actual_arrival,
                   const char *status,
                   const char *origin,
                   const char *destination,
                   const char *aircraft,
                   const char *airline)
{
    Flight *f = g_slice_new0(Flight);

    f->id = g_strdup(id);
    f->departure = g_strdup(departure);
    f->actual_departure = g_strdup(actual_departure);
    f->arrival = g_strdup(arrival);
    f->actual_arrival = g_strdup(actual_arrival);
    f->status = g_strdup(status);
    f->origin = g_strdup(origin);
    f->destination = g_strdup(destination);
    f->aircraft = g_strdup(aircraft);
    f->airline = g_strdup(airline);

    return f;
}

void flight_free(Flight *f)
{
    if (!f) return;

    g_free(f->id);
    g_free(f->departure);
    g_free(f->actual_departure);
    g_free(f->arrival);
    g_free(f->actual_arrival);
    g_free(f->status);
    g_free(f->origin);
    g_free(f->destination);
    g_free(f->aircraft);
    g_free(f->airline);

    g_slice_free(Flight, f);
}

/* ===================== */
/* LÓGICA DA ENTIDADE    */
/* ===================== */

int flight_is_delayed(const Flight *f)
{
    return f && g_strcmp0(f->status, "Delayed") == 0;
}

/* Converte "YYYY-MM-DD HH:MM" em time_t */
static time_t parse_datetime(const char *s)
{
    if (!s || strcmp(s, "N/A") == 0) return 0;

    int year, month, day, hour, min;
    if (sscanf(s, "%d-%d-%d %d:%d",
               &year, &month, &day, &hour, &min) != 5)
        return 0;

    struct tm tm = {0};
    tm.tm_year = year - 1900;
    tm.tm_mon  = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min  = min;
    tm.tm_isdst = -1;

    return mktime(&tm);
}

/*
 * Calcula o atraso real do voo (em minutos).
 * Regra:
 *   - só conta se status == "Delayed"
 *   - atraso = max(atraso_partida, atraso_chegada)
 *   - se não houver atraso real, devolve 0
 */

long flight_get_delay_minutes(const Flight *f)
{
    if (!f || strcmp(f->status, "Delayed") != 0)
        return 0;

    // MUDANÇA AQUI: Usar departure em vez de arrival
    if (!f->departure || !f->actual_departure)
        return 0;

    if (strcmp(f->departure, "N/A") == 0 ||
        strcmp(f->actual_departure, "N/A") == 0)
        return 0;

    time_t sched = parse_datetime(f->departure);        // departure
    time_t actual = parse_datetime(f->actual_departure); // actual_departure

    if (sched <= 0 || actual <= sched)
        return 0;

    return (actual - sched) / 60;
}
