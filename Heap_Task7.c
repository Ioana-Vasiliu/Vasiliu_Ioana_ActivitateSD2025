#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Pacient {
    unsigned int id;
    char* nume;
    char* diagnostic;
    unsigned int varsta;
    unsigned int grad; 
} Pacient;

Pacient initializarePacient(unsigned int id, const char* nume, const char* diagnostic, unsigned int varsta, unsigned int grad) {
    Pacient p;
    p.id = id;
    p.nume = (char*)malloc(strlen(nume) + 1);
    strcpy(p.nume, nume);
    p.diagnostic = (char*)malloc(strlen(diagnostic) + 1);
    strcpy(p.diagnostic, diagnostic);
    p.varsta = varsta;
    p.grad = grad;
    return p;
}

typedef struct Heap {
    int dim;
    Pacient* vector;
} Heap;

void filtrare(Heap h, int index) {
    int indexRad = index;
    int indexSt = 2 * index + 1;
    int indexDr = 2 * index + 2;

    if (indexSt < h.dim && h.vector[indexSt].grad > h.vector[indexRad].grad) {
        indexRad = indexSt;
    }
    if (indexDr < h.dim && h.vector[indexDr].grad > h.vector[indexRad].grad) {
        indexRad = indexDr;
    }

    if (index != indexRad) {
        Pacient aux = h.vector[indexRad];
        h.vector[indexRad] = h.vector[index];
        h.vector[index] = aux;
        filtrare(h, indexRad);
    }
}

Heap adaugare(Heap h, Pacient p) {
    Pacient* copieVector = (Pacient*)malloc(sizeof(Pacient) * (h.dim + 1));
    for (int i = 0; i < h.dim; i++) {
        copieVector[i] = h.vector[i];
    }

    copieVector[h.dim] = p;
    free(h.vector);
    h.dim++;
    h.vector = copieVector;

    for (int i = (h.dim / 2) - 1; i >= 0; i--) {
        filtrare(h, i);
    }

    return h;
}

void afisareHeap(Heap h) {
    for (int i = 0; i < h.dim; i++) {
        printf("ID: %u, Nume: %s, Diagnostic: %s, Varsta: %u, Grad Urgenta: %u\n",
            h.vector[i].id, h.vector[i].nume, h.vector[i].diagnostic, h.vector[i].varsta, h.vector[i].grad);
    }
}
Heap extragePacientUrgent(Heap h, Pacient* p) {
    *p = h.vector[0];
    h.vector[0] = h.vector[h.dim - 1];
    Pacient* copieVector = (Pacient*)malloc(sizeof(Pacient) * (h.dim - 1));
    for (int i = 0; i < h.dim - 1; i++) {
        copieVector[i] = h.vector[i];
    }
    free(h.vector);
    h.dim--;
    h.vector = copieVector;

    for (int i = (h.dim / 2) - 1; i >= 0; i--) {
        filtrare(h, i);
    }

    return h;
}


void dezalocareHeap(Heap* h) {
    if (h->vector) {
        for (int i = 0; i < h->dim; i++) {
            free(h->vector[i].nume);
            free(h->vector[i].diagnostic);
        }
        free(h->vector);
        h->vector = NULL;
        h->dim = 0;
    }
}


void main() {
    Heap h;
    Pacient p;
    h.vector = NULL;
    h.dim = 0;

    char buffer[100];
    char sep[] = ",\n";
    char* token;

    FILE* f = fopen("pacienti.txt", "r");
    if (f) {
        while (fgets(buffer, 100, f)) {
            token = strtok(buffer, sep);
            p.id = atoi(token);

            token = strtok(NULL, sep);
            p.nume = (char*)malloc(strlen(token) + 1);
            strcpy(p.nume, token);

            token = strtok(NULL, sep);
            p.diagnostic = (char*)malloc(strlen(token) + 1);
            strcpy(p.diagnostic, token);

            token = strtok(NULL, sep);
            p.varsta = atoi(token);

            token = strtok(NULL, sep);
            p.grad = atoi(token);

            h = adaugare(h, p);
        }
        fclose(f);
    }

    printf("--- Pacientii in functie de gradul de urgenta : ---\n");

    while (h.dim > 0) {
        h = extragePacientUrgent(h, &p);
        printf("ID: %u, Nume: %s, Diagnostic: %s, Varsta: %u, Grad Urgenta: %u\n",
            p.id, p.nume, p.diagnostic, p.varsta, p.grad);
    }

    dezalocareHeap(&h);
}
