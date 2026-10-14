/* aircraft.h
 *
 * Entidade Aircraft e construtores/destrutores.
 */

#ifndef AIRCRAFT_H
#define AIRCRAFT_H

typedef struct Aircraft {
    char *identifier; /* unique */
    char *manufacturer;
    char *model;
    int year;
    int total_flights; 
} Aircraft;

/* Cria uma nova Aircraft (duplica os campos passados). */
Aircraft *aircraft_new(const char *id, const char *manufacturer, const char *model, int year);

/* Liberta memória associada a Aircraft */
void aircraft_free(Aircraft *a);

#endif
