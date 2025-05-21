#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct consultatie {
    char* data_consultatie;
    char* nume_medic;
    char* specialitate;
    int pret;
} consultatie;

typedef struct nodLs {
    struct consultatie info;
    struct nodLs* next;
} nodLs;

typedef struct nodLp {
    nodLs* info;
    struct nodLp* next;
} nodLp;

struct consultatie creareConsultatie(const char* data_consultatie, const char* nume_medic, const char* specialitate, int pret) {
    consultatie c;
    c.data_consultatie = (char*)malloc(strlen(data_consultatie) + 1);
    strcpy(c.data_consultatie, data_consultatie);

    c.nume_medic = (char*)malloc(strlen(nume_medic) + 1);
    strcpy(c.nume_medic, nume_medic);

    c.specialitate = (char*)malloc(strlen(specialitate) + 1);
    strcpy(c.specialitate, specialitate);

    c.pret = pret;

    return c;
}

nodLs* inserareLs(nodLs* cap, consultatie c) {
    nodLs* nou = (nodLs*)malloc(sizeof(nodLs));
    nou->info = creareConsultatie(c.data_consultatie, c.nume_medic, c.specialitate, c.pret);
    nou->next = NULL;

    if (cap == NULL) {
        return nou;
    }
    else {
        nodLs* aux = cap;
        while (aux->next) {
            aux = aux->next;
        }
        aux->next = nou;
        return cap;
    }
}

nodLp* inserareLp(nodLp* capLp, nodLs* capLs) {
    nodLp* nou = (nodLp*)malloc(sizeof(nodLp));
    nou->info = capLs;
    nou->next = NULL;

    if (capLp == NULL) {
        return nou;
    }
    else {
        nodLp* aux = capLp;
        while (aux->next) {
            aux = aux->next;
        }
        aux->next = nou;
        return capLp;
    }
}

// exercitiul 2
// Functia calculeaza valoarea totala a tuturor consultatiilor
// care apartin unei anumite specialitati 
int valoareTotala(nodLp* capLp, const char* Specialitate) {
    int Valoare = 0;
    nodLp* aux = capLp;
    while (aux) {
        nodLs* temp = aux->info;
        while (temp) {
            if (strcmp(temp->info.specialitate, Specialitate) == 0) {
                Valoare += temp->info.pret;
            }
            temp = temp->next;
        }
        aux = aux->next;
    }
    return Valoare;
}

// exercitiul 3
// Functia cauta toate consultatiile care au o anumita data 
// si modifica pretul acestora cu o valoare noua data ca parametru
void modificarePret(nodLp* capLp, const char* data, int pret_nou) {
    nodLp* aux = capLp;
    while (aux) {
        nodLs* temp = aux->info;
        while (temp) {
            if (strcmp(temp->info.data_consultatie, data) == 0) {
                temp->info.pret = pret_nou;
            }
            temp = temp->next;
        }
        aux = aux->next;
    }
}

// exercitiul 4
// Functia sterge prima consultatie gasita care are data egala cu cea primita ca parametru
void stergereConsultatie(nodLp** capLp, const char* data) {
    nodLp* aux = *capLp;
    while (aux) {
        nodLs* prev = NULL;
        nodLs* temp = aux->info;
        while (temp) {
            if (strcmp(temp->info.data_consultatie, data) == 0) {
                if (prev == NULL) {
                    aux->info = temp->next;
                }
                else {
                    prev->next = temp->next;
                }
                free(temp->info.data_consultatie);
                free(temp->info.nume_medic);
                free(temp->info.specialitate);
                free(temp);
                break;
            }
            prev = temp;
            temp = temp->next;
        }
        aux = aux->next;
    }
}

void afisareLs(nodLs* capLs) {
    nodLs* aux = capLs;
    while (aux) {
        printf("%s, %s, %s, %d\n", aux->info.data_consultatie, aux->info.nume_medic, aux->info.specialitate, aux->info.pret);
        aux = aux->next;
    }
}

void afisareLp(nodLp* capLp) {
    nodLp* aux = capLp;
    while (aux) {
        printf("Lista secundara este : \n");
        afisareLs(aux->info);
        aux = aux->next;
    }
}

void dezalocareLs(nodLs* capLs) {
    nodLs* aux = capLs;
    while (aux) {
        free(aux->info.data_consultatie);
        free(aux->info.nume_medic);
        free(aux->info.specialitate);
        nodLs* temp = aux->next;
        free(aux);
        aux = temp;
    }
}

void dezalocareLp(nodLp* capLp) {
    nodLp* aux = capLp;
    while (aux) {
        dezalocareLs(aux->info);
        nodLp* temp = aux->next;
        free(aux);
        aux = temp;
    }
}

int main() {
    consultatie c;
    nodLp* capLp = NULL;
    nodLs* capLs1 = NULL;
    nodLs* capLs2 = NULL;
    nodLs* capLs3 = NULL;

    int nr;
    char buffer[50];

    FILE* f = fopen("consultatii.txt", "r");

    fscanf(f, "%d", &nr);

    for (int i = 0; i < nr; i++) {
        fscanf(f, "%s", buffer);
        c.data_consultatie = (char*)malloc(strlen(buffer) + 1);
        strcpy(c.data_consultatie, buffer);

        fscanf(f, "%s", buffer);
        c.nume_medic = (char*)malloc(strlen(buffer) + 1);
        strcpy(c.nume_medic, buffer);

        fscanf(f, "%s", buffer);
        c.specialitate = (char*)malloc(strlen(buffer) + 1);
        strcpy(c.specialitate, buffer);

        fscanf(f, "%d", &c.pret);

        if (strcmp(c.specialitate, "Cardiologie") == 0) {
            capLs1 = inserareLs(capLs1, c);
        }
        else if (strcmp(c.specialitate, "Chirurgie") == 0) {
            capLs2 = inserareLs(capLs2, c);
        }
        else if (strcmp(c.specialitate, "Pediatrie") == 0) {
            capLs3 = inserareLs(capLs3, c);
        }


        free(c.data_consultatie);
        free(c.nume_medic);
        free(c.specialitate);
    }

    fclose(f);

    capLp = inserareLp(capLp, capLs1);
    capLp = inserareLp(capLp, capLs2);
    capLp = inserareLp(capLp, capLs3);

    afisareLp(capLp);

    // Ex 1
    printf("\nEx 1\n");
    printf("%d\n", valoareTotala(capLp, "Cardiologie"));

    // Ex 2
    printf("\nEx 2\n");
    modificarePret(capLp, "20231210", 450);
    afisareLp(capLp);

    // Ex 4
    printf("\nEx 4\n");
    stergereConsultatie(&capLp, "20231027");
    afisareLp(capLp);

    // Dezalocare 
    dezalocareLp(capLp);

    return 0;
}
