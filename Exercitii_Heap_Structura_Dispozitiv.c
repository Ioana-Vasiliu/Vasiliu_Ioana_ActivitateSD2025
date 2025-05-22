#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>

typedef struct dispozitivMobil dispozitivMobil;
typedef struct heap heap;
typedef struct Nod Nod;
typedef struct NodStiva NodStiva;

struct dispozitivMobil {
	unsigned int id;
	char* sistemOperare;
	float pret;
	char* defecte;
	unsigned int nrZile;
};

dispozitivMobil inializareDispozitiv(unsigned int id, const char* sistemOperare, float pret, const char* defecte, unsigned int nrZile) {
	dispozitivMobil d;
	d.id = id;
	d.sistemOperare = (char*)malloc(sizeof(char) * (strlen(sistemOperare) + 1));
	strcpy(d.sistemOperare, sistemOperare);

	d.pret = pret;
	d.defecte = (char*)malloc(sizeof(char) * (strlen(defecte) + 1));
	strcpy(d.defecte, defecte);
	d.nrZile = nrZile;

	return d;
}

struct heap {
	int dim;
	dispozitivMobil* vector;
};
struct Nod {
	dispozitivMobil info;
	struct Nod* next;
};

struct NodStiva {
	dispozitivMobil info;
	struct NodStiva* next;
};

void afisareDispozitivMobil(dispozitivMobil d) {
	printf("ID: %u, Sistem: %s, Pret: %.2f, Defecte: %s, Zile: %u\n", d.id, d.sistemOperare, d.pret, d.defecte, d.nrZile);
}

//functia de filtrare
void filtrare(heap h, int index) {
	int indexRad = index;
	int indexSt = 2 * index + 1;
	int indexDr = 2 * index + 2;

	if (indexSt < h.dim && h.vector[indexSt].nrZile > h.vector[indexRad].nrZile) {
		indexRad = indexSt;
	}
	if (indexDr < h.dim && h.vector[indexDr].nrZile > h.vector[indexRad].nrZile) {
		indexRad = indexDr;
	}

	if (index != indexRad) {
		dispozitivMobil aux = h.vector[indexRad];
		h.vector[indexRad] = h.vector[index];
		h.vector[index] = aux;
		if (indexRad * 2 + 1 < h.dim - 1) {
			filtrare(h, indexRad);
		}
	}
}

heap adaugare(heap h, dispozitivMobil d) {
	dispozitivMobil* copieVector = (dispozitivMobil*)malloc(sizeof(dispozitivMobil) * (h.dim + 1));
	for (int i = 0;i < h.dim;i++) {
		copieVector[i] = h.vector[i];
	}
	copieVector[h.dim] = d;
	free(h.vector);
	h.dim++;
	h.vector = copieVector;
	for (int i = (h.dim / 2) - 1;i >= 0;i--) {
		filtrare(h, i);
	}
	return h;
}

//Ex 4 : Implementați funcția care șterge dispozitivul mobil cu cel mai mare număr de zile de la
//intrarea în service 
heap extragere(heap h, dispozitivMobil* dm)
{
	dispozitivMobil aux = h.vector[0];
	h.vector[0] = h.vector[h.dim - 1];
	h.vector[h.dim - 1] = aux;

	dispozitivMobil* copieVector = (dispozitivMobil*)malloc(sizeof(dispozitivMobil) * (h.dim - 1));
	for (int i = 0;i < h.dim - 1;i++) {
		copieVector[i] = h.vector[i];
	}
	(*dm) = inializareDispozitiv(h.vector[h.dim - 1].id, h.vector[h.dim - 1].sistemOperare, h.vector[h.dim - 1].pret,
		h.vector[h.dim - 1].defecte, h.vector[h.dim - 1].nrZile);
	free(h.vector);
	h.dim--;
	h.vector = copieVector;
	for (int i = (h.dim / 2) - 1;i >= 0;i--) {
		filtrare(h, i);
	}
	return h;
}

void afisareHeap(heap h) {
	if (h.vector) {
		for (int i = 0;i < h.dim;i++) {
			afisareDispozitivMobil(h.vector[i]);
		}
	}
}


