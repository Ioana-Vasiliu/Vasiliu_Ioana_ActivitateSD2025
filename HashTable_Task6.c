#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structura Cladire
typedef struct {
    int id;
    int anConstructie;
    char* nume;
    char* adresa;
} Cladire;


typedef struct Nod {
    Cladire info;
    struct Nod* next;
} Nod;

// Tabela hash
typedef struct {
    int dim;
    Nod** vector;
} HashTable;

// Functie hash
int functieHash(int an, int dim) {
    return an % dim;
}

// Initializare tabela
HashTable initializareTabela(int dim) {
    HashTable tabela;
    tabela.dim = dim;
    tabela.vector = (Nod**)malloc(dim * sizeof(Nod*));
    for (int i = 0; i < dim; i++)
        tabela.vector[i] = NULL;
    return tabela;
}

void inserareCladire(HashTable* tabela, Cladire c) {
    int poz = functieHash(c.anConstructie, tabela->dim);
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = c;
    nou->next = tabela->vector[poz];
    tabela->vector[poz] = nou;
}


void afisareCladire(Cladire c) {
    printf("ID: %d | An: %d | Nume: %s | Adresa: %s\n", c.id, c.anConstructie, c.nume, c.adresa);
}


void afisareTabela(HashTable tabela) {
    for (int i = 0; i < tabela.dim; i++) {
        printf("Cluster %d:\n", i);
        Nod* p = tabela.vector[i];
        while (p) {
            afisareCladire(p->info);
            p = p->next;
        }
    }
}

// 2. Afisare clustere dintr-un an
void afisareCladiriAn(HashTable tabela, int an) {
    int poz = functieHash(an, tabela.dim);
    Nod* p = tabela.vector[poz];
    while (p) {
        if (p->info.anConstructie == an)
            afisareCladire(p->info);
        p = p->next;
    }
}

// 3. Stergere dupa ID si an
void stergereCladireIDAn(HashTable* tabela, int id, int an) {
    int poz = functieHash(an, tabela->dim);
    Nod* p = tabela->vector[poz], * prev = NULL;
    while (p) {
        if (p->info.id == id && p->info.anConstructie == an) {
            if (prev)
                prev->next = p->next;
            else
                tabela->vector[poz] = p->next;
            free(p->info.nume);
            free(p->info.adresa);
            free(p);
            return;
        }
        prev = p;
        p = p->next;
    }
}

// 4. Stergere doar dupa ID
void stergereCladireID(HashTable* tabela, int id) {
    for (int i = 0; i < tabela->dim; i++) {
        Nod* p = tabela->vector[i], * prev = NULL;
        while (p) {
            if (p->info.id == id) {
                if (prev)
                    prev->next = p->next;
                else
                    tabela->vector[i] = p->next;
                free(p->info.nume);
                free(p->info.adresa);
                free(p);
                return;
            }
            prev = p;
            p = p->next;
        }
    }
}

// 6. Deep copy in vector
Cladire* cladiriDinAn(HashTable tabela, int an, int* nr) {
    *nr = 0;
    int poz = functieHash(an, tabela.dim);
    Nod* p = tabela.vector[poz];
    Nod* aux = p;
    while (aux) {
        if (aux->info.anConstructie == an)
            (*nr)++;
        aux = aux->next;
    }
    if (*nr == 0) return NULL;
    Cladire* vector = (Cladire*)malloc(*nr * sizeof(Cladire));
    int k = 0;
    while (p) {
        if (p->info.anConstructie == an) {
            vector[k].id = p->info.id;
            vector[k].anConstructie = an;
            vector[k].nume = _strdup(p->info.nume);
            vector[k].adresa = _strdup(p->info.adresa);
            k++;
        }
        p = p->next;
    }
    return vector;
}

// 7. Modificare an construire dupa ID si vechiul an
void modificareAnCladire(HashTable* tabela, int id, int AnulVechi, int AnulNou) {
    int poz = functieHash(AnulVechi, tabela->dim);
    Nod* p = tabela->vector[poz], * prev = NULL;
    while (p) {
        if (p->info.id == id && p->info.anConstructie == AnulVechi) {
            Cladire modificata = p->info;
            modificata.anConstructie = AnulNou;
            if (prev)
                prev->next = p->next;
            else
                tabela->vector[poz] = p->next;
            free(p);
            inserareCladire(tabela, modificata);
            return;
        }
        prev = p;
        p = p->next;
    }
}



// Dezalocare tabela
void dezalocareTabela(HashTable* tabela) {
    for (int i = 0; i < tabela->dim; i++) {
        Nod* p = tabela->vector[i];
        while (p) {
            Nod* tmp = p;
            p = p->next;
            free(tmp->info.nume);
            free(tmp->info.adresa);
            free(tmp);
        }
    }
    free(tabela->vector);
}




int main() {
    HashTable tabela = initializareTabela(10);

    Cladire c1 = { 1, 1977, _strdup("Bloc A"), _strdup("Str. Victoriei") };
    Cladire c2 = { 2, 1980, _strdup("Bloc B"), _strdup("Bd. Unirii") };
    Cladire c3 = { 3, 1977, _strdup("Bloc C"), _strdup("Str. Libertatii") };
    Cladire c4 = { 4, 1990, _strdup("Bloc D"), _strdup("Str. Aviatorilor") };


    inserareCladire(&tabela, c1);
    inserareCladire(&tabela, c2);
    inserareCladire(&tabela, c3);
    inserareCladire(&tabela, c4);

    printf("--- Afisare Tabela ---\n");
    afisareTabela(tabela);

    printf("\n--- Cladiri din 1977 ---\n");
    afisareCladiriAn(tabela, 1977);

    printf("\n--- Stergere ID 3 si cu anul 1977 ---\n");
    stergereCladireIDAn(&tabela, 3, 1977);
    afisareCladiriAn(tabela, 1977);

    printf("\n--- Stergere ID 2 ---\n");
    stergereCladireID(&tabela, 2);
    afisareTabela(tabela);

    int nr = 0;
    Cladire* vector = cladiriDinAn(tabela, 1977, &nr);
    printf("\n--- Vectorul cu cladirile din 1977 \n", nr);
    for (int i = 0; i < nr; i++)
        afisareCladire(vector[i]);

    for (int i = 0; i < nr; i++) {
        free(vector[i].nume);
        free(vector[i].adresa);
    }
    free(vector);

    printf("\n--- Modificare an Cladire ID 4 din 1990 in 2000 ---\n");
    modificareAnCladire(&tabela, 4, 1990, 2000);
    afisareTabela(tabela);

    dezalocareTabela(&tabela);
    return 0;
}