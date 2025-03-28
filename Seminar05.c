#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
    Masina info;
    struct Nod* next;
    struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla {
    Nod* inceput;
    Nod* final;
};
typedef struct ListaDubla Lista;

Masina citireMasinaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    fgets(buffer, 100, file);
    char* aux;
    Masina m1;
    aux = strtok(buffer, sep);
    m1.id = atoi(aux);
    m1.nrUsi = atoi(strtok(NULL, sep));
    m1.pret = atof(strtok(NULL, sep));
    aux = strtok(NULL, sep);
    m1.model = malloc(strlen(aux) + 1);
    strcpy_s(m1.model, strlen(aux) + 1, aux);

    aux = strtok(NULL, sep);
    m1.numeSofer = malloc(strlen(aux) + 1);
    strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

    m1.serie = *strtok(NULL, sep);
    return m1;
}

void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi: %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Lista lis) {
    Nod* p = lis.inceput;
    while (p) {
        afisareMasina(p->info);
        p = p->next;
    }
}

void afisareListaMasiniDeLaFinal(Lista lis) {
    Nod* p = lis.final;
    while (p) {
        afisareMasina(p->info);
        p = p->prev;
    }
}

void adaugaMasinaInLista(Lista* lis, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next = NULL;
    nou->prev = lis->final;

    if (lis->final != NULL) {
        lis->final->next = nou;
    }
    else {
        lis->inceput = nou;
    }
    lis->final = nou;
}

void adaugaLaInceputInLista(Lista* lis, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next = lis->inceput;
    nou->prev = NULL;
    if (lis->inceput) {
        lis->inceput->prev = nou;
    }
    else {
        lis->final = nou;
    }
    lis->inceput = nou;
}

Lista citireLDMasiniDinFisier(const char* numeFisier) {

    FILE* f = fopen(numeFisier, "r");
    Lista lis;
    lis.inceput = NULL;
    lis.final = NULL;

    while (!feof(f)) {
        Masina m;
        m = citireMasinaDinFisier(f);
        adaugaMasinaInLista(&lis, m);
    }
    fclose(f);

    return lis;
}


void dezalocareLDMasini(Lista* lis) {
    while (lis->inceput) {
        Nod* p = lis->inceput;
        lis->inceput = p->next;
        free(p->info.model);
        free(p->info.numeSofer);
        free(p);
    }
    lis->final = NULL;
}

float calculeazaPretMediu(Lista lis) {
    float sum = 0;
    int contor = 0;
    Nod* temp = lis.inceput;
    while (temp) {
        sum += temp->info.pret;
        contor++;
        temp = temp->next;
    }
    return (contor > 0) ? sum / contor : 0;
}

void stergeMasinaDupaID(Lista* lis, int id) {
    Nod* p = lis->inceput;
    while (p && p->info.id != id) {
        p = p->next;
    }
    if (p->prev) {
        p->prev->next = p->next;
        if (p->next) {
            p->next->prev = p->prev;
        }
        else {
            lis->final = p->prev;
        }
        free(p->info.model);
        free(p->info.numeSofer);
        free(p);
    }
    else {
        lis->inceput = p->next;
        if (p->next) {
            p->next->prev = NULL;
        }
        else
        {
            lis->final = NULL;
        }
        free(p->info.model);
        free(p->info.numeSofer);
        free(p);
    }

}


char* getNumeSoferMasinaScumpa(Lista lis) {
    if (!lis.inceput) return NULL;

    Nod* p = lis.inceput;
    Nod* maxPretMasina = p;
    while (p) {
        if (p->info.pret > maxPretMasina->info.pret) {
            maxPretMasina = p;
        }
        p = p->next;
    }

    char* nume = malloc(strlen(maxPretMasina->info.numeSofer) + 1);
    strcpy(nume, maxPretMasina->info.numeSofer);
    return nume;
}

int main() {
    Lista lis = citireLDMasiniDinFisier("masini.txt");

    printf("Lista masini:\n");
    afisareListaMasini(lis);

    printf("Lista masini de la final:\n");
    afisareListaMasiniDeLaFinal(lis);

    float medie = calculeazaPretMediu(lis);
    printf("Pret mediu: %.2f\n", medie);

    stergeMasinaDupaID(&lis, 1);

    char* nume = getNumeSoferMasinaScumpa(lis);
    if (nume) {
        printf("Numele soferului celei mai scumpe masini: %s\n", nume);
        free(nume);
    }

    dezalocareLDMasini(&lis);
    return 0;
}