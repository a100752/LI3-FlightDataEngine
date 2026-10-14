#ifndef COMANDO_H
#define COMANDO_H

#include <glib.h>

typedef struct Comando Comando;

/* criação / destruição */
Comando *comando_constroi_de_linha(char *linha);
void comando_destroi(Comando *comando);

/* getters */
int         comando_get_numero_query(Comando *comando);
const char *comando_get_separador(Comando *comando);
char       *comando_get_argumento_por_indice(Comando *comando, int indice);

#endif
