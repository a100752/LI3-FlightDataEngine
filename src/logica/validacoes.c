/* src/logica/validacoes.c
   Validações dos dados
*/

#include "logica/validacoes.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <glib.h>

#include "repositorios/repositorio_flights.h"

/* Declaração manual de strptime caso o compilador reclame */
//char *strptime(const char *s, const char *format, struct tm *tm);

/* ----------------------------------------------------------- */
/* -------------------- Helpers internos -------------------- */
/* ----------------------------------------------------------- */

static bool so_digitos(const char *s) {
    if (!s || !*s) return false;
    for (int i = 0; s[i]; i++)
        if (!isdigit((unsigned char)s[i])) return false;
    return true;
}

/* ----------------------------------------------------------- */
/*                 Datas e Horas                               */
/* ----------------------------------------------------------- */

int cmp_dates_ymd_hm(const char *a, const char *b) {
    if (a == b) return 0;
    if (!a) return (b ? -1 : 0);
    if (!b) return (a ? 1 : 0);

    if (strcmp(a, "N/A") == 0 && strcmp(b, "N/A") == 0) return 0;
    if (strcmp(a, "N/A") == 0) return -1;
    if (strcmp(b, "N/A") == 0) return 1;

    return strcmp(a, b);
}

int cmp_dates_ymd(const char *a, const char *b) {
    if (!a || !b) {
        if (!a && !b) return 0;
        return (!a) ? -1 : 1;
    }
    char da[11] = {0}, db[11] = {0};
    strncpy(da, a, 10);
    strncpy(db, b, 10);
    return strcmp(da, db);
}

bool validar_data(const char *s) {
    if (!s || strlen(s) != 10) return false;
    int y, m, d;
    if (sscanf(s, "%4d-%2d-%2d", &y, &m, &d) != 3) return false;
    if (y < 1900 || y > 2025) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > 31) return false;
    if (y > 2025 || (y == 2025 && (m > 9 || (m == 9 && d > 30)))) return false;
    return true;
}

bool validar_datetime(const char *s) {
    if (!s) return false;
    if (strcmp(s, "N/A") == 0) return true;
    if (strlen(s) != 16) return false;

    int y, m, d, h, min;
    if (sscanf(s, "%4d-%2d-%2d %2d:%2d", &y, &m, &d, &h, &min) != 5) return false;
    if (y < 1900 || y > 2025) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > 31) return false;
    if (h < 0 || h > 23) return false;
    if (min < 0 || min > 59) return false;
    if (y == 2025 && (m > 9 || (m == 9 && d > 30))) return false;

    return true;
}

int validar_date_range(const char *date_start, const char *date_end) {
    if (!date_start || !date_end) return -2;
    if (!validar_data(date_start) || !validar_data(date_end)) return -2;
    return cmp_dates_ymd(date_start, date_end);
}

/* ----------------------------------------------------------- */
/*                         Email                               */
/* ----------------------------------------------------------- */

bool validar_email(const char *s) {
    if (!s) return false;

    const char *at = strchr(s, '@');
    if (!at || at == s) return false;

    for (const char *p = s; p < at; p++) {
        if (!islower((unsigned char)*p) && !isdigit((unsigned char)*p) && *p != '.') return false;
    }

    const char *dot = strrchr(at, '.');
    if (!dot || dot <= at + 1) return false;

    for (const char *p = at + 1; p < dot; p++) {
        if (!islower((unsigned char)*p)) return false;
    }

    int rlen = (int)strlen(dot + 1);
    if (rlen < 2 || rlen > 3) return false;
    for (const char *p = dot + 1; *p; p++) {
        if (!islower((unsigned char)*p)) return false;
    }

    return true;
}

/* ----------------------------------------------------------- */
/*                   Códigos e IDs                             */
/* ----------------------------------------------------------- */

bool validar_codigo_iata(const char *s) {
    if (!s) return false;

    /* Reject leading/trailing spaces immediately */
    if (isspace((unsigned char)s[0]) ||
        isspace((unsigned char)s[strlen(s) - 1]))
        return false;


    if (strlen(s) != 3) return false;

    for (int i = 0; i < 3; i++) {
        if (!isupper((unsigned char)s[i]))
            return false;
    }

    return true;
}

bool validar_flight_id(const char *s) {
    if (!s) return false;

    size_t len = strlen(s);
    if (len != 7 && len != 8) return false;

    if (!isupper((unsigned char)s[0]) || !isupper((unsigned char)s[1]))
        return false;

    for (size_t i = 2; i < len; i++)
        if (!isdigit((unsigned char)s[i])) return false;

    return true;
}

