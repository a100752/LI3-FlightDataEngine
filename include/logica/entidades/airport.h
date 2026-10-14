#ifndef AIRPORT_H
#define AIRPORT_H

typedef struct Airport {
    char *code;
    char *name;
    char *city;
    char *country;
    char *type;

    /* ✅ Q1: número de passageiros */
    int arrival_count;
    int departure_count;

} Airport;

Airport *airport_new(
    const char *code,
    const char *name,
    const char *city,
    const char *country,
    const char *type
);

void airport_free(Airport *a);

#endif
