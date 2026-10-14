#ifndef QUERY3_H
#define QUERY3_H

#include "repositorios/repositorio_airports.h"
#include "repositorios/repositorio_flights.h"
#include "logica/dados.h"
#include "logica/comando.h"
#include "logica/resultado-query.h"

ResultadoQuery *
gestor_query3_executa(Comando *comando, Dados *dados);

#endif
