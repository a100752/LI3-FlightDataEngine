#ifndef REPOSITORIO_RESERVATIONS_H
#define REPOSITORIO_RESERVATIONS_H

#include <glib.h>
#include "logica/entidades/reservation.h"
#include "repositorios/repositorio_flights.h"
#include <stdbool.h>


typedef struct RepositorioReservations {
    GHashTable *by_id;
    GList      *list_valid;
} RepositorioReservations;

/* Construtor / destrutor */
RepositorioReservations *repositorio_reservations_novo(void);
void repositorio_reservations_free(RepositorioReservations *r);

/* Inserção */
void repositorio_reservations_adiciona(
    RepositorioReservations *r,
    Reservation *res,
    RepositorioFlights *r_flights
);

/* Acesso */
Reservation *repositorio_reservations_get(
    RepositorioReservations *r,
    const char *id
);

GList *repositorio_reservations_list_all(
    RepositorioReservations *r
);


#endif
