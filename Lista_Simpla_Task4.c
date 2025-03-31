#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct Masina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct Masina Masina;

typedef struct Nod Nod;
 struct Nod {
	Masina info;
	Nod* next;
};

 Masina initializare(int idNou, int nrUsiNou, float pretNou, const char* modelNou, const char* numeSoferNou, unsigned char serieNou) {
	 Masina m;
	 m.id = idNou;
	 m.nrUsi = nrUsiNou;
	 m.pret = pretNou;

	 m.model = (char*)malloc(strlen(modelNou) + 1);
	 strcpy_s(m.model, strlen(modelNou) + 1, modelNou);

	 m.numeSofer = (char*)malloc(strlen(numeSoferNou) + 1);
	 strcpy_s(m.numeSofer, strlen(numeSoferNou) + 1, numeSoferNou);

	 m.serie = serieNou;

	 return m;
 }

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

void afisareListaMasini(Nod* lista) {
	while (lista) {
		afisareMasina(lista->info);
		lista = lista->next;
	}
}
// Functie pentru afisarea vectorului de masini
void afisareVector(Masina* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisareMasina(vector[i]);
	}
}


void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	//shallow copy
	nou->info = masinaNoua;
	nou->next = NULL;
	if ((*lista) == NULL) {
		(*lista) = nou;

	}
	else {
		Nod* temp = *lista;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}

}

void adaugaLaInceputInLista(Nod** lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = *lista;

	*lista = nou;

}

//inserare sortata dupa pret
Nod* inserareSortata(Masina m, Nod* cap) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initializare(m.id, m.nrUsi, m.pret, m.model, m.numeSofer, m.serie);
	nou->next = NULL;

	if (cap) {
		if (cap->info.pret > m.pret) {
			nou->next = cap;
			cap = nou;
		}
		else {
			Nod* p = cap;
			while (p->next && p->next->info.pret < m.pret) {
				p = p->next;
			}
			nou->next = p->next;
			p->next = nou;
		}
	}
	else {
		cap = nou;
	}
	return cap;
}

//am modif si aici pt a lucra cu ea sortata
Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	if (!file) {
		printf("Eroare la deschiderea fisierului!\n");
		return NULL;
	}

	Nod* lista = NULL;
	while (!feof(file)) {
		Masina m = citireMasinaDinFisier(file);
		lista = inserareSortata(m, lista); 
	}
	fclose(file);
	return lista;
}


void dezalocareListaMasini(Nod** lista) {
	while (*lista);
	{
		Nod* p = (*lista);
		(*lista) = (*lista)->next;

		free(p->info.model);
		free(p->info.numeSofer);

		free(p);
	}
	
}

float calculeazaPretMediu(Nod* lista) {
	float  s = 0;
	int k = 0;

	while (lista) {
		s += lista->info.pret;
		k++;
		lista = lista->next;
	}
	return (k>0) ? s/k :0;
}

void stergeMasiniDinSeria(Nod** lista, char serieCautata) {
	while ((*lista)!=NULL && (*lista)->info.serie == serieCautata) {
		Nod* temp = (*lista);
		(*lista) = temp->next;
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);

	}
	Nod* p = (*lista);
	while (p != NULL) {
		while (p->next && p->next->info.serie != serieCautata) {
			p = p->next;

		}
		if (p->next) {
			Nod* temp = p->next;
			p->next = temp->next;
			free(temp->info.model);
			free(temp->info.numeSofer);
			free(temp);
		}
		else {
			p = p->next;
		}
	}

}

float calculeazaPretulMasinilorUnuiSofer(Nod* lista, const char* numeSofer) {
	float suma = 0;
	while (lista) {
		if (strcmp(lista->info.numeSofer, numeSofer) == 0) {
			suma += lista->info.pret;
		}
		lista = lista->next;
	}
	
	return suma;
}

