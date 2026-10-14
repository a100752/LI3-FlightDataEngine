#include "parsers/csv.h"
#include "repositorios/repositorio_airports.h"
#include "repositorios/repositorio_aircrafts.h"
#include "repositorios/repositorio_flights.h"
#include "repositorios/repositorio_passengers.h"
#include "repositorios/repositorio_reservations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <sys/stat.h>

#include "logica/validacoes.h"
#include "utils.h"
#include "logica/dados.h"

/* ================= GENERIC LOADER =================
   OTIMIZAÇÕES:
   - abrir o ficheiro de erros 1x (em vez de abrir/fechar por linha inválida)
   - buffering grande (setvbuf) para input e erros
   - verificar retorno do fgets do header (resolve -Werror=unused-result)
   - assinaturas de function pointers corretas
*/
static int generic_loader(
    const char *dataset_dir,
    const char *filename,
    GList *repositorios,
    const char *outdir,
    void *(*linha_para_obj)(char **cols, int ncols, GList *repositorios),
    void (*adiciona)(void *repo, void *obj),
    void *repo,
    const char *errfile_name
) {
    char *path = join_path(dataset_dir, filename);
    char *path_err = join_path(outdir, errfile_name);

    FILE *f = fopen(path, "r");
    if (!f) {
        g_free(path);
        g_free(path_err);
        if (repositorios) g_list_free(repositorios);
        return 0;
    }

    /* Buffer maior => mais rápido */
    setvbuf(f, NULL, _IOFBF, 1 << 20); /* 1 MB */

    char line[8192];

    /* Ler header (obrigatório) */
    if (!fgets(line, sizeof(line), f)) {
        fclose(f);
        g_free(path);
        g_free(path_err);
        if (repositorios) g_list_free(repositorios);
        return 0;
    }

    /* Abrir ficheiro de erros 1 vez */
    FILE *f_err = fopen(path_err, "w");
    if (f_err) {
        setvbuf(f_err, NULL, _IOFBF, 1 << 20); /* 1 MB */
        fprintf(f_err, "%s", line); /* escreve header */
    }

    int ok = 0, err = 0;

    while (fgets(line, sizeof(line), f)) {
        int ncols = 0;
        char **cols = split_csv_line(line, &ncols);

        void *obj = linha_para_obj(cols, ncols, repositorios);
        if (obj) {
            adiciona(repo, obj);
            ok++;
        } else {
            if (f_err) {
                fprintf(f_err, "%s", line);
            }
            err++;
        }

        for (int i = 0; i < ncols; i++)
            g_free(cols[i]);
        g_free(cols); /* <<< era free(cols); mas como usas g_realloc, o correto é g_free */
    }

    if (f_err) fclose(f_err);
    fclose(f);

    g_free(path);
    g_free(path_err);
    if (repositorios) g_list_free(repositorios);

    printf("Ficheiro %s carregado: %d válidos, %d inválidos\n",
           filename, ok, err);

    return ok;
}

/* ================= WRAPPERS ================= */

static void add_airport_wrapper(void *repo, void *obj) {
    repositorio_airports_adiciona((RepositorioAirports *)repo, (Airport *)obj);
}

static void add_aircraft_wrapper(void *repo, void *obj) {
    repositorio_aircrafts_adiciona((RepositorioAircrafts *)repo, (Aircraft *)obj);
}

/* -------- FLIGHTS -------- */

typedef struct {
    RepositorioFlights  *repo_flights;
    RepositorioAirports *repo_airports;
} FlightLoaderContext;

static void add_flight_wrapper(void *context, void *obj)
{
    FlightLoaderContext *ctx = (FlightLoaderContext *) context;
    Flight *flight = (Flight *) obj;

    if (!ctx || !flight)
        return;

    /* guardar voo válido (Q2, Q3, Q5 dependem disto) */
    repositorio_flights_adiciona_valid(
        ctx->repo_flights,
        flight
    );
}



/* -------- PASSENGERS -------- */

static void add_passenger_wrapper(void *repo, void *obj) {
    repositorio_passengers_adiciona((RepositorioPassengers *)repo, (Passenger *)obj);
}

/* -------- RESERVATIONS -------- */

static void add_reservation_wrapper(void *dados, void *obj) {
    dados_tenta_adicionar_reservation((Dados *)dados, (Reservation *)obj);
}

/* ================= LOADERS ================= */

void load_airports(const char *d, const char *o, RepositorioAirports *r) {
    generic_loader(
        d, "airports.csv", NULL, o,
        linha_para_airport, add_airport_wrapper,
        r, "airports_errors.csv"
    );
}

void load_aircrafts(const char *d, const char *o, RepositorioAircrafts *r) {
    generic_loader(
        d, "aircrafts.csv", NULL, o,
        linha_para_aircraft, add_aircraft_wrapper,
        r, "aircrafts_errors.csv"
    );
}

void load_flights(const char *d, const char *o, Dados *dados) {
    GList *repos = NULL;
    repos = g_list_append(repos, dados_get_repositorioAircrafts(dados));

    FlightLoaderContext ctx = {
        .repo_flights  = dados_get_repositorioFlights(dados),
        .repo_airports = dados_get_repositorioAirports(dados)
    };

    generic_loader(
        d,
        "flights.csv",
        repos,
        o,
        linha_para_flight,
        add_flight_wrapper,
        &ctx,
        "flights_errors.csv"
    );
}

void load_passengers(const char *d, const char *o, RepositorioPassengers *r) {
    generic_loader(
        d, "passengers.csv", NULL, o,
        linha_para_passenger, add_passenger_wrapper,
        r, "passengers_errors.csv"
    );
}

void load_reservations(const char *d, const char *o, Dados *dados) {
    GList *repos = NULL;
    repos = g_list_append(repos, dados_get_repositorioFlights(dados));
    repos = g_list_append(repos, dados_get_repositorioPassengers(dados));

    generic_loader(
        d,
        "reservations.csv",
        repos,
        o,
        linha_para_reservation,
        add_reservation_wrapper,
        dados,
        "reservations_errors.csv"
    );
}