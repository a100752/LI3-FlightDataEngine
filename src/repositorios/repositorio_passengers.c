/* src/repositorios/repositorio_passengers.c

Implementação do repositório de passageiros (Passenger).
Armazena e gere todos os passageiros do sistema, permitindo
pesquisas rápidas por ID (número de documento).
*/
#include "repositorios/repositorio_passengers.h"
#include <glib.h>
#include <stdlib.h>

/**
 * Estrutura que guarda todos os passageiros do sistema.
 * 
 * Usa uma hash table para encontrar passageiros rapidamente pelo ID.
 */
struct RepositorioPassengers {
    GHashTable *by_id;
};

/**
 * Cria um novo repositório de passageiros vazio.
 * 
 * Prepara tudo para começar a adicionar passageiros.
 */
RepositorioPassengers *repositorio_passengers_novo(void){
    RepositorioPassengers *r = g_slice_new0(RepositorioPassengers);
    r->by_id = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) passenger_free);
    return r;
}

/**
 * Liberta toda a memória usada pelo repositório de passageiros.
 * 
 * Apaga todos os passageiros e a própria estrutura do repositório.
 */
void repositorio_passengers_free(RepositorioPassengers *r){
    if(!r) return;
    g_hash_table_destroy(r->by_id);
    g_slice_free(RepositorioPassengers, r);
}

/**
 * Adiciona um passageiro ao repositório.
 * 
 * O passageiro fica guardado e pode ser encontrado pelo seu ID depois.
 */
void repositorio_passengers_adiciona(RepositorioPassengers *r, Passenger *p){
    if(!r||!p) return;
    g_hash_table_insert(r->by_id, g_strdup(p->id), p);
}

/**
 * Procura um passageiro pelo seu número de documento.
 * 
 * Devolve o passageiro se existir, ou NULL se não for encontrado.
 */
Passenger *repositorio_passengers_get(RepositorioPassengers *r, const char *id){
    if(!r||!id) return NULL;
    return (Passenger*) g_hash_table_lookup(r->by_id, id);
}

/**
 * Procura um passageiro pelo seu número de documento.
 * 
 * Devolve o passageiro se existir, ou NULL se não for encontrado.
 */
int repositorio_passengers_size(RepositorioPassengers *r){
    if(!r) return 0;
    return g_hash_table_size(r->by_id);
}

/**
 * Verifica se existe um passageiro com determinado ID.
 * 
 * Devolve 1 se existir, 0 se não existir.
 */
int repositorio_passengers_is_there(RepositorioPassengers *r, const char *id){
    return g_hash_table_lookup(r->by_id, id) != NULL;
}