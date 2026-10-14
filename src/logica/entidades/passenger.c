/* src/logica/entidades/passenger.c

Implementação da entidade Passenger (Passageiro).
Representa um passageiro com informações pessoais e de contacto.
*/
#include "logica/entidades/passenger.h"
#include <glib.h>
#include <stdlib.h>
#include <string.h>

/*

    Cria um novo passageiro.
Aloca e inicializa uma estrutura Passenger com os dados fornecidos.
Todos os campos string são duplicados para garantir ownership.

@param id: Identificador único do passageiro
@param first_name: Primeiro nome
@param last_name: Apelido
@param dob: Data de nascimento
@param nationality: Nacionalidade
@param gender: Género ("M", "F", "O")
@param email: Endereço de email
@param phone: Número de telefone
@param address: Morada
@param photo: Caminho para foto (opcional)
@return : Ponteiro para o novo passageiro, ou NULL em caso de erro
*/
Passenger *passenger_new(const char *id, const char *first_name, const char *last_name,
                         const char *dob, const char *nationality, const char *gender,
                         const char *email, const char *phone, const char *address) {
    Passenger *p = g_slice_new0(Passenger);
    p->id          = id?g_strdup(id):g_strdup("");
    p->first_name  = first_name?g_strdup(first_name):g_strdup("");
    p->last_name   = last_name?g_strdup(last_name):g_strdup("");
    p->dob         = dob?g_strdup(dob):g_strdup("");
    p->nationality = nationality?g_strdup(nationality):g_strdup("");
    p->gender      = gender?g_strdup(gender):g_strdup("");
    p->email       = email?g_strdup(email):g_strdup("");
    p->phone       = phone?g_strdup(phone):g_strdup("");
    p->address     = address?g_strdup(address):g_strdup("");
    return p;
}

/**

Liberta um passageiro da memória.
Liberta todos os recursos associados ao passageiro, incluindo
strings duplicadas, e depois liberta a estrutura principal.
@param p: Passageiro a libertar (pode ser NULL)
*/
void passenger_free(Passenger *p){
    if(!p) return;
    g_free(p->id); g_free(p->first_name); g_free(p->last_name);
    g_free(p->dob); g_free(p->nationality); g_free(p->gender);
    g_free(p->email); g_free(p->phone); g_free(p->address);
    g_slice_free(Passenger, p);
}