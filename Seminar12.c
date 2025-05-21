#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

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

//1. 
// structuri necesare
//dorim stocarea unui graf intr-o lista de liste
//astfel avem nod ListaPrincipala si NodListaSecundara

typedef struct NodLP NodLP;//astea le definim inainte 
typedef struct NodLS  NodLS;

struct NodLP {
	Masina info;
	NodLP* next;
	NodLS* vecini;
};

struct NodLS {
	NodLS* next;
	NodLP* nodInfo;
};


//2.
//functii de inserare in liste
//si in principala si in secundara
void inserareLP(NodLP** cap, Masina masina) {
	NodLP* nou = (NodLP*)malloc(sizeof(NodLP));
	nou->next = NULL;
	nou->info = masina; //shallow copy
	nou->vecini = NULL;

	//inserare la final
	if (*cap) {
		NodLP* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}

}

void inserareLS(NodLS** cap, NodLP* nodInfo) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));

	nou->next = *cap;
	nou->nodInfo = nodInfo;

	*cap = nou;


}


//3.
//functie de cautarea in lista principala dupa ID
void* cautaNodDupaID(NodLP* cap, int id) {
	while (cap && cap->info.id != id) {
		cap = cap->next;
	}
	return cap;
}



//4.
//inserare muchie
void inserareMuchie(NodLP* cap, int idStart, int idStop) {
	NodLP* nodStart = cautaNodDupaID(cap, idStart);
	NodLP* nodStop = cautaNodDupaID(cap, idStop);

	if (nodStart && nodStop) {
		inserareLS(&(nodStart->vecini), nodStop);
		inserareLS(&(nodStop->vecini), nodStart);
	}
}




NodLP* citireNoduriMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	NodLP* graf = NULL;
	if (f) {
		while (!feof(f)) {
			inserareLP(&graf, citireMasinaDinFisier(f));
		}

	}
	fclose(f);
	return graf;


}



void citireMuchiiDinFisier(const char* numeFisier, NodLP* graf) {
	//functia primeste numele fisierului, il deschide si citeste 
	//toate id-urile de start si stop pentru fiecare muchie
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			int idStart;
			int idStop;
			fscanf(f, "%d %d", &idStart, &idStop);
			inserareMuchie(graf, idStart, idStop);
		}
		fclose(f);
	}

}

void stergereLS(NodLS** cap) {
	while (*cap) {
		NodLS* aux = (*cap)->next;
		free(*cap);
		*cap = aux;
	}
}


void dezalocareNoduriGraf(NodLP** cap) {
	//sunt dezalocate toate masinile din graf 
	//si toate nodurile celor doua liste
	while (*cap) {
		NodLP* aux = (*cap)->next;
		stergereLS(&((*cap)->vecini));
		if ((*cap)->info.numeSofer) {
			free((*cap)->info.numeSofer);
		}
		if ((*cap)->info.model) {
			free((*cap)->info.model);
		}
		free(*cap);
		*cap = aux;
	}

}

NodLP* citireGrafDinFisiere(const char* fisierNoduri, const char* fisierMuchii) {
	NodLP* graf = citireNoduriMasiniDinFisier(fisierNoduri);
	citireMuchiiDinFisier(fisierMuchii, graf);

	return graf;
}

void afisareGraf(NodLP* graf) {
	while (graf) {
		printf("In nodul %d avem masina: \n", graf->info.id);
		afisareMasina(graf->info);
		printf("Aceasta are ca vecini nodurile urmatoare: ");
		NodLS* p = graf->vecini;

		while (p) {
			printf("%d, ", p->nodInfo->info.id);
			p = p->next;
		}
		printf("\n\n");
		graf = graf->next;
	}
}

int main() {
	NodLP* graf = citireGrafDinFisiere("masini.txt", "muchii.txt");
	afisareGraf(graf);

	dezalocareNoduriGraf(&graf);

	return 0;
}