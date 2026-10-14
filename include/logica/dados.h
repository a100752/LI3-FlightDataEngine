#ifndef DADOS_H
#define DADOS_H

#include <glib.h>
#include "logica/entidades/reservation.h"

typedef struct Dados Dados;

/* forward declarations */
typedef struct RepositorioAirports RepositorioAirports;
typedef struct RepositorioFlights RepositorioFlights;
typedef struct RepositorioReservations RepositorioReservations;
typedef struct RepositorioAircrafts RepositorioAircrafts;
typedef struct RepositorioPassengers RepositorioPassengers;

/* lifecycle */
Dados *dados_novo(void);
void   dados_destroi(Dados *dados);

/* getters */
RepositorioAirports *
dados_get_repositorioAirports(Dados *dados);

RepositorioFlights *
dados_get_repositorioFlights(Dados *dados);

RepositorioReservations *
dados_get_repositorioReservations(Dados *dados);

RepositorioAircrafts *
dados_get_repositorioAircrafts(Dados *dados);

RepositorioPassengers *
dados_get_repositorioPassengers(Dados *dados);

/* Q6 */
GHashTable *
dados_get_q6_arrivals_por_nacionalidade(Dados *dados);

/**
 * Tenta adicionar uma reserva e calcula estatísticas.
 */
gboolean dados_tenta_adicionar_reservation(Dados *dados, Reservation *res);

#endif
