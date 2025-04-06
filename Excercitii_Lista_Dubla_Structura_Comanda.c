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
	char buffer[256];
	char sep[] = ",\n";
	char* token;

	fgets(buffer, sizeof(buffer), file);
	Comanda c;
	token = strtok(buffer, sep);
	c.id = atoi(token);

	// dataLivrare
	token = strtok(NULL, sep);
	c.data_livarare = malloc(strlen(token) + 1);
	strcpy(c.data_livarare, token);

	// numeClient
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

//functia ce determina numarul de comenzi cu data livrare <= 20.04.2025
int nrComenziDataLivrare(LD lista, const char* dataPrag) {
	int nrComenziPrag = 0;
	Nod* aux = lista.inceput;

	while (aux) {
		if (strcmp(aux->info.data_livarare,dataPrag) <=0) {
			nrComenziPrag++;
		}
		aux = aux->next;
	}
	return nrComenziPrag;
}

//functia ce modifica data livrare pentru comenzi. Comenzile sunt identificate prin nume client
void actualizareDataLivrare(LD lista, const char* dataNoua, const char* numeClient) {
	Nod* aux = lista.inceput;

	printf("\n--- Traversare INAINTE de modificare ---\n");
	while (aux) {
		afisareComanda(aux->info);
		aux = aux->next;
	}

	aux = lista.inceput;
	while (aux) {
		if (strcmp(aux->info.nume_client, numeClient) == 0) {
			aux->info.data_livarare = (char*)malloc(strlen(dataNoua) + 1);
			strcpy(aux->info.data_livarare, dataNoua);
		}
		aux = aux->next;
	}

	printf("\n--- Traversare INVERS dupa modificare ---\n");
	aux = lista.final;
	while (aux) {
		afisareComanda(aux->info);
		aux = aux->prev;
	}
}

//conversie la vector
int contorComenziDupaNume(LD lista, const char* numeCLient) {
	int contor = 0;
	Nod* aux = lista.inceput;

	while (aux) {
		if (strcmp(aux->info.nume_client, numeCLient) == 0) {
			contor++;
		}
		aux = aux->next;
	}
	return contor;

}


Comanda* conversieVector(LD lista, const char* numeClient, int* dimensiune) {
	*dimensiune = contorComenziDupaNume(lista, numeClient);
	Comanda* vector = (Comanda*)malloc((*dimensiune) * sizeof(Comanda));
	int i = 0;
	Nod* aux = lista.inceput;

	while (aux) {
		if (strcmp(aux->info.nume_client, numeClient) == 0) {
			vector[i] = creareComanda(aux->info.id, aux->info.data_livarare, aux->info.nume_client);
			i++;
		}
		aux = aux->next;
	}
	return vector;
}

void afisareVector(Comanda* vector, int dim) {
	for(int i = 0;i < dim;i++) {
		afisareComanda(vector[i]);
		printf("------");
	}
}


void dezalocareVector(Comanda* vector, int dim) {
	for (int i = 0;i < dim;i++) {
		free(vector[i].data_livarare);
		free(vector[i].nume_client);

	}
	free(vector);
}

//stergerea unui nod dupa o pozitie data ca parametru
void stergereDupaPozitie(LD* lista, int pozitie) {
	int index = 0;

	Nod* aux = lista->inceput;

	while (aux && index < pozitie) {
		aux = aux->next;
		index++;
	}
	if (!aux) return;

	if (aux->prev) {
		aux->prev->next = aux->next;
	}
	else {
		lista->inceput = aux->next;
	}

	if (aux->next) {
		aux->next->prev = aux->prev;
	}
	else {
		lista->final = aux->prev;
	}

	free(aux->info.data_livarare);
	free(aux->info.nume_client);
	free(aux);
}

//stergerea unui nod dupa prima aparitie a unui nume dat ca parametru
void stergereDupaNume(LD* lista, const char* numeClient) {
	int index = 0;

	Nod* aux = lista->inceput;

	while (aux) {
		if (strcmp(aux->info.nume_client, numeClient) == 0) {
			if (aux->prev) {
				aux->prev->next = aux->next;
			}
			else {
				lista->inceput = aux->next;
			}

			if (aux->next) {
				aux->next->prev = aux->prev;
			}
			else {
				lista->final = aux->prev;
			}
			free(aux->info.data_livarare);
			free(aux->info.nume_client);
			free(aux);
			return;
		}
		aux = aux->next;

	}
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
	printf("Nr. comenzi pentru Darius: %d\n", calculeazaNrComenzi(lista, "Darius"));

	printf("---CERINTA 2---\n");
	printf("\nNr comenzi cu data <= 20.04.2025: %d\n", nrComenziDataLivrare(lista, "20.04.2025"));

	printf("---CERINTA 3-\n");
	actualizareDataLivrare(lista, "15.05.2025", "Darius");

	printf("---CERINTA 4-\n");
	int dimVector = 4;
	Comanda* vectorComenzi = conversieVector(lista, "Darius", &dimVector);

	printf("\n---Comezile clientului 'Darius' in vector---\n");
	afisareVector(vectorComenzi, dimVector);

	printf("---CERINTA 5-\n");
	printf("\n---Afisarea listei fara prima pozitie (clinetul ar fi 'Maria'---\n");
	stergereDupaPozitie(&lista, 1);
	afisareLista(lista);

	printf("---CERINTA 6-\n");
	stergereDupaNume(&lista, "Ioana");
	printf("\n---Afisarea listei fara clientul cu numele 'Ioana'---\n");
	afisareLista(lista);


	dezalocareLD(&lista);
	dezalocareVector(vectorComenzi, dimVector);

	return 0;
}