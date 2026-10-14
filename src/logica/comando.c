#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "logica/comando.h"

struct Comando {
    int numeroQuery;
    char separador;          /* ';' ou '=' */
    GPtrArray *argumentos;   /* char* */
};

static Comando *comando_novo(void) {
    Comando *c = g_new0(Comando, 1);
    c->separador = ';';
    c->argumentos = g_ptr_array_new_with_free_func(g_free);
    return c;
}

void comando_destroi(Comando *c) {
    if (!c) return;
    g_ptr_array_free(c->argumentos, TRUE);
    g_free(c);
}

Comando *comando_constroi_de_linha(char *linha)
{
    if (!linha || linha[0] == '\0')
        return NULL;

    Comando *c = comando_novo();

    /* número da query */
    c->numeroQuery = linha[0] - '0';

    /* separador */
    if (linha[1] == 'S') {
        c->separador = '=';
        linha += 3; /* "1S " */
    } else {
        c->separador = ';';
        linha += 2; /* "1 " */
    }

    /* =====================================================
       Q6: argumento único com espaços
       ===================================================== */
    if (c->numeroQuery == 6) {
        char *arg = g_strdup(linha);
        g_strstrip(arg);
        g_ptr_array_add(c->argumentos, arg);
        return c;
    }

    /* =====================================================
       RESTANTES QUERIES: split normal por espaços
       ===================================================== */
    char *token = strtok(linha, " ");
    while (token) {
        g_ptr_array_add(c->argumentos, g_strdup(token));
        token = strtok(NULL, " ");
    }

    return c;
}

/* ================= GETTERS ================= */

int comando_get_numero_query(Comando *c) {
    return c ? c->numeroQuery : -1;
}

const char *comando_get_separador(Comando *c) {
    static char sep[2];
    sep[0] = c ? c->separador : ';';
    sep[1] = '\0';
    return sep;
}

char *comando_get_argumento_por_indice(Comando *c, int i) {
    if (!c || i < 0 || i >= (int)c->argumentos->len)
        return NULL;
    return g_strdup(g_ptr_array_index(c->argumentos, i));
}
