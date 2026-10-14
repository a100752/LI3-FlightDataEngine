/* src/logica/entidades/aircraft.c

Implementação da entidade Aircraft (Aeronave).
Representa uma aeronave com identificador, fabricante, modelo e ano.
*/
#include "logica/entidades/aircraft.h"
#include <glib.h>
#include <stdlib.h>
#include <string.h>

/**

Cria uma nova aeronave.
Aloca e inicializa uma estrutura Aircraft com os dados fornecidos.
Todos os campos string são duplicados para garantir ownership.
@param id: Identificador único da aeronave
@param manufacturer: Fabricante da aeronave (ex: "Boeing")
@param model: Modelo da aeronave (ex: "737-800")
@param year: Ano de fabrico
@ return:  Ponteiro para a nova aeronave, ou NULL em caso de erro
*/
Aircraft *aircraft_new(const char *id, const char *manufacturer, const char *model, int year) {
    Aircraft *a = g_slice_new0(Aircraft);
    a->identifier = id ? g_strdup(id) : g_strdup("");
    a->manufacturer = manufacturer ? g_strdup(manufacturer) : g_strdup("");
    a->model = model ? g_strdup(model) : g_strdup("");
    a->year = year;
    return a;
}

/**

Liberta uma aeronave da memória.
Liberta todos os recursos associados à aeronave, incluindo
strings duplicadas, e depois liberta a estrutura principal.
@param a. Aeronave a libertar (pode ser NULL)
*/
void aircraft_free(Aircraft *a) {
    if (!a) return;
    g_free(a->identifier);
    g_free(a->manufacturer);
    g_free(a->model);
    g_slice_free(Aircraft, a);
}

void aircraft_inc_flights(Aircraft *a) {
    if (a) a->total_flights++;
}
