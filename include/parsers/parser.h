/* parser.h
 *
 * Interfaces para carregar o dataset a partir de ficheiros CSV e preencher os
 * repositórios correspondentes. As funções de loader são pequenas facades que
 * invocam os parsers de linhas e validam erros básicos.
 */

#ifndef PARSER_H
#define PARSER_H

#include "utils.h"
#include "repositorios/repositorio_airports.h"
#include "repositorios/repositorio_aircrafts.h"
#include "repositorios/repositorio_flights.h"
#include "repositorios/repositorio_passengers.h"
#include "repositorios/repositorio_reservations.h"
#include "logica/dados.h"

/* LOADERS SEM DEPENDÊNCIAS */

/*loader de aeroportos*/
void load_airports(const char *dataset_dir, const char *outdir, RepositorioAirports *r);

/*loader de aeronaves*/
void load_aircrafts(const char *dataset_dir, const char *outdir, RepositorioAircrafts *r);

/*loader de passageiros*/
void load_passengers(const char *dataset_dir, const char *outdir, RepositorioPassengers *r);

/* Loader com dependência de Aircrafts */
void load_flights(const char *d, const char *o, Dados *dados);

void load_reservations(const char *d, const char *o, Dados *dados);


#endif

