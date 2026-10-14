#ifndef GESTOR_DE_QUERIES_H
#define GESTOR_DE_QUERIES_H

#include <glib.h>

/* forward declarations */
typedef struct GestorDeQueries GestorDeQueries;
typedef struct Estatisticas Estatisticas;
typedef struct Comando Comando;
typedef struct Dados Dados;
typedef struct ResultadoQuery ResultadoQuery;

/* ================= GESTOR ================= */

GestorDeQueries *gestor_queries_novo(Estatisticas *estatisticas);

void gestor_queries_destroi(GestorDeQueries *gestor);

ResultadoQuery *gestor_queries_executa(
    Comando *comando,
    Dados *dados
);

Estatisticas *gestor_queries_get_estatisticas(
    GestorDeQueries *gestorQueries
);

#endif
