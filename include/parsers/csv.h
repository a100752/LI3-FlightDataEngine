/* csv.h
 *
 * Helpers e conversores de linhas CSV para entidades do modelo. Utilizados
 * pelos loaders em `parser.c`.
 */

#ifndef CSV_H
#define CSV_H
#include <glib.h>

typedef struct RepositorioFlights RepositorioFlights;


/* Divide uma linha CSV simples em colunas.
 * - line: string CSV
 * - ncols: saída com número de colunas
 * Retorno: array de strings alocadas (cada coluna); chamador deve libertar.
 */
char **split_csv_line(const char *line, int *ncols);

/* Funções que convertem uma linha (array de colunas) numa entidade específica.
 * - cols: array de strings com as colunas
 * - ncols: número de colunas
 * - repositorios: lista de repositórios necessários para resolver referências
 * Retorno: ponteiro para entidade alocada (Airport/Aircraft/Flight/Passenger/Reservation)
 * ou NULL em caso de erro.
 */
void *linha_para_airport(char **cols, int ncols, GList *repositorios);
void *linha_para_aircraft(char **cols, int ncols, GList *repositorios);
void *linha_para_flight(char **cols, int ncols, GList *repos);
void *linha_para_passenger(char **cols, int ncols, GList *repositorios);
void *linha_para_reservation(char **cols, int ncols, GList *repositorios);

#endif