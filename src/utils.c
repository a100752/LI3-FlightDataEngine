/* src/utils.c */

#include "utils.h"

#include <glib.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* ================= STRINGS ================= */

char *trim(const char *s) {
    if (!s) return g_strdup("");

    while (*s && (isspace((unsigned char)*s) || *s == '"'))
        s++;

    const char *end = s + strlen(s);
    while (end > s && (isspace((unsigned char)*(end - 1)) || end[-1] == '"'))
        end--;

    return g_strndup(s, (gsize)(end - s));
}

char **split_csv(const char *line, int *n)
{
    if (!line) {
        if (n) *n = 0;
        return NULL;
    }

    /* g_strsplit returns a NULL-terminated array */
    gchar **tokens = g_strsplit(line, ",", -1);
    if (!tokens) {
        if (n) *n = 0;
        return NULL;
    }

    /* count tokens */
    int count = 0;
    while (tokens[count] != NULL)
        count++;

    char **cols = g_new(char *, count);

    for (int i = 0; i < count; i++) {
        cols[i] = trim(tokens[i]);
    }

    g_strfreev(tokens);

    if (n) *n = count;
    return cols;
}


/* ================= DATAS ================= */

int cmp_datetimes(const char *a, const char *b) {
    if (!a || !b) return 0;

    char A[64], B[64];
    strncpy(A, a, sizeof(A) - 1); A[sizeof(A) - 1] = '\0';
    strncpy(B, b, sizeof(B) - 1); B[sizeof(B) - 1] = '\0';

    for (char *p = A; *p; p++) if (*p == ' ') *p = 'T';
    for (char *p = B; *p; p++) if (*p == ' ') *p = 'T';

    return strcmp(A, B);
}

int is_valid_datetime(const char *s) {
    if (!s || strcmp(s, "N/A") == 0) return 0;
    return strlen(s) >= 16;
}

/* ================= PATHS ================= */

char *join_path(const char *dir, const char *file) {
    if (!dir || !file) return NULL;
    return g_strdup_printf("%s/%s", dir, file);
}

/* ================= FLIGHT IDS ================= */

char *clean_ids(const char *s) {
    if (!s) return NULL;

    size_t len = strlen(s);
    if (len < 2) return g_strdup(s);

    if (s[0] == '[' && s[len - 1] == ']')
        return g_strndup(s + 1, (gsize)(len - 2));

    return g_strdup(s);
}

/* ================= PASSAGEIROS ================= */

int obter_passageiros_voo(GHashTable *flight_passengers, const char *flight_id) {
    if (!flight_passengers || !flight_id) return 0;

    int *count = g_hash_table_lookup(flight_passengers, flight_id);
    return count ? *count : 0;
}