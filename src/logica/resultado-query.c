#include "logica/resultado-query.h"
#include <glib.h>
#include <stdlib.h>
#include <string.h>

/* ============================= */
/*           ESTRUTURAS          */
/* ============================= */
struct Linha {
    GPtrArray *celulas;
};

struct ResultadoQuery {
    GPtrArray *linhas;
};

/* ============================= */
/*          LINHA                */
/* ============================= */
Linha *resultado_query_linha_novo(void)
{
    Linha *linha = g_new(Linha, 1);
    linha->celulas = g_ptr_array_new_with_free_func(g_free);
    return linha;
}

void resultado_query_linha_destroi(Linha *linha)
{
    if (!linha) return;
    g_ptr_array_free(linha->celulas, TRUE);
    g_free(linha);
}

void resultado_query_linha_adiciona_celula(Linha *linha, const char *celula)
{
    if (!linha || !celula) return;
    g_ptr_array_add(linha->celulas, g_strdup(celula));
}

/* ============================= */
/*        RESULTADO QUERY        */
/* ============================= */
ResultadoQuery *resultado_query_novo(void)
{
    ResultadoQuery *res = g_new(ResultadoQuery, 1);
    res->linhas = g_ptr_array_new_with_free_func(
        (GDestroyNotify) resultado_query_linha_destroi
    );
    return res;
}

void resultado_query_destroi(ResultadoQuery *res)
{
    if (!res) return;
    g_ptr_array_free(res->linhas, TRUE);
    g_free(res);
}

void resultado_query_adiciona_linha(ResultadoQuery *res, Linha *linha)
{
    if (!res || !linha) return;
    g_ptr_array_add(res->linhas, linha);
}

/* ============================= */
/*        SERIALIZAÇÃO           */
/* ============================= */
char *resultado_query_toString(void *tabelaRespostas, const char *delimitador)
{
    ResultadoQuery *res = (ResultadoQuery *) tabelaRespostas;
    
    /* REGRA: output vazio => uma linha vazia */
    if (!res || !res->linhas || res->linhas->len == 0)
        return g_strdup("\n");
    
    GString *out = g_string_new("");
    gboolean first_line = TRUE;
    
    for (guint i = 0; i < res->linhas->len; i++) {
        Linha *linha = g_ptr_array_index(res->linhas, i);
        
        if (!linha || !linha->celulas || linha->celulas->len == 0)
            continue;
        
        if (!first_line)
            g_string_append(out, "\n");
        first_line = FALSE;
        
        for (guint j = 0; j < linha->celulas->len; j++) {
            if (j > 0)
                g_string_append(out, delimitador);
            char *valor = g_ptr_array_index(linha->celulas, j);
            g_string_append(out, valor ? valor : "");
        }
    }
    
    /* CORREÇÃO: Adicionar newline final se houve pelo menos uma linha */
    if (!first_line)
        g_string_append(out, "\n");
    
    return g_string_free(out, FALSE);
}

/* ============================= */
/*        ORDENAÇÃO              */
/* ============================= */
/* compara por ordem decrescente do segundo elemento
   e desempata por ordem alfabética do primeiro */
int compara_linhas_decrescente_alfabetico(const void *a, const void *b)
{
    Linha *linha_a = *(Linha **) a;
    Linha *linha_b = *(Linha **) b;
    
    char *key_a = g_ptr_array_index(linha_a->celulas, 0);
    char *key_b = g_ptr_array_index(linha_b->celulas, 0);
    char *val_a = g_ptr_array_index(linha_a->celulas, 1);
    char *val_b = g_ptr_array_index(linha_b->celulas, 1);
    
    int ia = val_a ? atoi(val_a) : 0;
    int ib = val_b ? atoi(val_b) : 0;
    
    if (ia != ib)
        return ib - ia;
    
    return g_strcmp0(key_a, key_b);
}

void resultado_query_ordena(ResultadoQuery *res, GCompareFunc cmp)
{
    if (!res || !res->linhas || !cmp) return;
    g_ptr_array_sort(res->linhas, cmp);
}