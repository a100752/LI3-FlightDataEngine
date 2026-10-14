#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>

#include "parsers/csv.h"
#include "utils.h"
#include "logica/validacoes.h"
#include "repositorios/repositorio_airports.h"
#include "repositorios/repositorio_aircrafts.h"
#include "repositorios/repositorio_flights.h"
#include "repositorios/repositorio_passengers.h"
#include "repositorios/repositorio_reservations.h"

/* ========================================================= */
/* =================== CSV SPLIT =========================== */
/* ========================================================= */

char **split_csv_line(const char *line, int *ncols) {
    char *copy = g_strdup(line);
    char *p = copy;
    if (!copy) { *ncols = 0; return NULL; }

    char **cols = NULL;
    int count = 0;

    while (*p) {
        while (*p && (*p == ' ' || *p == '\t')) p++;

        char *token = NULL;

        if (*p == '"') {
            p++;
            GString *sb = g_string_new(NULL);
            while (*p) {
                if (*p == '"') {
                    if (*(p + 1) == '"') {
                        g_string_append_c(sb, '"');
                        p += 2;
                    } else {
                        p++;
                        break;
                    }
                } else {
                    g_string_append_c(sb, *p++);
                }
            }
            token = g_string_free(sb, FALSE);
            while (*p && *p != ',') p++;
            if (*p == ',') p++;
        } else {
            const char *start = p;
            while (*p && *p != ',') p++;
            token = g_strndup(start, p - start);
            if (*p == ',') p++;
        }

        cols = g_realloc(cols, sizeof(char*) * (count + 1));
        cols[count++] = token ? token : g_strdup("");
    }

    *ncols = count;
    g_free(copy);
    return cols;
}

static char *clean_field(const char *s) {
    if (!s) return g_strdup("");

    const char *b = s;
    while (*b && isspace((unsigned char)*b)) b++;

    const char *e = s + strlen(s) - 1;
    while (e >= b && isspace((unsigned char)*e)) e--;

    if (e < b) return g_strdup("");

    int len = (int)(e - b + 1);
    if (len >= 2 && b[0] == '"' && b[len - 1] == '"')
        return g_strndup(b + 1, len - 2);

    return g_strndup(b, len);
}

/* ========================================================= */
/* ======================= AIRPORT ========================= */
/* ========================================================= */

void *linha_para_airport(char **cols, int ncols, GList *repositorios) {
    (void)repositorios;
    if (ncols < 8) return NULL;

    /* 🚨 VALIDATE RAW FIELD (no trimming!) */
    if (!validar_codigo_iata(cols[0])) {
        return NULL;
    }

    char *code = clean_field(cols[0]);
    char *name = clean_field(cols[1]);
    char *city = clean_field(cols[2]);
    char *country = clean_field(cols[3]);
    char *lat = clean_field(cols[4]);
    char *lon = clean_field(cols[5]);
    char *type = clean_field(cols[7]);

    if (!validar_codigo_iata(code) ||
        !validar_latitude(lat) ||
        !validar_longitude(lon) ||
        !validar_tipo_aeroporto(type)) {
        g_free(code); g_free(name); g_free(city); g_free(country);
        g_free(lat); g_free(lon); g_free(type);
        return NULL;
    }

    Airport *a = airport_new(code, name, city, country, type);

    g_free(code); g_free(name); g_free(city); g_free(country);
    g_free(lat); g_free(lon); g_free(type);

    return a;
}

/* ========================================================= */
/* ======================= AIRCRAFT ======================== */
/* ========================================================= */

void *linha_para_aircraft(char **cols, int ncols, GList *repositorios) {
    (void)repositorios;
    if (ncols < 4) return NULL;

    char *id = clean_field(cols[0]);
    char *man = clean_field(cols[1]);
    char *model = clean_field(cols[2]);
    char *year_s = clean_field(cols[3]);

    if (!validar_ano(year_s)) {
        g_free(id); g_free(man); g_free(model); g_free(year_s);
        return NULL;
    }

    int year = atoi(year_s);
    Aircraft *a = aircraft_new(id, man, model, year);

    g_free(id); g_free(man); g_free(model); g_free(year_s);
    return a;
}

/* ========================================================= */
/* ======================= FLIGHT ========================== */
/* ========================================================= */

