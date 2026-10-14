/* repositorio_airports.h
 *
 * Repositório de aeroportos — encapsula criação, inserção e consulta por código.
 */

#ifndef REPO_AIRPORTS_H
#define REPO_AIRPORTS_H

#include <glib.h>
#include "logica/entidades/airport.h"

typedef struct RepositorioAirports {
    GHashTable *by_code;
    GList      *list_valid; 
} RepositorioAirports;

/* Cria um novo repositório de aeroportos. */
RepositorioAirports *repositorio_airports_novo(void);

/* Liberta recursos associados ao repositório. */
void repositorio_airports_free(RepositorioAirports *r);

/* Adiciona um Airport ao repositório. */
void repositorio_airports_adiciona(RepositorioAirports *r, Airport *a);

/* Pesquisa um aeroporto por código (IATA) — retorna NULL se não existir. */
Airport *repositorio_airports_get(RepositorioAirports *r, const char *code);

/* Número de aeroportos no repositório. */
int repositorio_airports_size(RepositorioAirports *r);
GList *repositorio_airports_list_all(RepositorioAirports *r);


#endif