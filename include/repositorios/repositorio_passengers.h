/* repositorio_passengers.h
 *
 * Repositório de passageiros — operações básicas.
 */

#ifndef REPO_PASSENGERS_H
#define REPO_PASSENGERS_H

#include "logica/entidades/passenger.h"

typedef struct RepositorioPassengers RepositorioPassengers;

/* Cria e devolve um novo repositório de passageiros. */
RepositorioPassengers *repositorio_passengers_novo(void);

/* Liberta recursos associados. */
void repositorio_passengers_free(RepositorioPassengers *r);

/* Adiciona um Passenger (o repositório pode assumir propriedade). */
void repositorio_passengers_adiciona(RepositorioPassengers *r, Passenger *p);

/* Obtém passageiro por id (ou NULL). */
Passenger *repositorio_passengers_get(RepositorioPassengers *r, const char *id);

/* Número de passageiros. */
int repositorio_passengers_size(RepositorioPassengers *r);

/* Verifica existência de um id */
int repositorio_passengers_is_there(RepositorioPassengers *r, const char * id);

#endif