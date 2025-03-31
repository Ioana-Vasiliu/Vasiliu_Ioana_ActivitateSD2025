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


//functia de sterge un nod pentru o anumita pozitie 
void stergeNodPePozitie(Lista* lis, int pozitie) {
    Nod* p = lis->inceput;
    int contor = 0;

    while (p && contor < pozitie) {
        p = p->next;
        contor++;
    }

    if (!p) { 
        return;
    }

    if (p->prev) {
        p->prev->next = p->next;
        if (p->next) {
            p->next->prev = p->prev;
        }
        else {
            lis->final = p->prev;
        }
    }
    else { 
        lis->inceput = p->next;
        if (p->next) {
            p->next->prev = NULL;
        }
        else {
            lis->final = NULL;
        }
    }

    free(p->info.model);
    free(p->info.numeSofer);
    free(p);
}


int main() {
    Lista lis = citireLDMasiniDinFisier("masini.txt");

    printf("-----Lista masini inainte de stergere dupa ID:------\n");
    afisareListaMasini(lis);

    //apel pentru stergere dupa pozitie
    printf("\n---Stergem masina de pe pozitia 2-----\n");
    stergeNodPePozitie(&lis, 2);
    afisareListaMasini(lis);


    dezalocareLDMasini(&lis);
    return 0;
}