bool validar_reservation_id(const char *s) {
    return s && s[0] == 'R' && strlen(s) == 10 && so_digitos(s + 1);
}

bool validar_document_number(const char *s) {
    if (!s) return false;
    int len = (int)strlen(s);
    if (len != 9) return false;
    for (int i = 0; i < len; i++)
        if (!isdigit((unsigned char)s[i])) return false;
    return true;
}

/* ----------------------------------------------------------- */
/*               Coordenadas e Ano                              */
/* ----------------------------------------------------------- */

bool matches_pattern(const char *str, int max) {
    if (!str || !*str) return false;
    if (*str == '-') str++;
    if (!isdigit((unsigned char)*str)) return false;

    long value = 0;
    while (isdigit((unsigned char)*str)) {
        value = value * 10 + (*str - '0');
        str++;
        if (value > max) return false;
    }
    if (*str != '.') return false;
    str++;
    if (!isdigit((unsigned char)*str)) return false;

    int frac = 0;
    while (isdigit((unsigned char)*str)) {
        frac++;
        str++;
        if (frac > 8) return false;
    }
    return *str == '\0';
}

bool validar_latitude(const char *s) {
    if (!s || !*s) return false;
    return matches_pattern(s, 90);
}

bool validar_longitude(const char *s) {
    if (!s || !*s) return false;
    return matches_pattern(s, 180);
}

bool validar_tipo_aeroporto(const char *s) {
    if (!s) return false;
    const char *tipos[] = {
        "small_airport",
        "medium_airport",
        "large_airport",
        "heliport",
        "seaplane_base",
        NULL
    };
    for (int i = 0; tipos[i]; i++)
        if (strcmp(tipos[i], s) == 0) return true;
    return false;
}

bool validar_ano(const char *s) {
    if (!s || strlen(s) != 4 || !so_digitos(s)) return false;
    int ano = atoi(s);
    return ano >= 1900 && ano <= 2025;
}

bool validar_genero(const char *s) {
    return s && (strcmp(s, "M") == 0 || strcmp(s, "F") == 0 || strcmp(s, "O") == 0);
}

/* ----------------------------------------------------------- */
/*                        Tipo de lista                        */
/* ----------------------------------------------------------- */

bool validar_lista_flight_ids(const char *s) {
    if (!s) return false;

    size_t len = strlen(s);
    if (len < 5 || s[0] != '[' || s[len - 1] != ']')
        return false;

    char *copy = g_strndup(s + 1, len - 2);
    if (!copy) return false;

    char *ids[2] = { NULL, NULL };
    int count = 0;

    char *token = strtok(copy, ",");

    while (token) {
        while (isspace((unsigned char)*token)) token++;

        char *end = token + strlen(token) - 1;
        while (end > token && isspace((unsigned char)*end)) {
            *end = '\0';
            end--;
        }

        size_t tlen = strlen(token);
        if (tlen < 3 || token[0] != '\'' || token[tlen - 1] != '\'') {
            g_free(copy);
            return false;
        }

        token[tlen - 1] = '\0';
        char *fid = token + 1;

        if (!validar_flight_id(fid)) {
            g_free(copy);
            return false;
        }

        if (count >= 2) {   /* more than 2 IDs → invalid */
            g_free(copy);
            return false;
        }

        ids[count++] = fid;

        token = strtok(NULL, ",");
    }

    /* must be 1 or 2 */
    if (count == 0) {
        g_free(copy);
        return false;
    }

    /* if 2 IDs, enforce the rule */
    if (count == 2) {
        /* same airline code */
        if (ids[0][0] != ids[1][0] || ids[0][1] != ids[1][1]) {
            g_free(copy);
            return false;
        }

        int n1 = atoi(ids[0] + 2);
        int n2 = atoi(ids[1] + 2);

        if (n2 - n1 != 1) {
            g_free(copy);
            return false;
        }
    }

    g_free(copy);
    return true;
}


/* ----------------------------------------------------------- */
/*                        Lógica de voos                       */
/* ----------------------------------------------------------- */

