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
void afisareVectorVizite(VizitaMedicala* vizite, int nrVizite) {
	for (int i = 0; i < nrVizite; i++)
	{
		afisareVizita(vizite[i]);
	}
}

void adaugaViziteInVector(VizitaMedicala** vizite, int* nrVizite, VizitaMedicala vizitaNoua) {
	VizitaMedicala* vector = malloc(sizeof(VizitaMedicala) * ((*nrVizite) + 1));
	for (int i = 0;i < (*nrVizite);i++) {
		vector[i] = (*vizite)[i];
	}
	vector[(*nrVizite)] = vizitaNoua;
	vector[(*nrVizite)].numePacient = malloc(sizeof(char) * (strlen(vizitaNoua.numePacient) + 1));
	strcpy_s((vector)[*nrVizite].numePacient, strlen(vizitaNoua.numePacient) + 1, vizitaNoua.numePacient);

	vector[(*nrVizite)].diagnostic = malloc(sizeof(char) * (strlen(vizitaNoua.diagnostic) + 1));
	strcpy_s((vector)[*nrVizite].diagnostic, strlen(vizitaNoua.diagnostic) + 1, vizitaNoua.diagnostic);
	(*nrVizite)++;
	free(*vizite);
	(*vizite) = vector;
}

VizitaMedicala citireViziteFisier(FILE* file) {

	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;

	VizitaMedicala vizita;
	aux = strtok(buffer, sep);
	vizita.id = atoi(aux);

	aux = strtok(NULL, sep);
	vizita.numePacient = malloc(strlen(aux) + 1);
	strcpy_s(vizita.numePacient, strlen(aux) + 1, aux);

	vizita.cost = atof(strtok(NULL, sep));
	vizita.varsta = atoi(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	vizita.diagnostic = malloc(strlen(aux) + 1);
	strcpy_s(vizita.diagnostic, strlen(aux) + 1, aux);

	return vizita;


}

VizitaMedicala* citireVectorViziteFisier(const char* numeFisier, int* nrViziteCitite) {
	FILE* f = fopen(numeFisier, "r");
	VizitaMedicala* vizite = NULL;
	while (!feof(f)) {
		VizitaMedicala v = citireViziteFisier(f);
		adaugaViziteInVector(&vizite, nrViziteCitite, v);
		free(v.numePacient);
		free(v.diagnostic);

	}

	fclose(f);
	return vizite;
}


//functie ce salveaza un obiect in fisier
void salveazaVizitaInFisier(VizitaMedicala vizita, const char* numeFisier) {
	FILE* f = fopen(numeFisier, "a");
	fprintf(f, "\n%d,%s,%.2f,%d,%s\n", vizita.id, vizita.numePacient, vizita.cost, vizita.varsta, vizita.diagnostic);
	fclose(f);
}


void dezalocareVectorVizite(VizitaMedicala** vector, int* nrVizite) {
	for (int i = 0; i < *nrVizite; i++) {
		free((*vector)[i].numePacient);
		free((*vector)[i].diagnostic);
	}
	free(*vector);
	*vector = NULL;
	*nrVizite = 0;
}



int main() {
	VizitaMedicala* vizite;
	int nrVizite = 0;
	vizite = citireVectorViziteFisier("vizite.txt", &nrVizite);
	afisareVectorVizite(vizite, nrVizite);

	//adaugare obiect in fisier
	VizitaMedicala vizitaNoua = { 20, "Marian", 300.0, 40, "Hemoragie" };
	salveazaVizitaInFisier(vizitaNoua, "vizite.txt");

	dezalocareVectorVizite(&vizite, &nrVizite);


	return 0;
}