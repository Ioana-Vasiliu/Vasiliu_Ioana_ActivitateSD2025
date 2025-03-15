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



int main() {

	//apel citire tastatura
	VizitaMedicala vizita = citesteVizita();

	afisareVizita(vizita);



	free(vizita.numePacient);
	free(vizita.diagnostic);



	return 0;
}