//functie pentru stergerea unui nod de pe o pozitie data 
void stergeNodPePozitie(Nod** lista, int pozitie) {
	if (*lista == NULL) { //lista goala, nu stergem
		return;
	}

	Nod* p = *lista;
	if (pozitie == 0) { //poz = 0, stergem primul nod
		*lista = p->next;
		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
		return;
	}

	int contor = 0;
	while (p != NULL && contor < pozitie - 1) {  
		p = p->next;
		contor++;
	}

	if (p == NULL || p->next == NULL) { 
		return;
	}

	Nod* temp = p->next;
	p->next = temp->next;  
	free(temp->info.model);
	free(temp->info.numeSofer);
	free(temp);
}

// Functie care creeaza un nou vector care contine doar elementele care indeplinesc conditia : numele soferului sa fie "Ionesu"
void copiazaMasiniFiltrate(Nod* lista, const char* numeFiltrat, Masina** vectorNou, int* dimensiune) {
	(*dimensiune) = 0;
	Nod* temp = lista;
	while (temp) {
		if (strcmp(temp->info.numeSofer, numeFiltrat) == 0) {
			(*dimensiune)++;
		}
		temp = temp->next;
	}

	(*vectorNou) = (Masina*)malloc(sizeof(Masina) * (*dimensiune));
	int k = 0;
	temp = lista;

	while (temp) {
		if (strcmp(temp->info.numeSofer, numeFiltrat) == 0) {
			(*vectorNou)[k].id = temp->info.id;
			(*vectorNou)[k].nrUsi = temp->info.nrUsi;
			(*vectorNou)[k].pret = temp->info.pret;
			(*vectorNou)[k].serie = temp->info.serie;

			(*vectorNou)[k].model = (char*)malloc(strlen(temp->info.model) + 1);
			strcpy_s((*vectorNou)[k].model, strlen(temp->info.model) + 1, temp->info.model);
			(*vectorNou)[k].numeSofer = (char*)malloc(strlen(temp->info.numeSofer) + 1);
			strcpy_s((*vectorNou)[k].numeSofer, strlen(temp->info.numeSofer) + 1, temp->info.numeSofer);
			k++;
		}
		temp = temp->next; 
	}
}

void dezalocaVectorMasini(Masina* vector, int dimensiune) {
	for (int i = 0; i < dimensiune; i++) {
		free(vector[i].model);
		free(vector[i].numeSofer);
	}
	free(vector);
}


int main() {
	Nod* masini = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(masini);


	printf("-----PRET MEDIU----\n");
	printf("%.2f", calculeazaPretMediu(masini));

	printf("\n-----SUMA MASINI SOFER----\n");
	printf("Suma masinilor lui Ionescu: %.2f", calculeazaPretulMasinilorUnuiSofer(masini, "Ionescu"));

	char serieCautata = 'A';
	printf("\n-----STERGERE SERIA A---\n");
	stergeMasiniDinSeria(&masini, serieCautata);
	afisareListaMasini(masini);

	//testare pt stergerea nodului de pe poz 3
	int pozitie = 3;  
	printf("\n-----STERGERE POZITIA 3, IN CAZUL NOSTRU ID = 6---\n");
	stergeNodPePozitie(&masini, pozitie);
	afisareListaMasini(masini);

	// adaugare element nou dupa ce am sortat lista 
	printf("\n-----ADAUGARE IN LISTA DUPA CE AM SORTAT-O DUPA PRET\n");
	Masina masina1 = initializare(1, 4, 7000, "BMW", "Popescu", 'A');
	masini = inserareSortata(masina1, masini);
	afisareListaMasini(masini);


	//apel pentru adaugarea elementelor filtrate in vector
	printf("\n-----VECTOR FILTRAT DUPA NUME\n");
	Masina* vectorFiltrat = NULL;
	int dimensiune = 0;
	copiazaMasiniFiltrate(masini, "Ionescu", &vectorFiltrat, &dimensiune);
	afisareVector(vectorFiltrat, dimensiune);

	dezalocareListaMasini(&masini);
	dezalocaVectorMasini(vectorFiltrat, dimensiune);


	return 0;
}