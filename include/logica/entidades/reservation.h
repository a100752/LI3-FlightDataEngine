/* reservation.h
 *
 * Estrutura de dados para reservas (Reservation) e construtores/destrutores.
 */

#ifndef RESERVATION_H
#define RESERVATION_H
#include <glib.h>

typedef struct Reservation {
    char *id;               /* identificador da reserva */
    char *flight_ids;       /* lista de flight ids (ex: "F1;F2") */
    char *document_number;  /* número de documento do passageiro */
    char *seat;             /* assento reservado */
    double price;           /* preço pago */
    gboolean extra_luggage; /* flag: bagagem extra */
    gboolean priority_boarding; /* flag: embarque prioritário */
} Reservation;

/* Cria uma nova Reservation duplicando os campos recebidos. */
Reservation *reservation_new(const char *id,
                             const char *flight_ids,
                             const char *document_number,
                             const char *seat,
                             double price,
                             gboolean extra_luggage,
                             gboolean priority_boarding);

/* Liberta memória associada a Reservation */
void reservation_free(Reservation *r);

#endif
