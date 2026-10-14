#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logica/comando.h"
#include "logica/dados.h"
#include "logica/query6.h"
#include "logica/resultado-query.h"

/* normaliza string: remove espaços e força uppercase */
static char *normalize(const char *s) {
  if (!s)
    return NULL;

  char *cpy = g_strdup(s);
  g_strstrip(cpy);
  for (char *p = cpy; *p; p++)
    *p = g_ascii_toupper(*p);

  return cpy;
}

ResultadoQuery *gestor_query6_executa(Comando *comando, Dados *dados) {
  char *nat_raw = comando_get_argumento_por_indice(comando, 0);
  if (!nat_raw)
    return resultado_query_novo();

  char *nat = normalize(nat_raw);
  g_free(nat_raw);

  GHashTable *q6 = dados_get_q6_arrivals_por_nacionalidade(dados);

  if (!q6) {
    g_free(nat);
    return resultado_query_novo();
  }

  /* procurar nacionalidade normalizada */
  GHashTable *by_airport = NULL;

  GHashTableIter it_nat;
  gpointer key_nat, val_nat;
  g_hash_table_iter_init(&it_nat, q6);

  while (g_hash_table_iter_next(&it_nat, &key_nat, &val_nat)) {
    char *stored_nat = normalize((char *)key_nat);

    if (strcmp(stored_nat, nat) == 0) {
      by_airport = (GHashTable *)val_nat;
      g_free(stored_nat);
      break;
    }
    g_free(stored_nat);
  }

  if (!by_airport || g_hash_table_size(by_airport) == 0) {
    g_free(nat);
    return resultado_query_novo(); /* linha vazia */
  }

  /* encontrar aeroporto vencedor */
  char *best_code = NULL;
  int best_count = -1;

  GHashTableIter it;
  gpointer key, value;
  g_hash_table_iter_init(&it, by_airport);

  while (g_hash_table_iter_next(&it, &key, &value)) {
    char *code = key;
    int cnt = *(int *)value;

    if (cnt > best_count ||
        (cnt == best_count && (!best_code || strcmp(code, best_code) < 0))) {
      best_code = code;
      best_count = cnt;
    }
  }

  ResultadoQuery *res = resultado_query_novo();
  Linha *linha = resultado_query_linha_novo();

  char buf[16];
  snprintf(buf, sizeof(buf), "%d", best_count);

  resultado_query_linha_adiciona_celula(linha, best_code);
  resultado_query_linha_adiciona_celula(linha, buf);

  resultado_query_adiciona_linha(res, linha);

  g_free(nat);
  return res;
}
