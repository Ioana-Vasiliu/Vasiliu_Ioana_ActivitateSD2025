#define _CRT_SECURE_NO_WARNINGS
#include<string.h>
#include<stdio.h>
#include<stdlib.h>


typedef struct Comanda {
	unsigned int id;
	char* data_livarare;
	char* nume_client;
}Comanda;

//definim nod
typedef struct Nod{
	Comanda info;
	struct Nod* prev;
	struct Nod* next;
}Nod;

//definim lista dubla
typedef struct LD {
	Nod* inceput;
	Nod* final;
}LD;

//functia de creare

Comanda creareComanda(unsigned int id, const char* data_livarare, const char* nume_client) {
	Comanda c;
	c.id = id;
	c.data_livarare = (char*)malloc(strlen(data_livarare) + 1);
	strcpy_s(c.data_livarare, strlen(data_livarare) + 1, data_livarare);

	c.nume_client = (char*)malloc(strlen(nume_client) + 1);
	strcpy_s(c.nume_client, strlen(nume_client) + 1, nume_client);

	return c;
}

//afisare comanda
void afisareComanda(Comanda c) {
	printf("Id-ul este : %d\n", c.id);
	printf("Data livrarii este : %s\n", c.data_livarare);
	printf("Numele clientului este : %s\n", c.nume_client);
}

//afisare lista de comenzi
void afisareLista(LD lista) {
	Nod* aux = lista.inceput;
	while (aux) {
		afisareComanda(aux->info);
		aux = aux->next;
	}
}

//inserare in lista (la final)
void inserareFinal(LD* lista, Comanda c) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = creareComanda(c.id, c.data_livarare, c.nume_client);
	nou->next = NULL;
	nou->prev = lista->final;

	if (lista->final) {
		lista->final->next = nou;
	}
	else {
		lista->inceput = nou;
	}
	lista->final = nou;
}

//inserare inceput

void inserareInceput(LD* lista, Comanda c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = creareComanda(c.id, c.data_livarare, c.nume_client);
	nou->prev = NULL;
	nou->next = lista->inceput;

	if (lista->inceput) {
		lista->inceput->prev = nou;
	}
	else {
		lista->final = nou;
	}
	lista->inceput = nou;
}

//citire comenda din fisier

Comanda citireComandaFisier(FILE* file) {
	char buffer[250];
	char sep[] = ", \n";
	char* token;

	fgets(buffer, sizeof(buffer), file);
	Comanda c;
	token = strtok(buffer, sep);
	c.id = atoi(token);

	token = strtok(NULL, sep);
	c.data_livarare = malloc(strlen(token) + 1);
	strcpy(c.data_livarare, token);

	token = strtok(NULL, sep);
	c.nume_client = malloc(strlen(token) + 1);
	strcpy(c.nume_client, token);

	return c;

}

//citire lista dubla din fisier
LD citireLDFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	LD lista;
	lista.inceput = NULL;
	lista.final = NULL;

	while (!feof(f)) {
		Comanda c = citireComandaFisier(f);
		inserareFinal(&lista, c);
	}
	fclose(f);
	return lista;
}

//calcul nr comenzi, avand nume client ca parametru
int calculeazaNrComenzi(LD lista,const char* numeClient) {
	int nrComenzi = 0;
	Nod* aux = lista.inceput;

	while (aux) {
		if (strcmp(aux->info.nume_client, numeClient) == 0) {
			nrComenzi++;
		}
		aux = aux->next;
	}
	return nrComenzi;

}
//dezalocare

void dezalocareLD(LD* lista) {
	while (lista->inceput) {
		Nod* aux = lista->inceput;
		lista->inceput = aux->next;
		free(aux->info.data_livarare);
		free(aux->info.nume_client);
		free(aux);
	}
	lista->final = NULL;
}

int main() {
	LD lista = citireLDFisier("comenzi.txt");
	printf("Lista initiala: \n");
	afisareLista(lista);

	printf("---CERINTA 1---\n");
	printf("\nComenzi Darius\n");
	printf("Nr. comenzi pentru Darius: %d\n", calculeazaNrComenzi(lista, "Darius"));

	dezalocareLD(&lista);

	return 0;
}