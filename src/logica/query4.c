/* src/logica/query4.c

Query 4: Qual o passageiro que esteve mais tempo no top 10 de passageiros
que mais gastaram em viagens durante um período?

Output: passenger_id;first_name;last_name;total_semanas
*/

#include "logica/query4.h"
#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "logica/resultado-query.h"
#include "logica/dados.h"
#include "logica/comando.h"

/*Estrutura para contagem de semanas no top 10 - talvez devesse ser has */
typedef struct {
    char *passenger_id;
    int count_top_10;
} Top10Count;

/* Estrutura para gastos semanais */
typedef struct {
    char *passenger_id;
    double total_gasto;
} WeeklySpending;

/** 
 *  Executa a Query 4: conta em quantas semanas cada passageiro aparece no top-10 de gasto semanal.
 * 
 * @param r_pa: repositório de passageiros
 * @param r_rs: repositório de reservas
 * @param r_fl: repositório de voos
 * @param begin_date: data de início (formato "YYYY-MM-DD")
 * @param end_date: data de fim (formato "YYYY-MM-DD")
 * @param outpath: caminho do ficheiro de output

EXEMPLO: EM CONSTRUÇÃO / ЕМ КОНШТРУСАУН / œ∑æ∂æ¥∂π
 */

 /* TO-DO: Implementar a função 
void query4_executar(RepositorioPassengers *r_pa, RepositorioReservations *r_rs, 
                     RepositorioFlights *r_fl, const char *begin_date, 
                     const char *end_date, const char *outpath) {
    FILE *f = fopen(outpath, "w");
    if (!f) return;

    // Verificar se há dados !
    if (!r_pa || !r_rs || !r_fl || repositorio_reservations_size(r_rs) == 0) {
        fprintf(f, "\n");
        fclose(f);
        return;
    }
*/
