#ifndef QUERY5_H
#define QUERY5_H

#include "repositorios/repositorio_airports.h"
#include "repositorios/repositorio_flights.h"
#include "repositorios/repositorio_reservations.h"
#include "logica/dados.h"
#include "logica/comando.h"
#include "logica/resultado-query.h"

ResultadoQuery *gestor_query5_executa(Comando *comando, Dados *dados);

#endif
