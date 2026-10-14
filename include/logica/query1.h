#ifndef QUERY1_H
#define QUERY1_H

#include "repositorios/repositorio_airports.h"
#include "repositorios/repositorio_flights.h"
#include "repositorios/repositorio_reservations.h"
#include "logica/dados.h"
#include "logica/comando.h"
#include "logica/resultado-query.h"

/*
 * Executa a Query 1:
 *  - Resumo de um aeroporto (departures + arrivals)
 *  - Baseado exclusivamente em RESERVAS
 */
ResultadoQuery *gestor_query1_executa(Comando *comando, Dados *dados);

#endif
