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

struct Nod {
	Masina info;
	Nod* st;
	Nod* dr;
};
typedef struct Nod Nod;


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


Nod* adaugaMasinaInArbore(Nod* arbore, Masina masinaNoua) {
	if (arbore != NULL) {
		if (arbore->info.id < masinaNoua.id) {
			arbore->dr = adaugaMasinaInArbore(arbore->dr, masinaNoua);
		}
		else {
			arbore->st = adaugaMasinaInArbore(arbore->st, masinaNoua);

		}
	}
	else {
		Nod* temp = (Nod*)malloc(sizeof(Nod));
		temp->info = masinaNoua;
		temp->dr = NULL;
		temp->st = NULL;
		arbore = temp;
	}
	return arbore;


}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {

	Nod* radacina = NULL;
	if (numeFisier != NULL && strlen(numeFisier) != 0) {
		FILE* f = fopen(numeFisier, "r");
		if (f != NULL) {
			int dim = 0;
			fscanf(f, "%d", &dim);
			for (int i = 0; i < dim; i++) {
				Masina masina;
				masina = citireMasinaDinFisier(f);
				adaugaMasinaInArbore(&radacina, masina);

			}
		}
	}
	return radacina;

}

void afisareMasiniDinArboreInOrdine(Nod* arbore) {


	if (arbore != NULL) {

		afisareMasiniDinArbore(arbore->st); 
		afisareMasina(arbore->info);       
		afisareMasiniDinArbore(arbore->dr); 
	}
}



void afisareMasiniDinArboreInPreOrdine(Nod* arbore) {
	if (arbore != NULL) {

		afisareMasina(arbore->info);


		afisareMasiniDinArboreInPreOrdine(arbore->st);

	
		afisareMasiniDinArboreInPreOrdine(arbore->dr);
	}
}


void afisareMasiniDinArboreInPostOrdine(Nod* arbore) {
	if (arbore != NULL) {

		afisareMasiniDinArboreInPostOrdine(arbore->st);


		afisareMasiniDinArboreInPostOrdine(arbore->dr);

	
		afisareMasina(arbore->info);
	}
}



void dezalocareArboreDeMasini(Nod* arbore) {
	if (arbore != NULL) {
	
		dezalocareArboreDeMasini(arbore->st);

	
		dezalocareArboreDeMasini(arbore->dr);

		free(arbore->info.model);
		free(arbore->info.numeSofer);

		
		free(arbore);
	}

}

Masina getMasinaByID(Nod* arbore, int id) {
	Masina m;
	if (arbore) {
		if (arbore->info.id < id) {
			return getMasinaByID(arbore->dr, id);

		}
		else {
			if (arbore->info.id > id) {
				return getMasinaByID(arbore->st, id);
			}

		}
	}
	else {
		Masina m;
		m.id = -1;
		m.numeSofer = NULL;
		m.nrUsi = 0;
		m.model = NULL;
		m.pret = 0;
		m.serie = 0;

	}


	return m;
}

int determinaNumarNoduri(Nod* arbore) {

	if (arbore) {  
		return 1 + determinaNumarNoduri(arbore->st) + determinaNumarNoduri(arbore->dr);
	}
	return 0;
}

int calculeazaInaltimeArbore(Nod* arbore) {

	if (arbore == NULL) {
		return 0;
	}

	int inaltimeStanga = calculeazaInaltimeArbore(arbore->st);
 
	int inaltimeDreapta = calculeazaInaltimeArbore(arbore->dr);
 
	return 1 + (inaltimeStanga > inaltimeDreapta ? inaltimeStanga : inaltimeDreapta);
}

float calculeazaPretTotal(Nod* arbore) {
	if (arbore == NULL) {
		return 0;
	}

	return arbore->info.pret + calculeazaPretTotal(arbore->st) + calculeazaPretTotal(arbore->dr);
}

float calculeazaPretulMasinilorUnuiSofer(Nod* arbore, const char* numeSofer) {
	if (arbore == NULL) {
		return 0;
	}

	float pretSofer = 0;


	if (strcmp(arbore->info.numeSofer, numeSofer) == 0) {
		pretSofer = arbore->info.pret;
	}

	return pretSofer + calculeazaPretulMasinilorUnuiSofer(arbore->st, numeSofer) + calculeazaPretulMasinilorUnuiSofer(arbore->dr, numeSofer);
}

int main() {
	Nod* arbore = NULL;


	arbore = citireArboreDeMasiniDinFisier("masini.txt");
	if (arbore == NULL) {
		printf("Eroare: Nu s-a putut citi arborele din fisier sau fisierul este gol.\n");
		return -1;
	}

	printf("Afisare masini in ordine (inordine):\n");
	afisareMasiniDinArboreInOrdine(arbore);

	printf("\nAfisare masini in preordine:\n");
	afisareMasiniDinArboreInPreOrdine(arbore);

	printf("\nAfisare masini in postordine:\n");
	afisareMasiniDinArboreInPostOrdine(arbore);


	int idCautat = 2; 
	Masina masinaGasita = getMasinaByID(arbore, idCautat);
	if (masinaGasita.id != -1) {
		printf("\nMasina cu ID-ul %d a fost gasita:\n", idCautat);
		afisareMasina(masinaGasita);
	}
	else {
		printf("\nMasina cu ID-ul %d nu a fost gasita.\n", idCautat);
	}


	int numarNoduri = determinaNumarNoduri(arbore);
	printf("\nNumarul total de masini din arbore: %d\n", numarNoduri);

	int inaltimeArbore = calculeazaInaltimeArbore(arbore);
	printf("Inaltimea arborelui: %d\n", inaltimeArbore);

	float pretTotal = calculeazaPretTotal(arbore);
	printf("Pretul total al masinilor din arbore: %.2f\n", pretTotal);

	const char* numeSofer = "Ion Popescu";
	float pretSofer = calculeazaPretulMasinilorUnuiSofer(arbore, numeSofer);
	printf("Pretul total al masinilor soferului %s: %.2f\n", numeSofer, pretSofer);

	dezalocareArboreDeMasini(arbore);

	return 0;
}