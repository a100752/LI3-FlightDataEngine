#ifndef QUERY6_H
#define QUERY6_H

#include "logica/comando.h"
#include "logica/dados.h"
#include "logica/resultado-query.h"

/**
 * Q6: Listar o aeroporto de destino mais comum
 *     para passageiros de uma determinada nacionalidade.
 *
 * Comando:
 *   6 <nationality>
 *
 * Output:
 *   code;arrival_count
 *
 * - Apenas voos com estado diferente de "Cancelled"
 * - Em caso de empate, menor código IATA (ordem lexicográfica)
 * - Linha vazia se não existirem passageiros dessa nacionalidade
 */
ResultadoQuery *
gestor_query6_executa(Comando *comando, Dados *dados);

#endif
