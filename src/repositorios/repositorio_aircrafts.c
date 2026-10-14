/* src/repositorios/repositorio_aircrafts.c

Implementação do repositório de aeronaves (Aircraft).
Armazena e gere todas as aeronaves do sistema, permitindo
pesquisas rápidas por ID.
*/
#include "repositorios/repositorio_aircrafts.h"
#include "logica/entidades/aircraft.h"
#include <glib.h>
#include <stdlib.h>
#include <ctype.h>


/**
 * Estrutura que guarda todas as aeronaves do sistema.
 * 
 * Usa uma hash table para encontrar aeronaves rapidamente pelo ID.
 */
struct RepositorioAircrafts {
    GHashTable *by_id; /* key -> char* (clean id), value -> Aircraft* */
};

/**
 * Limpa o identificador de uma aeronave, removendo aspas e espaços.
 * 
 * Alguns IDs vêm com formatação extra que precisa de ser removida.
 */
void clean_identifier(const char *src, char *dest, size_t max_len) {
    size_t j = 0;

    for (size_t i = 0; src[i] && j < max_len - 1; i++) {
        char c = src[i];

        if (c != '"' && c != '\'' && !isspace((unsigned char)c)) {
            dest[j++] = c;
        }
    }
    dest[j] = '\0';
}

/**
 * Cria um novo repositório de aeronaves vazio.
 */
RepositorioAircrafts *repositorio_aircrafts_novo(void){
    RepositorioAircrafts *r = g_slice_new0(RepositorioAircrafts);
    r->by_id = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) aircraft_free);
    return r;
}

/**
 * Liberta toda a memória usada pelo repositório de aeronaves.
 */
void repositorio_aircrafts_free(RepositorioAircrafts *r){
    if(!r) return;
    g_hash_table_destroy(r->by_id);
    g_slice_free(RepositorioAircrafts, r);
}

/**
 * Adiciona uma aeronave ao repositório.
 * 
 * A aeronave é guardada com o ID limpo (sem aspas ou espaços).
 */
void repositorio_aircrafts_adiciona(RepositorioAircrafts *r, Aircraft *a) {
    if (!r || !a || !a->identifier) return;

    // Remover aspas e espaços antes de inserir
    char clean_id[128];
    clean_identifier(a->identifier, clean_id, sizeof(clean_id));
    g_hash_table_insert(r->by_id, g_strdup(clean_id), a);
}

/**
 * Verifica se existe uma aeronave com determinado ID.
 */
int repositorio_aircrafts_is_there(RepositorioAircrafts *r, const char * id) {
    if (!r || !id) return 0;

    char clean_id[128];
    clean_identifier(id, clean_id, sizeof(clean_id));
    return g_hash_table_lookup(r->by_id, clean_id) != NULL;
}

/**
 * Procura uma aeronave pelo seu ID.
 * 
 * Devolve a aeronave se existir, ou NULL se não for encontrada.
 */
Aircraft *repositorio_aircrafts_get(RepositorioAircrafts *r, const char *id) {
    if (!r || !id) return NULL;

    char clean_id[128];
    clean_identifier(id, clean_id, sizeof(clean_id));
    return (Aircraft *) g_hash_table_lookup(r->by_id, clean_id);
}

/**
 * Conta quantas aeronaves estão no repositório.
 */
int repositorio_aircrafts_size(RepositorioAircrafts *r){
    if(!r) return 0;
    return g_hash_table_size(r->by_id);
}

/**
 * Devolve uma lista com todas as aeronaves do repositório.
 * 
 * Útil para percorrer todas as aeronaves quando precisamos.
 */
GList *repositorio_aircrafts_list_all(RepositorioAircrafts *r){
    if(!r) return NULL;
    return g_hash_table_get_values(r->by_id);
}