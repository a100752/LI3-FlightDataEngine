#ifndef FLIGHT_H
#define FLIGHT_H

typedef struct Flight {
    char *id;
    char *departure;
    char *actual_departure;
    char *arrival;
    char *actual_arrival;
    char *status;
    char *origin;
    char *destination;
    char *aircraft;
    char *airline;
} Flight;

/* construtor / destrutor */
Flight *flight_new(const char *id, const char *departure, const char *actual_departure,
                   const char *arrival, const char *actual_arrival, const char *status,
                   const char *origin, const char *destination,
                   const char *aircraft, const char *airline);

void flight_free(Flight *f);

/* atraso em minutos */
long flight_get_delay_minutes(const Flight *f);

/* getters */
const char *flight_get_airline(const Flight *f);

#endif
