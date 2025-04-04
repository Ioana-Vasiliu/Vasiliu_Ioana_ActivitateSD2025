#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>

typedef struct Farmacie {
	char* denumire;
	float suprafataSpatiu;
	unsigned char nrAngajati;
	char* adresa;
}Farmacie;

typedef struct Nod {
	Farmacie info;
	struct Nod* next;
	struct Nod* prev;

}Nod;

typedef struct LD {
	struct Nod* prim;
	struct Nod* ultim;
}LD;

Farmacie creareFarmacie(const char* denumire, float suprafataSpatiu, unsigned char nrAngajati, const char* adresa) {
	Farmacie f;
	f.denumire = (char*)malloc(sizeof(char) * strlen(denumire) + 1);
	strcpy(f.denumire, denumire);
	f.suprafataSpatiu = suprafataSpatiu;
	f.nrAngajati = nrAngajati;
	f.adresa = (char*)malloc(sizeof(char) * strlen(adresa) + 1);
	strcpy(f.adresa, adresa);

	return f;
}

LD inserareInceputLD(LD lista, Farmacie f) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = creareFarmacie(f.denumire, f.suprafataSpatiu, f.nrAngajati, f.adresa);
	nou->prev = NULL;
	nou->next = NULL;

	if (lista.prim) {
		nou->next = lista.prim;
		lista.prim->prev = nou;
		lista.prim = nou;
	}
	else {
		lista.prim = nou;
		lista.ultim = nou;
	}
	return lista;
}

LD inserareFinalLD(LD lista, Farmacie f) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = creareFarmacie(f.denumire, f.suprafataSpatiu, f.nrAngajati, f.adresa);
	nou->next = NULL;
	nou->prev = NULL;

	if (lista.ultim) {
		nou->prev = lista.ultim;
		lista.ultim->next = nou;
		lista.ultim = nou;
	}
	else {
		lista.prim = nou;
		lista.ultim = nou;
	}
	return lista;
}

void afisareLD(LD lista) {

	if (lista.prim) {
		Nod* aux = lista.prim;
		while (aux) {
			printf(" Farmacia %s are ca suprafata spatiu comercial %5.2f, numar de angajati %u si adresa %s\n", aux->info.denumire, aux->info.suprafataSpatiu, aux->info.nrAngajati, aux->info.adresa);
			aux = aux->next;
		}
	}
	else {
		printf("Lista nu exista!\n");
	}
}

//2. Fct care determ NR DE FARMACII din lista cu suprafata spatiului comercial < un prag specificat ca parametru
//fct se apeleaza in main()

int nrFarmaciiSubPrag(LD lista, float prag) {
	int count = 0;
	Nod* aux = lista.prim;

	while (aux) {
		if (aux->info.suprafataSpatiu < prag) {
			count++;
		}
		aux = aux->next;
	}
	return count;
}

//3. Fct care elimina din LD toate farmaciile cu un nr de angajati mai mare decat un prag specificat ca parametru
//fct se apeleaza in main()

void eliminaFarmaciiPestePrag(LD* lista, int prag) {
	Nod* aux = lista->prim;
	while (aux) {
		Nod* temp = aux;
		aux = aux->next;

		if (temp->info.nrAngajati >= prag) {
			if (temp->prev) {
				temp->prev->next = temp->next;
			}
			else {
				lista->prim = temp->next; // primul nod
			}

			if (temp->next) {
				temp->next->prev = temp->prev;
			}
			else {
				lista->ultim = temp->prev; // ultimul nod
			}

			free(temp->info.denumire);
			free(temp->info.adresa);
			free(temp);
		}
	}
}
//4. Fct care copiaza intr-un vector toate farmaciile din LD care fac parte din acelasi LANT COMERCIAL
int numarFarmaciiDupaDenumire(LD lista, const char* denumire) {
	int count = 0;
	Nod* aux = lista.prim;

	while (aux) {
		if (strcmp(aux->info.denumire, denumire) == 0) {
			count++;
		}
		aux = aux->next;
	}
	return count;
}

Farmacie* conversieLDlaVector(LD lista, const char* denumire) {
	int dim = numarFarmaciiDupaDenumire(lista, denumire);
	Farmacie* vector = (Farmacie*)malloc(sizeof(Farmacie) * dim);
	int poz = 0;

	if (lista.prim) {
		Nod* aux = lista.prim;
		while (aux) {
			if (strcmp(aux->info.denumire, denumire) == 0) {
				vector[poz] = creareFarmacie(aux->info.denumire, aux->info.suprafataSpatiu, aux->info.nrAngajati, aux->info.adresa);
				poz++;

			}
			aux = aux->next;
		}
	}
	return vector;

}


void dezalocareLD(LD* lista) {
	Nod* aux = lista->prim;
	while (aux) {
		free(aux->info.denumire);
		free(aux->info.adresa);

		Nod* temp = aux->next;
		free(aux);
		aux = temp;
	}
	lista->prim = NULL;
	lista->ultim = NULL;
}

int main() {
	Farmacie f;
	LD lista;
	lista.prim = NULL;
	lista.ultim = NULL;
	char buffer[50];

	int nr = 0;
	FILE* fisier = fopen("farmacii.txt", "r");
	fscanf(fisier, "%d", &nr);
	for (int i = 0; i < nr; i++) {
		fscanf(fisier, "%s", buffer);
		f.denumire = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
		strcpy(f.denumire, buffer);
		fscanf(fisier, "%f", &f.suprafataSpatiu);
		fscanf(fisier, "%u", &f.nrAngajati);
		fscanf(fisier, "%s", buffer);
		f.adresa = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
		strcpy(f.adresa, buffer);

		lista = inserareInceputLD(lista, f);
		free(f.denumire);
		free(f.adresa);
	}
	fclose(fisier);

	afisareLD(lista);

	printf("\nCerinta 2\n");
	float prag = 1000;
	int numar = nrFarmaciiSubPrag(lista, prag);
	printf("Numarul de farmacii cu suprafata sub %.2f este: %d\n", prag, numar);

	printf("\nCerinta 3\n");
	int prag1 = 12;
	eliminaFarmaciiPestePrag(&lista, prag1);
	printf("Afisare actualizata dupa eliminare: \n");
	afisareLD(lista);

	printf("\nCerinta 4\n");
	int nrFarmacii = numarFarmaciiDupaDenumire(lista, "Sensiblu");
	Farmacie* vector = conversieLDlaVector(lista, "Sensiblu");

	for (int i = 0; i < nrFarmacii; i++) {
		printf("Farmacia %s are suprafata %5.2f, numar de angajati %u si adresa %s\n",
			vector[i].denumire, vector[i].suprafataSpatiu, vector[i].nrAngajati, vector[i].adresa);
		free(vector[i].denumire);
		free(vector[i].adresa);
	}
	free(vector);



	return 0;

}