#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <sys/stat.h>

#include "utils.h"

/* dados + gestor */
#include "logica/dados.h"
#include "logica/comando.h"
#include "logica/gestor-queries.h"
#include "logica/resultado-query.h"

/* loaders */
#include "parsers/parser.h"

/* repos */
#include "repositorios/repositorio_airports.h"
#include "repositorios/repositorio_aircrafts.h"
#include "repositorios/repositorio_flights.h"
#include "repositorios/repositorio_passengers.h"
#include "repositorios/repositorio_reservations.h"

int main(int argc, char **argv) {
    printf("DEBUG: main start\n");
    fflush(stdout);

    if (argc != 3 && argc != 4) {
        fprintf(stderr,
                "Uso: %s <dataset_dir> <input_file> [output_dir]\n",
                argv[0]);
        return 1;
    }

    const char *dataset_dir = argv[1];
    const char *input_file  = argv[2];
    const char *output_dir  = (argc == 4) ? argv[3] : "resultados";

    mkdir(output_dir, 0777);

    /* ================= DADOS ================= */

    Dados *dados = dados_novo();

    printf("DEBUG[main]: dados=%p\n", (void*)dados);
    printf("DEBUG[main]: repositorioAirports=%p\n",
           (void*)dados_get_repositorioAirports(dados));
    printf("DEBUG[main]: repositorioAircrafts=%p\n",
           (void*)dados_get_repositorioAircrafts(dados));
    printf("DEBUG[main]: repositorioFlights=%p\n",
           (void*)dados_get_repositorioFlights(dados));
    printf("DEBUG[main]: repositorioPassengers=%p\n",
           (void*)dados_get_repositorioPassengers(dados));
    printf("DEBUG[main]: repositorioReservations=%p\n",
           (void*)dados_get_repositorioReservations(dados));
    fflush(stdout);

    RepositorioAirports    *r_ap = dados_get_repositorioAirports(dados);
    RepositorioAircrafts   *r_ac = dados_get_repositorioAircrafts(dados);
    RepositorioPassengers  *r_pa = dados_get_repositorioPassengers(dados);

    load_airports(dataset_dir, output_dir, r_ap);
    load_aircrafts(dataset_dir, output_dir, r_ac);
    load_passengers(dataset_dir, output_dir, r_pa);
    load_flights(dataset_dir, output_dir, dados);
    load_reservations(dataset_dir, output_dir, dados);

    /* ================= GESTOR ================= */

    GestorDeQueries *gestor = gestor_queries_novo(NULL);

    /* ================= INPUT ================= */

    FILE *input = fopen(input_file, "r");
    if (!input) {
        perror("Erro ao abrir ficheiro de input");
        gestor_queries_destroi(gestor);
        dados_destroi(dados);
        return 1;
    }

    char linha[8192];
    int numeroComando = 0;

    while (fgets(linha, sizeof(linha), input)) {

        numeroComando++;

        /* remover \n e \r */
        size_t L = strlen(linha);
        while (L > 0 && (linha[L - 1] == '\n' || linha[L - 1] == '\r'))
            linha[--L] = '\0';

        if (L == 0)
            continue;

        Comando *comando = comando_constroi_de_linha(linha);
        if (!comando)
            continue;

        ResultadoQuery *res =
            gestor_queries_executa(comando, dados);

        char outpath[512];
        snprintf(outpath, sizeof(outpath),
                 "%s/command%d_output.txt",
                 output_dir, numeroComando);

        FILE *out = fopen(outpath, "w");
        if (out) {
            char *texto =
                resultado_query_toString(
                    res,
                    comando_get_separador(comando)
                );
            if (texto) {
                fprintf(out, "%s", texto);
                g_free(texto);
            }
            fclose(out);
        }

        resultado_query_destroi(res);
        comando_destroi(comando);
    }

    fclose(input);

    /* ================= FREE ================= */

    gestor_queries_destroi(gestor);
    dados_destroi(dados);

    return 0;
}