/* utils.h
# *
# * Funções utilitárias usadas por vários módulos do projecto. 
# */

#ifndef UTILS_H
#define UTILS_H

#include <glib.h>
#include "repositorios/repositorio_reservations.h"

/* --- Manipulação de strings --- */
/**
 * trim
 * Remove espaços em branco e aspas do início e fim da string.
 *  - s: string de entrada (não modificada)
 */
char *trim(const char *s);

/**
 * split_csv
 * Divide uma linha CSV simples pelos separadores ',' (não trata escapes complexos).
 * 
 *  - line: linha CSV a partir da qual extrair colunas
 *  - n: saída com número de colunas
 */
char **split_csv(const char *line, int *n);

/* --- Manipulação de datas --- */
/**
 * cmp_datetimes
 * Compara duas datas/horas representadas como strings no formato
 * "YYYY-MM-DDTHH:MM" ou "YYYY-MM-DD HH:MM". Substitui espaço por 'T'
 * antes de comparar lexicograficamente.
 * Retorna <0 se a<b, 0 se iguais, >0 se a>b.
 */
int cmp_datetimes(const char *a, const char *b);

/**
 * is_valid_datetime
 * Verifica se a string representa uma data/hora válida razoável (não "N/A").
 * Retorna 1 se válida, 0 caso contrário.
 */
int is_valid_datetime(const char *s);

/* --- Caminhos / ficheiros --- */
/**
 * join_path
 * Junta diretório e nome de ficheiro numa nova string com separador '/'.
 * Retorna uma string alocada (g_strdup_printf) que o chamador deve g_free.
 */
char *join_path(const char *dir, const char *file);

/**
 * @brief Remove os colchetes de uma string de IDs.
 * 
 * Transforma "[ABC12345, DEF67890]" em "ABC12345,DEF67890"
 * 
 * @param s String com IDs entre colchetes
 * @return String limpa (deve ser libertada com free())
 */
char *clean_ids(const char *s);

/**
 * @brief Obtém o número de passageiros de um voo.
 * 
 * Faz lookup O(1) no índice de passageiros.
 * 
 * @param flight_passengers Índice flight_id -> int*
 * @param flight_id ID do voo
 * @return Número de passageiros, ou 0 se não existir
 */
int obter_passageiros_voo(GHashTable *flight_passengers, const char *flight_id);

#endif