void *linha_para_flight(char **cols, int ncols, GList *repos) {
    if (ncols < 11) return NULL;

    /* REGRA CRÍTICA: ID NÃO PODE TER ESPAÇOS */
    if (!cols[0]) return NULL;

    if (isspace((unsigned char)cols[0][0]) ||
        isspace((unsigned char)cols[0][strlen(cols[0]) - 1])) {
        return NULL;
    }

    RepositorioAircrafts *r_ac =
        repos ? (RepositorioAircrafts*) repos->data : NULL;

    char *fid = clean_field(cols[0]);
    char *departure = clean_field(cols[1]);
    char *actual_departure = clean_field(cols[2]);
    char *arrival = clean_field(cols[3]);
    char *actual_arrival = clean_field(cols[4]);
    char *status = clean_field(cols[6]);
    char *origin = clean_field(cols[7]);
    char *destination = clean_field(cols[8]);
    char *aircraft = clean_field(cols[9]);
    char *airline = clean_field(cols[10]);

    if (!validar_flight_id(fid)) goto invalido;
    if (!validar_flight_status(status)) goto invalido;
    if (!validar_codigo_iata(origin)) goto invalido;
    if (!validar_codigo_iata(destination)) goto invalido;
    if (!validar_datetime(departure)) goto invalido;
    if (!validar_datetime(arrival)) goto invalido;
    if (!validar_flight_origem_destino(origin, destination)) goto invalido;
    if (!validar_flight_temporal(status, departure, arrival,
                                 actual_departure, actual_arrival)) goto invalido;

    if (r_ac && !repositorio_aircrafts_get(r_ac, aircraft))
        goto invalido;

    Flight *f = flight_new(
        fid, departure, actual_departure,
        arrival, actual_arrival,
        status, origin, destination,
        aircraft, airline
    );

    g_free(fid); g_free(departure); g_free(actual_departure);
    g_free(arrival); g_free(actual_arrival); g_free(status);
    g_free(origin); g_free(destination); g_free(aircraft); g_free(airline);

    return f;

invalido:
    g_free(fid); g_free(departure); g_free(actual_departure);
    g_free(arrival); g_free(actual_arrival); g_free(status);
    g_free(origin); g_free(destination); g_free(aircraft); g_free(airline);
    return NULL;
}

/* ========================================================= */
/* ====================== PASSENGER ======================== */
/* ========================================================= */

void *linha_para_passenger(char **cols, int ncols, GList *repositorios) {
    (void)repositorios;
    if (ncols < 9) return NULL;

    char *id = clean_field(cols[0]);
    char *first = clean_field(cols[1]);
    char *last = clean_field(cols[2]);
    char *dob = clean_field(cols[3]);
    char *nat = clean_field(cols[4]);
    char *gender = clean_field(cols[5]);
    char *email = clean_field(cols[6]);
    char *phone = clean_field(cols[7]);
    char *address = clean_field(cols[8]);

    if (!validar_document_number(id) ||
        !validar_data(dob) ||
        !validar_genero(gender) ||
        !validar_email(email)) {
        g_free(id); g_free(first); g_free(last); g_free(dob);
        g_free(nat); g_free(gender); g_free(email);
        g_free(phone); g_free(address);
        return NULL;
    }

    Passenger *p = passenger_new(
        id, first, last,
        dob, nat, gender,
        email, phone, address
    );

    g_free(id); g_free(first); g_free(last); g_free(dob);
    g_free(nat); g_free(gender); g_free(email);
    g_free(phone); g_free(address);

    return p;
}

/* ========================================================= */
/* ===================== RESERVATION ======================= */
/* ========================================================= */

void *linha_para_reservation(char **cols, int ncols, GList *repos) {
    if (ncols < 7) return NULL;

    /* CORREÇÃO: Extrair repositórios da GList */
    RepositorioFlights *r_flights = NULL;
    RepositorioPassengers *r_pass = NULL;
    
    if (repos) {
        r_flights = (RepositorioFlights*) repos->data;
        if (repos->next) {
            r_pass = (RepositorioPassengers*) repos->next->data;
        }
    }

    char *rid   = clean_field(cols[0]);
    char *fids  = clean_field(cols[1]);
    char *pid   = clean_field(cols[2]);
    char *date  = clean_field(cols[3]);
    char *price = clean_field(cols[4]);

    /* Validações de formato */
    if (!validar_reservation_id(rid) ||
        !validar_lista_flight_ids(fids) ||  
        !validar_document_number(pid) ||
        !validar_preco(price)) {
        g_free(rid); g_free(fids); g_free(pid);
        g_free(date); g_free(price);
        return NULL;
    }

    /* CORREÇÃO: Adicionar validação cruzada */
    if (r_flights && !validar_flights_da_reservation(fids, r_flights)) {
        g_free(rid); g_free(fids); g_free(pid);
        g_free(date); g_free(price);
        return NULL;
    }

    /* Validar se passenger existe */
    if (r_pass && !repositorio_passengers_get(r_pass, pid)) {
        g_free(rid); g_free(fids); g_free(pid);
        g_free(date); g_free(price);
        return NULL;
    }

    Reservation *res = reservation_new(
        rid, fids, pid, date,
        atof(price),
        g_ascii_strcasecmp(cols[5], "true") == 0,
        g_ascii_strcasecmp(cols[6], "true") == 0
    );

    g_free(rid); g_free(fids); g_free(pid);
    g_free(date); g_free(price);

    return res;
}