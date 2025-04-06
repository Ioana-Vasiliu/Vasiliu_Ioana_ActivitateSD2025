#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct RaportAnalize {
    unsigned int cod_raport;
    unsigned int cod_pacient;
    unsigned char nr_analize;
    char** analize_medicale;
    float* valori_calculate;
    float* valoare_referinta;
    char* data_recoltarii;
} RaportAnalize;

typedef struct nod {
    RaportAnalize info;
    struct nod* next;
} nod;

typedef struct ht {
    int dim;
    nod** vector;
} ht;

RaportAnalize creareRaport(unsigned int cod_raport, unsigned int cod_pacient, unsigned char nr_analize, char** analize_medicale,
    float* valori_calculate, float* valoare_referinta, const char* data_recoltarii) {

    RaportAnalize r;
    r.cod_raport = cod_raport;
    r.cod_pacient = cod_pacient;
    r.nr_analize = nr_analize;

    r.analize_medicale = (char**)malloc(sizeof(char*) * nr_analize);
    for (int i = 0; i < nr_analize; i++) {
        r.analize_medicale[i] = (char*)malloc(strlen(analize_medicale[i]) + 1);
        strcpy(r.analize_medicale[i], analize_medicale[i]);
    }

    r.valori_calculate = (float*)malloc(sizeof(float) * nr_analize);
    r.valoare_referinta = (float*)malloc(sizeof(float) * nr_analize);
    for (int i = 0; i < nr_analize; i++) {
        r.valori_calculate[i] = valori_calculate[i];
        r.valoare_referinta[i] = valoare_referinta[i];
    }

    r.data_recoltarii = (char*)malloc(strlen(data_recoltarii) + 1);
    strcpy(r.data_recoltarii, data_recoltarii);

    return r;
}

int functieHash(RaportAnalize r, int dimensiune) {
    int suma = 0;

    for (int i = 0; i < strlen(r.data_recoltarii); i++) {
        suma += r.data_recoltarii[i];
    }

    return suma % dimensiune;
}

int inserareHt(ht tabela, RaportAnalize r) {
    int pozitie = functieHash(r, tabela.dim);
    nod* nou = (nod*)malloc(sizeof(nod));
    nou->info = r;
    nou->next = NULL;

    if (tabela.vector[pozitie] == NULL) {
        tabela.vector[pozitie] = nou;
    }
    else {
        nod* aux = tabela.vector[pozitie];
        while (aux->next) {
            aux = aux->next;
        }
        aux->next = nou;
    }

    return pozitie;
}


void afisareHt(ht tabela) {
    for (int i = 0; i < tabela.dim; i++) {
        if (tabela.vector[i]) {
            printf("Pozitie %d:\n", i);
            nod* aux = tabela.vector[i];
            while (aux) {
                printf("Raport %u, Pacient %u, Data: %s\n", aux->info.cod_raport, aux->info.cod_pacient, aux->info.data_recoltarii);
                for (int j = 0; j < aux->info.nr_analize; j++) {
                    printf(" - %s: %.2f / %.2f\n", aux->info.analize_medicale[j],
                        aux->info.valori_calculate[j], aux->info.valoare_referinta[j]);
                }
                aux = aux->next;
            }
        }
        else {
            printf("Pozitie %d: goala\n", i);
        }
    }
}

void dezalocare(ht tabela) {
    for (int i = 0; i < tabela.dim; i++) {
        nod* aux = tabela.vector[i];
        while (aux) {
            for (int j = 0; j < aux->info.nr_analize; j++) {
                free(aux->info.analize_medicale[j]);
            }
            free(aux->info.analize_medicale);
            free(aux->info.valori_calculate);
            free(aux->info.valoare_referinta);
            free(aux->info.data_recoltarii);

            nod* temp = aux;
            aux = aux->next;
            free(temp);
        }
    }
    free(tabela.vector);
}

int main() {
    FILE* f = fopen("rapoarte.txt", "r");

    int nr_rapoarte;
    fscanf(f, "%d", &nr_rapoarte);

    ht tabela;
    tabela.dim = 10;
    tabela.vector = (nod**)malloc(sizeof(nod*) * tabela.dim);
    for (int i = 0; i < tabela.dim; i++)
        tabela.vector[i] = NULL;

    for (int i = 0; i < nr_rapoarte; i++) {
        unsigned int cod_raport, cod_pacient;
        unsigned char nr_analize;
        char data[50];
        fscanf(f, "%u %u %hhu %s", &cod_raport, &cod_pacient, &nr_analize, data);

        char** analize_medicale = (char**)malloc(nr_analize * sizeof(char*));
        for (int j = 0; j < nr_analize; j++) {
            char buffer[100];
            fscanf(f, "%s", buffer);
            analize_medicale[j] = (char*)malloc(strlen(buffer) + 1);
            strcpy(analize_medicale[j], buffer);
        }

        float* valori_calculate = (float*)malloc(nr_analize * sizeof(float));
        float* valori_referinta = (float*)malloc(nr_analize * sizeof(float));
        for (int j = 0; j < nr_analize; j++)
            fscanf(f, "%f", &valori_calculate[j]);
        for (int j = 0; j < nr_analize; j++)
            fscanf(f, "%f", &valori_referinta[j]);

        RaportAnalize r = creareRaport(cod_raport, cod_pacient, nr_analize, analize_medicale, valori_calculate, valori_referinta, data);
        inserareHt(tabela, r);

        for (int j = 0; j < nr_analize; j++)
            free(analize_medicale[j]);
        free(analize_medicale);
        free(valori_calculate);
        free(valori_referinta);
    }

    fclose(f);
    afisareHt(tabela);
    dezalocare(tabela);
    return 0;
}
