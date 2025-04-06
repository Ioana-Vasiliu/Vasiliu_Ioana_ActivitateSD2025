#define _CRT_SECURE_NO_WARNINGS
#include<malloc.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>


typedef struct proiect {
	unsigned int cod_proiect;
	char* titlu_proiect;
	char* beneficiar;
	unsigned char nr_executanti;
	float buget;

}proiect;

typedef struct nod {
	struct nod* prev;
	struct nod* next;
	proiect info;
}nod;

typedef struct ld {
	nod* ultim;
	nod* prim;
}ld;

proiect creareProiect(unsigned int cod_proiect, const char* titlu_proiect, const char* beneficiar, unsigned char nr_executanti, float buget) {
	proiect p;
	p.cod_proiect = cod_proiect;

	p.titlu_proiect = (char*)malloc(sizeof(char) * (strlen(titlu_proiect) + 1));
	strcpy(p.titlu_proiect, titlu_proiect);

	p.beneficiar = (char*)malloc(sizeof(char) * (strlen(beneficiar) + 1));
	strcpy(p.beneficiar, beneficiar);

	p.nr_executanti = nr_executanti;
	p.buget = buget;

	return p;

}

//inserare inceput
ld inserareInceput(ld lista, proiect p) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareProiect(p.cod_proiect, p.titlu_proiect, p.beneficiar, p.nr_executanti, p.buget);
	nou->prev = NULL;
	nou->next = NULL;

	if (lista.prim == NULL) {
		lista.prim = lista.ultim = nou;

	}
	else {
		nou->next = lista.prim;
		lista.prim->prev = nou;
		lista.prim = nou;
	}

	return lista;
}

//inserare final
ld inserareFinal(ld lista, proiect p) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareProiect(p.cod_proiect, p.titlu_proiect, p.beneficiar, p.nr_executanti, p.buget);
	nou->prev = NULL;
	nou->next = NULL;

	if (lista.prim == NULL) {
		lista.prim = lista.ultim = nou;
	}
	else {
		lista.ultim->next = nou;
		nou->prev = lista.ultim;
		lista.ultim = nou;
	}
	return lista;
}

//afisarea inceput
void afisareInceput(ld lista) {
	if (lista.prim) {
		nod* aux = lista.prim;
		while (aux) {
			printf("%u, %s, %s, %u, %5.2f\n", aux->info.cod_proiect, aux->info.titlu_proiect, aux->info.beneficiar, aux->info.nr_executanti, aux->info.buget);
			aux = aux->next;
		}
	}
}

//afisare final
void afisareFinal(ld lista) {
	if (lista.prim) {
		nod* aux = lista.ultim;
		while (aux) {
			printf("%u, %s, %s, %u, %5.2f\n", aux->info.cod_proiect, aux->info.titlu_proiect, aux->info.beneficiar, aux->info.nr_executanti, aux->info.buget);
			aux = aux->prev;
		}
	}
	else {
		printf("Nu exista lista\n");
	}
}

//dezalocare
void dezalocare(ld lista) {
	nod* aux = lista.prim;
	while (aux) {
		free(aux->info.titlu_proiect);
		free(aux->info.beneficiar);

		nod* temp = aux->next;
		free(aux);
		aux = temp;
	}
	lista.prim = lista.ultim = NULL;
}

//nr proiecte peste un anumit prag
int nrProiectePrag(ld lista, unsigned char prag) {
	nod* aux = lista.prim;
	int contor = 0;

	while (aux) {
    if (aux->info.nr_executanti > prag) {
        contor++;
		}
		aux = aux->next;
	}
	return contor;
}

void actualizareBuget(ld lista, const char* beneficiar, float procentaj) {
	nod* aux = lista.prim;
	while (aux) {
		if (strcmp(beneficiar, aux->info.beneficiar) == 0) { 
			aux->info.buget += aux->info.buget * procentaj;

		}
		aux = aux->next;
	}
}

void sortareBuget(ld* lista) {
	int sortat;
	do {
		sortat = 0;
		nod* aux = lista->prim;
		while (aux->next) {
			if (aux->info.buget > aux->next->info.buget) {
				proiect temp = aux->info;
				aux->info = aux->next->info;
				aux->next->info = temp;

				sortat = 1;
			}
			aux = aux->next;
		}
	} while (sortat != 0);
}

int contorNoduri(ld lista) {
	nod* aux = lista.prim;
	int contor = 0;

	while (aux) {
		contor++;
		aux = aux->next;
	}
	return contor;
}

//conversie la vector fara partajare( fara pointer)
//sa salvam in vector doar anumite elemente
proiect* conversieVector(ld lista) {
	nod* aux = lista.prim;
	int dim = contorNoduri(lista);
	int iterator = 0;
	proiect* vector = (proiect*)malloc(dim * sizeof(proiect));

	while (aux) {
		vector[iterator] = creareProiect(aux->info.cod_proiect, aux->info.titlu_proiect, aux->info.beneficiar, aux->info.nr_executanti, aux->info.buget);
		iterator++;
		aux = aux->next;
	}
	return vector; 
}

void main() {
	ld lista;
	lista.prim = NULL;
	lista.ultim = NULL;
	int nr;
	char buffer[50];
	proiect p;

	FILE* f = fopen("proiecte.txt", "r");

	fscanf(f, "%d", &nr);
	for (int i = 0; i < nr; i++) {
		fscanf(f, "%u", &p.cod_proiect);

		fscanf(f, "%s", buffer);
		p.titlu_proiect = (char*)malloc(strlen(buffer) + 1);
		strcpy(p.titlu_proiect, buffer);

		fscanf(f, "%s", buffer);
		p.beneficiar = (char*)malloc(strlen(buffer) + 1);
		strcpy(p.beneficiar, buffer);

		fscanf(f, "%u", &p.nr_executanti);

		fscanf(f, "%f", &p.buget);

		lista = inserareInceput(lista, p);

		free(p.titlu_proiect);
		free(p.beneficiar);
	}
	fclose(f);

	// EXERCITIUL 2: numar proiecte cu mai multi executanti decat pragul dat
	printf("\n--- EXERCITIUL 2 ---\n");
	printf("Numar proiecte cu peste 24 executanti: %d\n", nrProiectePrag(lista, 24));

	// EXERCITIUL 3: marim bugetul proiectelor TechCorp cu 25%
	printf("\n--- EXERCITIUL 3 ---\n");
	printf("Bugete inainte de actualizare:\n");
	afisareInceput(lista);

	actualizareBuget(lista, "TechCorp", 0.25);

	printf("\nBugete DUPA actualizare pentru beneficiar 'TechCorp':\n");
	afisareInceput(lista);

	// EXERCITIUL 4: sortam lista dupa buget crescator
	printf("\n--- EXERCITIUL 4 ---\n");
	printf("Lista sortata dupa buget:\n");
	sortareBuget(&lista);
	afisareInceput(lista);

	// EXERCITIUL 5: conversie lista -> vector
	printf("\n--- EXERCITIUL 5 ---\n");
	printf("Conversie lista in vector\n");
	proiect* v = conversieVector(lista);

	//afisam vectorul
	printf("Proiecte in vector:\n");
	for (int i = 0; i < contorNoduri(lista); i++) {
		printf("%u, %s, %s, %u, %5.2f\n",
			v[i].cod_proiect,
			v[i].titlu_proiect,
			v[i].beneficiar,
			v[i].nr_executanti,
			v[i].buget);
	}

	free(v);
	dezalocare(lista);
}