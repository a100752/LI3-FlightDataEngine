#ifndef QUERY2_H
#define QUERY2_H

#include "repositorios/repositorio_aircrafts.h"
#include "repositorios/repositorio_flights.h"
#include "logica/dados.h"
#include "logica/comando.h"
#include "logica/resultado-query.h"

ResultadoQuery *
gestor_query2_executa(Comando *comando, Dados *dados);

#endif
