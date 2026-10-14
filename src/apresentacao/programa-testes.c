/* src/testes/programa-testes.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <glib.h>
#include <sys/resource.h>
#include <sys/time.h>

typedef struct {
    int total;
    int correct;
    double time_ms;
    GList *discrepancias;
} QueryResult;

typedef struct {
    int linha;
    char *ficheiro;
} Discrepancia;

/* ========================================================= */
/*                  COMPARAÇÃO DE FICHEIROS                  */
/* ========================================================= */

int comparar_ficheiros(const char *f1, const char *f2) {
    FILE *fp1 = fopen(f1, "r");
    FILE *fp2 = fopen(f2, "r");
    if (!fp1 || !fp2) {
        if (fp1) fclose(fp1);
        if (fp2) fclose(fp2);
        return -1;
    }

    char l1[2048], l2[2048];
    int line = 1;

    while (fgets(l1, sizeof(l1), fp1) &&
           fgets(l2, sizeof(l2), fp2)) {
        if (strcmp(l1, l2) != 0) {
            fclose(fp1);
            fclose(fp2);
            return line;
        }
        line++;
    }

    if (fgets(l1, sizeof(l1), fp1) ||
        fgets(l2, sizeof(l2), fp2)) {
        fclose(fp1);
        fclose(fp2);
        return line;
    }

    fclose(fp1);
    fclose(fp2);
    return 0;
}

/* ========================================================= */
/*               ORDENAR COMMAND OUTPUTS                     */
/* ========================================================= */

gint comparar_command(gconstpointer a, gconstpointer b) {
    int na = 0, nb = 0;
    sscanf(a, "command%d_output.txt", &na);
    sscanf(b, "command%d_output.txt", &nb);
    return na - nb;
}

/* ========================================================= */
/*                          MAIN                             */
/* ========================================================= */

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr,
            "Uso: %s <dataset_dir> <input_file> <expected_output_dir>\n",
            argv[0]);
        return 1;
    }

    const char *dataset_dir = argv[1];
    const char *input_file  = argv[2];
    const char *expected_dir = argv[3];
    const char *results_dir = "resultados";

    struct timeval t_start, t_end;
    gettimeofday(&t_start, NULL);

    char cmd[1024];
    snprintf(cmd, sizeof(cmd),
        "./programa-principal %s %s",
        dataset_dir, input_file);

    if (system(cmd) != 0) {
        fprintf(stderr, "Erro ao executar programa-principal\n");
        return 1;
    }

    /* ================= COMMAND OUTPUTS ================= */

    QueryResult queries[6] = {0};
    for (int i = 0; i < 6; i++)
        queries[i].discrepancias = NULL;

    DIR *d = opendir(results_dir);
    if (!d) { perror("resultados"); return 1; }

    GList *files = NULL;
    struct dirent *e;

    while ((e = readdir(d)) != NULL) {
        if (strncmp(e->d_name, "command", 7) == 0 &&
            strstr(e->d_name, "_output.txt")) {
            files = g_list_append(files, g_strdup(e->d_name));
        }
    }
    closedir(d);

    files = g_list_sort(files, comparar_command);

    for (GList *l = files; l; l = l->next) {
        char *fname = l->data;
        char res[512], exp[512];

        snprintf(res, sizeof(res), "%s/%s", results_dir, fname);
        snprintf(exp, sizeof(exp), "%s/%s", expected_dir, fname);

        struct timeval qs, qe;
        gettimeofday(&qs, NULL);
        int diff = comparar_ficheiros(res, exp);
        gettimeofday(&qe, NULL);

        double ms =
            (qe.tv_sec - qs.tv_sec) * 1000.0 +
            (qe.tv_usec - qs.tv_usec) / 1000.0;

        int num = 0, q = -1;
        if (sscanf(fname, "command%d_output.txt", &num) == 1) {
            if      (num <= 50)  q = 0;
            else if (num <= 100)  q = 1;
            else if (num <= 150) q = 2;
            else if (num <= 200) q = 3;
            else if (num <= 250) q = 4;
            else if (num <= 300) q = 5;
        }

        if (q >= 0) {
            queries[q].total++;
            queries[q].time_ms += ms;

            if (diff == 0) {
                queries[q].correct++;
            } else if (diff > 0) {
                Discrepancia *d = malloc(sizeof(Discrepancia));
                d->linha = diff;
                d->ficheiro = g_strdup(fname);
                queries[q].discrepancias =
                    g_list_append(queries[q].discrepancias, d);
            }
        }

        g_free(fname);
    }
    g_list_free(files);

    /* ================= RESULTADOS ================= */

    printf("\nRESULTADOS DAS QUERIES:\n");
    for (int i = 0; i < 6; i++) {
        printf("Q%d: %d / %d corretos\n",
            i+1, queries[i].correct, queries[i].total);

        for (GList *l = queries[i].discrepancias; l; l = l->next) {
            Discrepancia *d = l->data;
            printf("\tErro Q%d: linha %d em %s\n",
                i+1, d->linha, d->ficheiro);
            g_free(d->ficheiro);
            free(d);
        }
        g_list_free(queries[i].discrepancias);
    }

    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    printf("\nMemória máxima: %ld KB\n", ru.ru_maxrss);

    gettimeofday(&t_end, NULL);
    double total =
        (t_end.tv_sec - t_start.tv_sec) +
        (t_end.tv_usec - t_start.tv_usec) / 1e6;

    printf("Tempo total: %.2f s\n", total);
    return 0;
}
