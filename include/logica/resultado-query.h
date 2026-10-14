#ifndef RESULTADO_QUERY_H
#define RESULTADO_QUERY_H

#include <glib.h>

/* forward declarations */
typedef struct Linha Linha;
typedef struct ResultadoQuery ResultadoQuery;

/* ===== Linha ===== */

Linha *resultado_query_linha_novo(void);
void resultado_query_linha_adiciona_celula(Linha *linha, const char *celula);
void resultado_query_linha_destroi(Linha *linha);

/* ===== ResultadoQuery ===== */

ResultadoQuery *resultado_query_novo(void);
void resultado_query_adiciona_linha(ResultadoQuery *resultado, Linha *linha);
void resultado_query_destroi(ResultadoQuery *resultado);

char *resultado_query_toString(void *resultado, const char *delimitador);

void resultado_query_ordena(ResultadoQuery *result, GCompareFunc compare_func);

/* ===== util ===== */

int compara_linhas_decrescente_alfabetico(const void *a, const void *b);

#endif
