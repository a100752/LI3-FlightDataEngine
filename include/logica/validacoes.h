#ifndef LOGICA_VALIDACOES_H
#define LOGICA_VALIDACOES_H

#include <stdbool.h>
#include <time.h>
#include "repositorios/repositorio_flights.h"

/* forward declaration */
typedef struct RepositorioFlights RepositorioFlights;


/* ===================================================== */
/*                     Datas e Horas                     */
/* ===================================================== */

/* Valida apenas a componente de data (YYYY-MM-DD). */
bool validar_data(const char *s);

/* Valida data e hora (YYYY-MM-DD HH:MM ou "N/A"). */
bool validar_datetime(const char *s);

/* Compara datetimes (YYYY-MM-DD HH:MM). */
int validar_cmp_dates_ymd_hm(const char *a, const char *b);

/* Compara apenas datas (YYYY-MM-DD). */
int validar_cmp_dates_ymd(const char *a, const char *b);

/* Valida intervalo de datas (usado em queries). */
int validar_date_range(const char *date_start, const char *date_end);

/* Converte datetime para timestamp. */
time_t datetime_to_timestamp(const char *dt);

/* ===================================================== */
/*                     Emails                            */
/* ===================================================== */

/* Valida formato de email simples. */
bool validar_email(const char *s);

/* ===================================================== */
/*                 Códigos e IDs                         */
/* ===================================================== */

/* Código IATA (3 letras maiúsculas). */
bool validar_codigo_iata(const char *s);

/* ID de voo (AA12345). */
bool validar_flight_id(const char *s);

/* ID de reserva (R123456789). */
bool validar_reservation_id(const char *s);

/* Número de documento (9 dígitos). */
bool validar_document_number(const char *s);

/* ===================================================== */
/*               Coordenadas e Ano                       */
/* ===================================================== */

/* Utilitário interno exposto (usado por latitude/longitude). */
bool matches_pattern(const char *str, int max);

/* Latitude [-90,90]. */
bool validar_latitude(const char *s);

/* Longitude [-180,180]. */
bool validar_longitude(const char *s);

/* Ano (1900–2025). */
bool validar_ano(const char *s);

/* ===================================================== */
/*                   Tipos e Enumerações                 */
/* ===================================================== */

/* Tipo de aeroporto (small_airport, medium_airport, etc.). */
bool validar_tipo_aeroporto(const char *s);

/* Género: M, F ou O. */
bool validar_genero(const char *s);

/* Estatuto de voo: Scheduled, Delayed, Cancelled. */
bool validar_flight_status(const char *s);

/* Estatuto de reserva (se aplicável). */
bool validar_reservation_status(const char *s);

/* ===================================================== */
/*                Listas e Coleções                      */
/* ===================================================== */

/* Lista de flight ids: "[AB12345,CD67890]". */
bool validar_lista_flight_ids(const char *s);

/* ===================================================== */
/*               Lógica específica de Voos               */
/* ===================================================== */

/*
 * Valida consistência temporal de um voo:
 * - arrival >= departure
 * - Cancelled ⇒ actual times = "N/A"
 * - Delayed ⇒ actual >= planeado
 */
bool validar_flight_temporal(const char *status,
                             const char *dep, const char *arr,
                             const char *act_dep, const char *act_arr);

/* Origem e destino diferentes. */
bool validar_flight_origem_destino(const char *orig, const char *dest);

/* ===================================================== */
/*                 Outros validadores                    */
/* ===================================================== */

/* Rating [1..5]. */
bool validar_rating(const char *s);

/* Preço positivo com até 2 casas decimais. */
bool validar_preco(const char *s);

/* Número de noites > 0. */
bool validar_num_noites(const char *s);

bool validar_flights_da_reservation(const char *fids,
                                    RepositorioFlights *r_flights);

#endif
