#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structura contine literele V si I
typedef struct VizitaMedicala {
	int id;
	char* numePacient;
	float cost;
	int varsta;
	char* diagnostic;

}VizitaMedicala;


//Functia de afisare

void afisareVizita(VizitaMedicala vizita) {
	printf("\nVizita medicala:\n");
	printf("ID vizita: %d\n", vizita.id);
	printf("Nume pacient: %s\n", vizita.numePacient);
	printf("Cost: %.2f\n", vizita.cost);
	printf("Varsta: %d\n", vizita.varsta);
	printf("Diagnostic: %s\n", vizita.diagnostic);

}

//initializare
struct VizitaMedicala initializareVizita(int id, const char* numePacient, float cost, int varsta, const char* diagnostic) {
	struct VizitaMedicala v;

	v.id = id;
	v.numePacient = (char*)malloc(strlen(numePacient) + 1);
	strcpy_s(v.numePacient, strlen(numePacient) + 1, numePacient);

	v.cost = cost;
	v.varsta = varsta;
	v.diagnostic = (char*)malloc(strlen(diagnostic) + 1);
	strcpy_s(v.diagnostic, strlen(diagnostic) + 1, diagnostic);

	return v;


}
//afisare vector elemente
void afisareVector(struct VizitaMedicala* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisareVizita(vector[i]);
	}
}

//functia dezalocare
void dezalocareVector(struct VizitaMedicala** vector, int* nrElemente) {
	//dezalocam elementele din vector si vectorul
	for (int i = 0;i < *nrElemente;i++) {
		free((*vector)[i].numePacient);
		free((*vector)[i].diagnostic);
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

// Functie care creeaza un nou vector care contine doar elementele care indeplinesc conditia : varsta > 30 

void copiazaViziteFiltrate(struct VizitaMedicala* vector, int nrElemente, int varstaPrag, struct VizitaMedicala** vectorNou, int* dimensiune) {
	(*dimensiune) = 0;

	//det cate vizite indeplinesc conditia noastra (varsta >30)
	for(int i = 0;i < nrElemente;i++) {
		if (vector[i].varsta > varstaPrag) {
			(*dimensiune)++;
		}

	}

	(*vectorNou) = malloc(sizeof(struct VizitaMedicala) * (*dimensiune));

	int k = 0;

	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].varsta > varstaPrag) {
			(*vectorNou)[k] = vector[i];
			(*vectorNou)[k].numePacient = malloc(sizeof(char) * (strlen(vector[i].numePacient) + 1));
			strcpy_s((*vectorNou)[k].numePacient, strlen(vector[i].numePacient) + 1, vector[i].numePacient);
			(*vectorNou)[k].diagnostic = malloc(sizeof(char) * (strlen(vector[i].diagnostic) + 1));
			strcpy_s((*vectorNou)[k].diagnostic, strlen(vector[i].diagnostic) + 1, vector[i].diagnostic);

			k++;
		}
	}
}

void mutaVizitePneumonie(struct VizitaMedicala* vector, int* nrElemente, struct VizitaMedicala** vectorNou, int* dimensiune) {
	(*dimensiune) = 0;

	for (int i = 0; i < *nrElemente; i++) {
		if (strcmp(vector[i].diagnostic, "Pneumonie") == 0) {
			(*dimensiune)++;
		}
	}
	(*vectorNou) = malloc(sizeof(struct VizitaMedicala) * (*dimensiune));

	int k = 0;

	for (int i = 0; i < *nrElemente; i++) {
		if (strcmp(vector[i].diagnostic, "Pneumonie") == 0) {
			(*vectorNou)[k] = vector[i];
			(*vectorNou)[k].numePacient = malloc(sizeof(char) * (strlen(vector[i].numePacient) + 1));
			strcpy_s((*vectorNou)[k].numePacient, strlen(vector[i].numePacient) + 1, vector[i].numePacient);
			(*vectorNou)[k].diagnostic = malloc(sizeof(char) * (strlen(vector[i].diagnostic) + 1));
			strcpy_s((*vectorNou)[k].diagnostic, strlen(vector[i].diagnostic) + 1, vector[i].diagnostic);
			k++;
		}
	}
	int j = 0;
	for (int i = 0; i < *nrElemente; i++) {
		if (strcmp(vector[i].diagnostic, "Pneumonie") != 0) {
			vector[j] = vector[i];
			j++;
		}
	}
	*nrElemente = j;
}



int main() {

	//task 2 part 1
	//vector alocat dinamic cu cel putin 5 obiecte 

	struct VizitaMedicala v;
	int nrVizite = 5;

	struct VizitaMedicala* vectorVizite;
	vectorVizite = malloc(sizeof(struct VizitaMedicala) * nrVizite);
	vectorVizite[0] = initializareVizita(10, "Mircea Popescu", 560.50, 78, "Pneumonie");
	vectorVizite[1] = initializareVizita(11, "Elena Marinescu", 310.50, 25, "Psoriazis");
	vectorVizite[2] = initializareVizita(12, "Marcela Ionescu", 230.50, 40, "Diabet Tip 2");
	vectorVizite[3] = initializareVizita(13, "Vlad Teodorescu", 150.30, 34, "Pneumonie");
	vectorVizite[4] = initializareVizita(14, "Monica Sandu", 250.00, 15, "Hipertensiune");

	// Afisare vizite
	printf("\n Afisare vector de vizite");
	afisareVector(vectorVizite, nrVizite);

	// Apel vector copiat, filtrat pe baza varstei 
	struct VizitaMedicala* vectorViziteCopiat;
	vectorViziteCopiat = NULL;
	int nrElementeCopiate = 0;


	printf("\n Vector Vizite Filtrate dupa varsta : \n");
	copiazaViziteFiltrate(vectorVizite, nrVizite, 30, &vectorViziteCopiat, &nrElementeCopiate);
	afisareVector(vectorViziteCopiat, nrElementeCopiate);

	dezalocareVector(&vectorViziteCopiat, &nrElementeCopiate);


	//apel vectro mutat pe baza conditiei diagnostic = pneumonie
	struct VizitaMedicala* vectorViziteMutate;
	vectorViziteMutate = NULL;
	int nrElementeMutate = 0;

	
	mutaVizitePneumonie(vectorVizite, &nrVizite, &vectorViziteMutate, &nrElementeMutate);

	printf("\nVectorul cu vizite care au diagnostic 'Pneumonie' mutat:\n");
	afisareVector(vectorViziteMutate, nrElementeMutate);

	
	printf("\nVectorul original dupa mutare (trebuie sa nu mai contina vizite cu 'Pneumonie'):\n");
	afisareVector(vectorVizite, nrVizite);

	dezalocareVector(&vectorVizite, &nrVizite);
	dezalocareVector(&vectorViziteMutate, &nrElementeMutate);

	return 0;
}