//Ex 2 : Implementați funcția care determină numărul de dispozitive cu un anumit sistem de
//operare specificat ca parametru

int nrDispozitive(heap h, const char* sistemop) {
	int count = 0;
	for (int i = 0;i < h.dim;i++) {
		if (strcmp(h.vector[i].sistemOperare, sistemop) == 0)
			count++;
	}
	return count;
}

//Ex 3 : Implementați funcția care actualizează defectele unui dispozitiv mobil cu id-ul
//specificat ca parametru al funcției.

void actualizareDefecte(heap* h, unsigned int id, const char* defect) {
	for (int i = 0;i < h->dim;i++) {
		if (h->vector[i].id == id) {
			free(h->vector[i].defecte);
			h->vector[i].defecte = (char*)malloc((strlen(defect) + 1));
			strcpy(h->vector[i].defecte, defect);
		}
	}
}

//Ex 5 : Implementați funcția care salvează într-o structură de tip stivă dispozitivele mobile
//extrase din coada de priorități

//push pe stiva
void push(NodStiva** varf, dispozitivMobil dp) {
	NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
	nou->info = inializareDispozitiv(dp.id, dp.sistemOperare, dp.pret, dp.defecte, dp.nrZile);
	nou->next = *varf;
	*varf = nou;
}

//inserare in stiva

NodStiva* transferInStiva(heap h) {
	NodStiva* stiva = NULL;
	dispozitivMobil extras;
	if (h.vector) {

		//for (int i = 0;i < h.dim;i++) {
		//while(h.vector!=NULL){
		while (h.dim != 0) {
			h = extragere(h, &extras);
			push(&stiva, extras);
		}
		return stiva;
	}
}

void afisareStiva(NodStiva* varf) {
	while (varf) {
		afisareDispozitivMobil(varf->info);
		varf = varf->next;
	}
}




void dezalocare(heap* h) {
	if (h->vector) {
		for (int i = 0;i < h->dim;i++) {
			free(h->vector[i].sistemOperare);
			free(h->vector[i].defecte);
		}
		free(h->vector);
		h->vector = NULL;
		h->dim = 0;
	}
}

void dezalocareStiva(NodStiva** varf) {
	while (*varf) {
		NodStiva* temp = *varf;
		free(temp->info.sistemOperare);
		free(temp->info.defecte);
		*varf = (*varf)->next;
		free(temp);
	}
}

void main() {
	dispozitivMobil dm;
	heap h;
	h.dim = 0;
	h.vector = NULL;

	char liniebuffer[100];
	char sep[] = ",\n";
	char* token;

	FILE* f = fopen("dispozitive.txt", "r");
	if (f) {
		while (fgets(liniebuffer, 100, f)) {
			token = strtok(liniebuffer, sep);
			dm.id = atoi(token);

			token = strtok(NULL, sep);
			dm.sistemOperare = (char*)malloc(sizeof(char) * (strlen(token) + 1));
			strcpy(dm.sistemOperare, token);

			token = strtok(NULL, sep);
			dm.pret = atof(token);

			token = strtok(NULL, sep);
			dm.defecte = (char*)malloc(sizeof(char) * (strlen(token) + 1));
			strcpy(dm.defecte, token);

			token = strtok(NULL, sep);
			dm.nrZile = atoi(token);

			h = adaugare(h, dm);
		}
		fclose(f);
	}
	else {
		printf("Nu s-a gasit fisierul");
	}
	afisareHeap(h);

	//Ex2
	printf("\nNr dispozitive LogiTech %d\n", nrDispozitive(h, "LogiTech"));

	//Ex3
	actualizareDefecte(&h, 1, "Placa Sparta");
	afisareHeap(h);

	//Ex4
	printf("\nDupa extragerea dispozitivului cu cel mai mare nr de zile :\n ");
	h = extragere(h, &dm);
	afisareHeap(h);

	//Ex5
	NodStiva* stiva = transferInStiva(h);
	printf("\nDispozitivele din stiva :\n");
	afisareStiva(stiva);
	//dezalocare(&h);
	dezalocareStiva(&stiva);
}