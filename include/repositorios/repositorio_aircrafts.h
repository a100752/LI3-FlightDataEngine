/* repositorio_aircrafts.h
 *
 * Repositório de aeronaves (Aircraft). Contém utilitários para normalizar
 * identificadores e funções de acesso.
 */

#ifndef REPO_AIRCRAFTS_H
#define REPO_AIRCRAFTS_H

#include "logica/entidades/aircraft.h"
#include <glib.h>

typedef struct RepositorioAircrafts RepositorioAircrafts;

/* Cria novo repositório */
RepositorioAircrafts *repositorio_aircrafts_novo(void);

/* Normaliza/limpa um identificador de aeronave (cópia para dest). */
void clean_identifier(const char *src, char *dest, size_t max_len);

/* Liberta o repositório e os objetos associados. */
void repositorio_aircrafts_free(RepositorioAircrafts *r);

/* Adiciona uma Aircraft ao repositório (o repositório pode assumir propriedade). */
void repositorio_aircrafts_adiciona(RepositorioAircrafts *r, Aircraft *a);

/* Obtém aeronave por id (retorna ponteiro ou NULL). */
Aircraft *repositorio_aircrafts_get(RepositorioAircrafts *r, const char *id);

/* Consultas auxiliares */
int repositorio_aircrafts_is_there(RepositorioAircrafts *r, const char * id);
int repositorio_aircrafts_size(RepositorioAircrafts *r);
GList *repositorio_aircrafts_list_all(RepositorioAircrafts *r);

#endif