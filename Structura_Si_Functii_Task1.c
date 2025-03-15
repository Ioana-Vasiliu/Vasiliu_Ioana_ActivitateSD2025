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


//Citirea de la tastatura 
VizitaMedicala citesteVizita() {
	VizitaMedicala vizita;
	char aux[100];
	char auxf[100];

	printf("Introduceti id-ul: ");
	scanf("%d", &vizita.id);

	printf("Numele pacientului este : ");
	scanf(" %[^\n]", aux);
	vizita.numePacient = (char*)malloc(strlen(aux) + 1);
	strcpy_s(vizita.numePacient, strlen(aux) + 1,aux);

	printf("Costul vizitei este de: ");
	scanf("%f", &vizita.cost);

	printf("Varsta pacientului este: ");
	scanf("%d", &vizita.varsta);

	printf("Diagnosticul pacientului este de: ");
	scanf(" %[^\n]", auxf);
	vizita.diagnostic = (char*)malloc(strlen(auxf) + 1);
	strcpy_s(vizita.diagnostic, strlen(auxf) + 1, auxf);
	

	return vizita;


}

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


//reduce costul consultatiei cu 20%  daca diagnosticul este Diabet Zaharat
void modificaCostul(struct VizitaMedicala* v) {
	if (strcmp(v->diagnostic, "Diabet zaharat") == 0) {
		v->cost *= 0.8;
	}
}




int main() {

	//apel citire tastatura
	//VizitaMedicala vizita = citesteVizita();

	//afisareVizita(vizita);



	struct VizitaMedicala vizita1 = initializareVizita(1, "Ion", 200.0, 45, "Diabet zaharat");
	struct VizitaMedicala vizita2 = initializareVizita(2, "Maria", 150.5, 38, "Raceala");
	struct VizitaMedicala vizita3 = initializareVizita(3, "Alex", 175.75, 50, "Diabet zaharat");


	modificaCostul(&vizita1);
	afisareVizita(vizita1);




	//free(vizita.numePacient);
	//free(vizita.diagnostic);
	return 0;
}

