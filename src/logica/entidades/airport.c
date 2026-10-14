#include "logica/entidades/airport.h"
#include <glib.h>
#include <stdlib.h>
#include <string.h>

Airport *airport_new(
    const char *code,
    const char *name,
    const char *city,
    const char *country,
    const char *type
) {
    Airport *a = g_slice_new0(Airport);

    a->code = g_strdup(code);
    a->name = g_strdup(name);
    a->city = g_strdup(city);
    a->country = g_strdup(country);
    a->type = g_strdup(type);

    /* ✅ inicializar contadores */
    a->arrival_count = 0;
    a->departure_count = 0;

    return a;
}

void airport_free(Airport *a)
{
    if (!a) return;

    g_free(a->code);
    g_free(a->name);
    g_free(a->city);
    g_free(a->country);
    g_free(a->type);

    g_slice_free(Airport, a);
}