time_t datetime_to_timestamp(const char *dt)
{
    if (!dt || strcmp(dt, "N/A") == 0)
        return (time_t)-1;

    struct tm tm_time;
    memset(&tm_time, 0, sizeof(struct tm));

    int y, m, d, h, min, sec = 0;

    /* try with seconds */
    int n = sscanf(dt, "%4d-%2d-%2d %2d:%2d:%2d",
                   &y, &m, &d, &h, &min, &sec);

    /* fallback: without seconds */
    if (n < 5) {
        n = sscanf(dt, "%4d-%2d-%2d %2d:%2d",
                   &y, &m, &d, &h, &min);
        sec = 0;
    }

    if (n < 5)
        return (time_t)-1;

    tm_time.tm_year = y - 1900;
    tm_time.tm_mon  = m - 1;
    tm_time.tm_mday = d;
    tm_time.tm_hour = h;
    tm_time.tm_min  = min;
    tm_time.tm_sec  = sec;
    tm_time.tm_isdst = -1;  /* let libc decide */

    return mktime(&tm_time);
}


bool validar_flight_temporal(const char *status,
                             const char *dep, const char *arr,
                             const char *act_dep, const char *act_arr)
{
    if (!dep || !arr || !validar_datetime(dep) || !validar_datetime(arr))
        return false;

    time_t t_dep = datetime_to_timestamp(dep);
    time_t t_arr = datetime_to_timestamp(arr);

    if (t_dep <= 0 || t_arr <= 0 || t_arr < t_dep)
        return false;

    if (status && strcmp(status, "Cancelled") == 0) {
        return act_dep && act_arr &&
               strcmp(act_dep, "N/A") == 0 &&
               strcmp(act_arr, "N/A") == 0;
    }

    if (status && (strcmp(status, "On Time") == 0 || strcmp(status, "Delayed") == 0)) {

        if (!act_dep || !act_arr ||
            strcmp(act_dep, "N/A") == 0 || strcmp(act_arr, "N/A") == 0)
            return false;

        if (!validar_datetime(act_dep) || !validar_datetime(act_arr))
            return false;

        time_t t_act_dep = datetime_to_timestamp(act_dep);
        time_t t_act_arr = datetime_to_timestamp(act_arr);

        if (t_act_dep <= 0 || t_act_arr <= 0) return false;
        if (t_act_arr < t_act_dep) return false;

        if (strcmp(status, "Delayed") == 0) {
            if (t_act_arr <= t_arr)
                return false;
        }

        return true;
    }

    return false;
}

bool validar_flight_origem_destino(const char *orig, const char *dest) {
    return orig && dest && strcmp(orig, dest) != 0;
}

int validar_cmp_dates_ymd_hm(const char *a, const char *b) { return cmp_dates_ymd_hm(a,b); }
int validar_cmp_dates_ymd(const char *a, const char *b) { return cmp_dates_ymd(a,b); }

bool validar_rating(const char *s) {
    if (!s || !*s) return false;
    if (strlen(s) != 1) return false;
    int rating = atoi(s);
    return rating >= 1 && rating <= 5;
}

bool validar_flight_status(const char *s) {
    if (!s) return false;

    return strcmp(s, "Scheduled") == 0 ||
           strcmp(s, "Delayed")   == 0 ||
           strcmp(s, "Cancelled") == 0 ||
           strcmp(s, "On Time")   == 0;
}

bool validar_reservation_status(const char *s) {
    if (!s) return false;
    return (strcmp(s, "Active") == 0 ||
            strcmp(s, "Cancelled") == 0 ||
            strcmp(s, "Completed") == 0);
}

bool validar_preco(const char *s) {
    if (!s || !*s) return false;

    char *end;
    double valor = strtod(s, &end);

    if (end == s || *end != '\0') return false;
    if (valor < 0.0) return false;

    const char *dot = strchr(s, '.');
    if (dot && strlen(dot + 1) > 2) return false;

    return true;
}

bool validar_num_noites(const char *s) {
    if (!s || !*s) return false;
    if (!so_digitos(s)) return false;
    int noites = atoi(s);
    return noites > 0;
}

/* ✅ validação cruzada (existência dos flight_ids no repo) */
bool validar_flights_da_reservation(const char *fids,
                                    RepositorioFlights *r_flights)
{
    if (!fids || !r_flights) return false;

    size_t len = strlen(fids);
    if (len < 2 || fids[0] != '[' || fids[len - 1] != ']')
        return false;

    char *copy = g_strndup(fids + 1, len - 2);
    if (!copy) return false;

    /* remove aspas simples -> espaço; depois strip e tokens */
    g_strdelimit(copy, "'", ' ');
    g_strstrip(copy);

    char *token = strtok(copy, ",");

    while (token != NULL) {
        g_strstrip(token);

        if (!validar_flight_id(token) ||
            !repositorio_flights_get(r_flights, token)) {
            g_free(copy);
            return false;
        }

        token = strtok(NULL, ",");
    }

    g_free(copy);
    return true;
}