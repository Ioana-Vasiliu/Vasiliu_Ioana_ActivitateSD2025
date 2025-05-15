//la stiva / cozi putem avea orice structura de date liniara, astfel incat sa respecte regule fifo (coasa) / lifo (stiva) (lista dubla, simpla sau vector)
// pt lista simpla,daca facem inserare la inceput, la coada facem extragere de la final (sau invers)
// la stiva, daca inseram la final, extragem de la final (sau invers), dar este recomandat la inceput ca sa nu mai trebuiasca sa parcurgem stiva.
//la coada inserarea se numeste enqueue, extragerea dequeue
//la stiva, inserarea se numeste push, extragerea pop
//la pop /  dequeue, cand se aplica, elementul se elimina.
//la stiva si coada nu putem parcurge lista. pt parcurgere, trebuiesc extrase unul cate unul (prin pop / dequeue)
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
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

//STACK
//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
// //putem reprezenta o stiva prin LSI, LDI sau vector
//reprezentam prin lista simpla inlantuita.
//!daca avem de ales intre lista simpla si vector, sa alegem mereu lista pt ca e mai usor

typedef struct Nod Nod;
struct Nod {
	Masina info;
	Nod* next;

};

void pushStack(Nod** stiva, Masina masina) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = *stiva;
	*stiva = nou;
}

Masina popStack(Nod** stiva) {
	Masina m;
	m.id = -1;
	if ((*stiva) != NULL) {
		m = (*stiva)->info;
		Nod* prev = (*stiva);
		*stiva = (*stiva)->next;
		free(prev);
	}
	return m;
}


unsigned char* emptyStack(Nod* stiva) { //ne spune daca stiva este goala sau nu
	return stiva == NULL;
}

Nod* citireStackMasiniDinFisier(const char* numeFisier) {
	Nod* stiva = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		pushStack(&stiva, m);
	}
	fclose(f);
	return stiva;
}

void dezalocareStivaDeMasini(Nod** stiva) {
	while ((*stiva) != NULL) {
		Masina m = popStack(stiva);
		free(m.numeSofer);
		free(m.model);
	}
}

int size(Nod* stiva) {
	int nr = 0;
	while (stiva != NULL) {
		nr++;
		stiva = stiva->next;
	}
	return nr;
}

//QUEUE
//Alegeti prin ce veti reprezenta coada si creati structura necesara acestei cozi
//putem reprezenta o coada prin LSI, LDI sau vector

typedef struct NodDublu NodDublu;
typedef struct ListaDubla ListaDubla;
struct NodDublu {
	Masina info;
	NodDublu* next;
	NodDublu* prev;
};

struct ListaDubla {
	NodDublu* prim;
	NodDublu* ultim;

};

void enqueue(ListaDubla* coada, Masina masina) {
	NodDublu* nou = (NodDublu*)malloc(sizeof(NodDublu));
	nou->info = masina;
	nou->prev = NULL;
	nou->next = NULL;
	if (coada->ultim != NULL) {
		coada->ultim->next = nou;

	}
	else {
		coada->prim = nou;
	}
	coada->ultim = nou;
}

Masina dequeue(ListaDubla* lista) {
	//extrage o masina din coada
	Masina m;
	m.id = -1;
	if (lista->ultim != NULL) {
		m = lista->prim->info;
		NodDublu* aux = lista->prim;
		if (lista->prim->next != NULL) {
			lista->prim = lista->prim->next;
			lista->prim->prev = NULL;
		}
		else {
			lista->prim = NULL;
			lista->ultim = NULL;
		}
		free(aux);
	}
	return m;
}

ListaDubla citireCoadaDeMasiniDinFisier(const char* numeFisier) {

	ListaDubla lista;
	lista.prim = NULL;
	lista.ultim = NULL;
	FILE* f = fopen(numeFisier, "r");

	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		enqueue(&lista, m);
	}
	fclose(f);

	return lista;

}

void dezalocareCoadaDeMasini(ListaDubla* lista) {
	NodDublu* aux = lista->ultim;
	while (aux->prev) {
		NodDublu* temp = aux;
		aux = aux->prev;
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
	}
	lista->prim = NULL;
	lista->ultim = NULL;
}


//metode de procesare
Masina getMasinaByID(/*stiva sau coada de masini*/int id);

float calculeazaPretTotal(ListaDubla* coada) {
	float suma = 0;
	ListaDubla nou;
	nou.prim = NULL;
	nou.ultim = NULL;

	while ((*coada).prim) {
		Masina m = dequeue(coada);
		suma += m.pret;
		enqueue(&nou, m);
	}
	coada->prim = nou.prim;
	coada->ultim = nou.ultim;
	return suma;
}

int main() {
	Nod* stiva = citireStackMasiniDinFisier("masini.txt");
	int nr = size(stiva);
	printf("Nr. masini in stiva: %d\n", nr);

	Masina m = popStack(&stiva);
	afisareMasina(m);

	ListaDubla coada = citireCoadaDeMasiniDinFisier("masini.txt");
	printf("Suma preturi coada: %.2f\n", calculeazaPretTotal(&coada));

	dezalocareStivaDeMasini(&stiva);
	dezalocareCoadaDeMasini(&coada);

	return 0;
}
