/* passenger.h
 *
 * Estrutura Passenger e funções auxiliares.
 */

#ifndef PASSENGER_H
#define PASSENGER_H

typedef struct Passenger {
    char *id;          /* identificador único */
    char *first_name;
    char *last_name;
    char *dob;         /* data de nascimento (YYYY-MM-DD) */
    char *nationality;
    char *gender;
    char *email;
    char *phone;
    char *address;     /* caminho para ficheiro ou representação */
} Passenger;

/**
 * Cria um novo Passenger duplicando os campos. Retorna NULL em falha.
 */
Passenger *passenger_new(const char *id, const char *first_name, const char *last_name,
                         const char *dob, const char *nationality, const char *gender,
                         const char *email, const char *phone, const char *address);

/**
 * Liberta a memória associada a um Passenger criado por passenger_new.
 */
void passenger_free(Passenger *p);

#endif