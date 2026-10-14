/* include/logica/query4.h
   
    Interface da Query 4 — Passageiro que esteve mais tempo no top 10 semanal de maiores gastos
    
 */

#ifndef QUERY4_H
#define QUERY4_H

#include "repositorios/repositorio_passengers.h"
#include "repositorios/repositorio_reservations.h"
#include "repositorios/repositorio_flights.h"

/** 
 * Executa a Query 4: Determina o passageiro que esteve mais tempo no
 * top 10 semanal de maiores gastos entre `begin_date` e `end_date`, escrevendo
 * o resultado em `outpath`.
 *
 * @param r_pa: repositório de passageiros (obrigatório)
 * @param r_rs: repositório de reservas (obrigatório)
 * @param r_fl: repositório de voos (obrigatório)
 * @param begin_date, end_date: strings de data (YYYY-MM-DD) ou NULL para ausência
 * @param outpath: caminho do ficheiro de saída
 */
void query4_executar(RepositorioPassengers *r_pa, RepositorioReservations *r_rs, RepositorioFlights *r_fl,
                    const char *begin_date,
                    const char *end_date,
                    const char *outpath);

#endif