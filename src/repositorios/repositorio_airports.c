/* src/repositorios/repositorio_airports.c */

#include <stdio.h>
#include <ctype.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>

#include "repositorios/repositorio_airports.h"
#include "logica/entidades/airport.h"

/* ========================================================= */
/*              CONSTRUTOR / DESTRUTOR                        */
/* ========================================================= */

RepositorioAirports *
repositorio_airports_novo(void)
{
    RepositorioAirports *r = g_new0(RepositorioAirports, 1);

    r->by_code = g_hash_table_new_full(
        g_str_hash,
        g_str_equal,
        g_free,
        (GDestroyNotify) airport_free
    );

    r->list_valid = NULL;

    return r;
}

void repositorio_airports_free(RepositorioAirports *r)
{
    if (!r) return;

    g_hash_table_destroy(r->by_code);
    g_list_free(r->list_valid);
    g_free(r);
}

/* ========================================================= */
/*                    OPERAÇÕES                              */
/* ========================================================= */

void repositorio_airports_adiciona(RepositorioAirports *r, Airport *a)
{
    if (!r || !a || !a->code) return;

    /* Normalizar código para maiúsculas */
    char upper_code[32];
    size_t len = strlen(a->code);

    for (size_t i = 0; i < len && i < sizeof(upper_code) - 1; i++) {
        upper_code[i] = g_ascii_toupper(a->code[i]);
    }
    upper_code[len] = '\0';

    /* Evitar duplicados */
    if (g_hash_table_contains(r->by_code, upper_code)) {
        airport_free(a);
        return;
    }

    g_hash_table_insert(
        r->by_code,
        g_strdup(upper_code),
        a
    );

    r->list_valid = g_list_prepend(r->list_valid, a);
}

Airport *
repositorio_airports_get(RepositorioAirports *r, const char *code)
{
    if (!r || !code) return NULL;

    char tmp[64];
    size_t j = 0;

    for (size_t i = 0; code[i] && j < sizeof(tmp) - 1; i++) {
        if (code[i] == '"' || code[i] == '\'' ||
            isspace((unsigned char)code[i]))
            continue;

        tmp[j++] = g_ascii_toupper(code[i]);
    }
    tmp[j] = '\0';

    return g_hash_table_lookup(r->by_code, tmp);
}

int repositorio_airports_size(RepositorioAirports *r)
{
    return r ? g_hash_table_size(r->by_code) : 0;
}

GList *
repositorio_airports_list_all(RepositorioAirports *r)
{
    return r ? r->list_valid : NULL;
}
