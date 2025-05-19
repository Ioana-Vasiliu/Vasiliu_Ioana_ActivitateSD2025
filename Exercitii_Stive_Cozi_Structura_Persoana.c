#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//CERINTE
//1. Sa se construiasca si sa se afiseze o structura de tip coada ale carei informatii utile sunt articole de tip "persoana", avand atribute precum varsta (int), nume (char*), precum si alte doua campuri la alegere. Informatiile aferete nodurilor sunt citite dintr-un fisier text
//2. Se considera o a doua coada de persoane cu date preluate de la consola.Sa se scrie functia pentru impletirea a doua cozi ale caror informatii utile sunt articole de tip "persoana" si sa se salveze intr - o a treia coada elementele structurii rezultata in urma impletirii.
//3. Sa se scrie functia pentru salvare din coada de articole de tip "persoana" (rezultata la pct 2) intr - o stiva de articole de tip "persoana" a persoanelor care indeplinesc un criteriu, la alegere.Sa se numere din stiva rezultata cate persoane au varsta peste un prag specificat ca parametru.
//4. Sa se dezaloce toate structurile de date dinamice construite.Operatia de dezalocare va fi insotita de afisarea continutului structurilor la consola inainte de dezalocarea memoriei heap.

// STRUCTURI
typedef struct Persoana {
    int varsta;
    char* nume;
    float inaltime;
    int nrPantof;
} Persoana;

typedef struct Nod {
    Persoana info;
    struct Nod* next;
} Nod;

typedef struct NodStiva {
    Persoana info;
    struct NodStiva* next;
} NodStiva;

// FUNCTII PERSOANA
Persoana crearePersoana(int varsta, const char* nume, float inaltime, int nrPantof) {
    Persoana p;
    p.varsta = varsta;
    p.nume = (char*)malloc((strlen(nume) + 1) * sizeof(char));
    strcpy(p.nume, nume);
    p.inaltime = inaltime;
    p.nrPantof = nrPantof;
    return p;
}

void afisarePersoana(Persoana p) {
    printf("%d, %s, %.2f, %d\n", p.varsta, p.nume, p.inaltime, p.nrPantof);
}

// COADA
void put(Nod** prim, Nod** ultim, Persoana p) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = crearePersoana(p.varsta, p.nume, p.inaltime, p.nrPantof);
    nou->next = NULL;
    if (*prim == NULL && *ultim == NULL) {
        *prim = nou;
        *ultim = nou;
    }
    else {
        (*ultim)->next = nou;
        *ultim = nou;
    }
}

int get(Nod** prim, Nod** ultim, Persoana* p) {
    if (*prim == NULL) return -1;
    *p = crearePersoana((*prim)->info.varsta, (*prim)->info.nume,
        (*prim)->info.inaltime, (*prim)->info.nrPantof);
    Nod* aux = *prim;
    *prim = (*prim)->next;
    if (*prim == NULL) *ultim = NULL;
    free(aux->info.nume);
    free(aux);
    return 0;
}

void afisareCoada(Nod* prim) {
    while (prim) {
        afisarePersoana(prim->info);
        prim = prim->next;
    }
}

// CITIRE DIN FISIER
Persoana citirePersoanaDinBuffer(char* buffer) {
    char sep[] = ",\n";
    char* token = strtok(buffer, sep);
    Persoana p;

    p.varsta = atoi(token);
    token = strtok(NULL, sep);
    p.nume = malloc(strlen(token) + 1);
    strcpy(p.nume, token);
    token = strtok(NULL, sep);
    p.inaltime = atof(token);
    token = strtok(NULL, sep);
    p.nrPantof = atoi(token);

    return p;
}

void inserarePersoaneDinFisier(const char* numeFisier, Nod** prim, Nod** ultim) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        perror("Eroare la deschiderea fisierului");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), f)) {
        if (strlen(buffer) < 3) continue;
        Persoana p = citirePersoanaDinBuffer(buffer);
        put(prim, ultim, p);
    }
    fclose(f);
}

