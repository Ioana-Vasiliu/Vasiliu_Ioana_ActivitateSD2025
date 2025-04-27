#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Serviciu {
	unsigned int id;
	char* tipServiciu;
	char* numeClient;
	float sumaPlata;
	unsigned int timpEstimat;
}Serviciu;

typedef struct Nod {
	Serviciu info;
	struct Nod* next;
}Nod;

typedef struct HashTable {
	int dim;
	Nod** vector;
}HashTable;

Serviciu creareComanda(unsigned int id, const char* tipServiciu, const char* numeClient, float sumaPlata, unsigned int timpEstimat) {
	Serviciu s;
	s.id = id;
	s.tipServiciu = (char*)malloc(strlen(tipServiciu) + 1);
	strcpy(s.tipServiciu, tipServiciu);
	s.numeClient = (char*)malloc(strlen(numeClient) + 1);
	strcpy(s.numeClient, numeClient);
	s.sumaPlata = sumaPlata;
	s.timpEstimat = timpEstimat;

	return s;
};

void afisareServiciu(Serviciu s) {
	printf("ID : %u || tip serviciu : %s || nume client : %s || suma plata : %.2f || timp estimat %u \n", s.id, s.tipServiciu, s.numeClient, s.sumaPlata, s.timpEstimat);
}

void afisareLista(Nod* cap) {
	while (cap) {
		afisareServiciu(cap->info);
		cap = cap->next;
	}
}

void adaugaInLista(Nod** cap, Serviciu s) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = creareComanda(s.id, s.tipServiciu, s.numeClient, s.sumaPlata, s.timpEstimat);
	nou->next = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		Nod* temp = *cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

//initializare hash
HashTable initializareHash(int dim) {
	HashTable ht;
	ht.dim = dim;
	ht.vector = malloc(sizeof(Nod*) * dim);
	for (int i = 0;i < dim;i++) {
		ht.vector[i] = NULL;
	}
	return ht;
}

//functie hash
int functieHash(const char* numeClient, int dim) {
	int suma = 0;
	for (int i = 0;i < strlen(numeClient);i++) {
		suma += numeClient[i];
	}
	return suma % dim;
}

//inserare in tabela
void inserareInTabela(HashTable* ht, Serviciu s) {
	int poz = functieHash(s.numeClient, ht->dim);
	adaugaInLista(&ht->vector[poz], s);
}

//afisare tabela
void afisareTabela(HashTable ht) {
	for (int i = 0;i < ht.dim;i++) {
		if (ht.vector[i]) {
			printf("\n CLuster : %d : \n", i);
			afisareLista(ht.vector[i]);
		}
	}
}

//citire fisier
Serviciu citireServicuDinFisier(FILE* f) {
	char buffer[128];
	char sep[] = ",\n";
	char* token;

	fgets(buffer, 128, f);
	Serviciu s;
	
	token = strtok(buffer, sep);
	s.id = atoi(token);

	token = strtok(NULL, sep);
	s.tipServiciu = malloc(strlen(token) + 1);
	strcpy(s.tipServiciu, token);

	token = strtok(NULL, sep);
	s.numeClient = malloc(strlen(token) + 1);
	strcpy(s.numeClient, token);

	token = strtok(NULL, sep);
	s.sumaPlata = atof(token);

	token = strtok(NULL, sep);
	s.timpEstimat = atoi(token);

	return s;
}

HashTable citireServiciiDinFisier(const char* numeFisier, int dim) {
	FILE* f = fopen(numeFisier, "r");
	HashTable ht = initializareHash(dim);
	while (!feof(f)) {
		Serviciu s = citireServicuDinFisier(f);
		inserareInTabela(&ht, s);
	}
	fclose(f);
	return ht;
}

//functia care determina nr de servicii care au fost cerute de un client specifiat ca parametru
int calculeazaNrServicii(HashTable ht, const char* numeClient) {
	int contor = 0;
	int poz = functieHash(numeClient,ht.dim);
	Nod* aux = ht.vector[poz];

	while (aux) {
		if (strcmp(aux->info.numeClient, numeClient) == 0) {
			contor++;
		}
		aux = aux->next;
	}
	return contor;
}

//functia care sterge un serviciu din tabela. serviciul sters va fi ultimul
void stergereServiciu(HashTable* ht) {
	if (ht->vector) {
		for (int i = 0;i < ht->dim;i++) {
			if (ht->vector[i]) {
				Nod* aux = ht->vector[i];
				Nod* prev = aux;
				if (aux->next == NULL) {
					ht->vector[i] = NULL;
				}
				else {
					while (aux->next) {
						prev = aux;
						aux = aux->next;
						

					}
				}
					free(aux->info.tipServiciu);
					free(aux->info.numeClient);
					free(aux);
					prev->next = NULL;
			}
		}
	}
}

void dezalocareTabela(HashTable* ht) {
	for (int i = 0;i < ht->dim;i++) {
		Nod* aux = ht->vector[i];

		while (aux) {
			Nod* temp = aux;
			aux = aux->next;
			free(temp->info.tipServiciu);
			free(temp->info.numeClient);
		}
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

int main() {
	HashTable tabela = citireServiciiDinFisier("servicii.txt", 27);
	afisareTabela(tabela);

	printf("\nNumar servicii pentru clienta 'Mihaela' : %d\n", calculeazaNrServicii(tabela, "Mihaela"));

	printf("\n---AFISARE INAINTE DE STERGERE---\n");
	afisareTabela(tabela);

	printf("\n---AFISARE DUPA STERGEREA ULTIMULUI ELEMENT---");
	stergereServiciu(&tabela);
	afisareTabela(tabela);


	return 0;
}