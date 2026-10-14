/* src/logica/entidades/reservation.c */

#include "logica/entidades/reservation.h"
#include <glib.h>
#include <stdlib.h>
#include <string.h>

Reservation *reservation_new(const char *id,
                             const char *flight_ids,
                             const char *document_number,
                             const char *seat,
                             double price,
                             gboolean extra_luggage,
                             gboolean priority_boarding)
{
    Reservation *r = g_slice_new0(Reservation);

    r->id = id ? g_strdup(id) : g_strdup("");
    r->flight_ids = flight_ids ? g_strdup(flight_ids) : g_strdup("");
    r->document_number = document_number ? g_strdup(document_number) : g_strdup("");
    r->seat = seat ? g_strdup(seat) : g_strdup("");
    r->price = price;
    r->extra_luggage = extra_luggage;
    r->priority_boarding = priority_boarding;

    return r;
}

void reservation_free(Reservation *r) {
    if (!r) return;

    g_free(r->id);
    g_free(r->flight_ids);
    g_free(r->document_number);
    g_free(r->seat);
    g_slice_free(Reservation, r);
}