// CITIRE DE LA CONSOLA
void citireCoadaDeLaConsola(Nod** prim, Nod** ultim, int n) {
    int varsta, nrPantof;
    char buffer[256];
    float inaltime;

    for (int i = 0; i < n; i++) {
        printf("Persoana %d:\n", i + 1);
        printf("Varsta: ");
        scanf("%d", &varsta);
        printf("Nume: ");
        scanf("%255s", buffer);
        printf("Inaltime: ");
        scanf("%f", &inaltime);
        printf("Numar pantof: ");
        scanf("%d", &nrPantof);
        Persoana p = crearePersoana(varsta, buffer, inaltime, nrPantof);
        put(prim, ultim, p);
    }
}

// IMPLETIRE COZI
void impletireCozi(Nod* prim1, Nod* ultim1, Nod* prim2, Nod* ultim2, Nod** prim3, Nod** ultim3) {
    Persoana p;
    while (prim1 || prim2) {
        if (prim1) {
            get(&prim1, &ultim1, &p);
            put(prim3, ultim3, p);
            free(p.nume);
        }
        if (prim2) {
            get(&prim2, &ultim2, &p);
            put(prim3, ultim3, p);
            free(p.nume);
        }
    }
}

// STIVA
void push(NodStiva** varf, Persoana p) {
    NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
    nou->info = crearePersoana(p.varsta, p.nume, p.inaltime, p.nrPantof);
    nou->next = *varf;
    *varf = nou;
}

NodStiva* salvareInStivaDupaCriteriu(Nod* coada) {
    NodStiva* varf = NULL;
    while (coada) {
        if (coada->info.nrPantof > 35) {
            push(&varf, coada->info);
        }
        coada = coada->next;
    }
    return varf;
}

int numarPersoanePestePrag(NodStiva* varf, int prag) {
    int count = 0;
    while (varf) {
        if (varf->info.varsta > prag) {
            count++;
        }
        varf = varf->next;
    }
    return count;
}

void afisareStiva(NodStiva* varf) {
    while (varf) {
        afisarePersoana(varf->info);
        varf = varf->next;
    }
}

// DEALOCARI
void dezalocareCoada(Nod** prim, Nod** ultim) {
    Persoana p;
    while (get(prim, ultim, &p) == 0) {
        free(p.nume);
    }
}

void dezalocareStiva(NodStiva** varf) {
    while (*varf) {
        NodStiva* temp = *varf;
        *varf = (*varf)->next;
        free(temp->info.nume);
        free(temp);
    }
}

// MAIN
int main() {
    Nod* prim = NULL, * ultim = NULL;
    inserarePersoaneDinFisier("persoane.txt", &prim, &ultim);

    printf("Afisare coada din fisier:\n");
    afisareCoada(prim);

    Nod* prim2 = NULL, * ultim2 = NULL;
    int n;
    printf("\nNr persoane noi: ");
    scanf("%d", &n);
    citireCoadaDeLaConsola(&prim2, &ultim2, n);

    printf("\nAfisare coada de la consola:\n");
    afisareCoada(prim2);

    printf("\nCoada Impletita:\n");
    Nod* prim3 = NULL, * ultim3 = NULL;
    impletireCozi(prim, ultim, prim2, ultim2, &prim3, &ultim3);
    afisareCoada(prim3);

    printf("\nAfisare stiva:\n");
    NodStiva* varf = salvareInStivaDupaCriteriu(prim3);
    afisareStiva(varf);

    int prag;
    printf("\nIntrodu pragul de varsta: ");
    scanf("%d", &prag);
    int rezultat = numarPersoanePestePrag(varf, prag);
    printf("Nr persoane cu varsta peste %d: %d\n", prag, rezultat);

    dezalocareCoada(&prim3, &ultim3);
    dezalocareStiva(&varf);

    return 0;
}
