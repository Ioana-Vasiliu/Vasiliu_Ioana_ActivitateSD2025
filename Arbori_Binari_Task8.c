#define _CRT_SECURE_NO_WARNINGS
#include<malloc.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

typedef struct Rezervare {
	unsigned int idRezervare;
	char* denumireHotel;
	int nrCamereRezervate;
	float suma;

}Rezervare;

Rezervare creareRezerve(unsigned int idRezervare, const char* denumireHotel, int nrCamereRezervate, float suma) {
	Rezervare r;

	r.idRezervare = idRezervare;
	r.denumireHotel = (char*)malloc(sizeof(char) * strlen(denumireHotel) + 1);
	strcpy(r.denumireHotel, denumireHotel);

	r.nrCamereRezervate = nrCamereRezervate;
	r.suma = suma;
	return r;
}

typedef struct Nod {
	Rezervare info;
	struct Nod* st;
	struct Nod* dr;
}Nod;

Nod* creareNod(Rezervare r) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = creareRezerve(r.idRezervare, r.denumireHotel, r.nrCamereRezervate, r.suma);

	nou->st = NULL;
	nou->dr = NULL;
	return nou;
}

Nod* inserare(Rezervare r, Nod* rad) {
	if (rad) {
		if (r.idRezervare < rad->info.idRezervare) {
			rad->st = inserare(r, rad->st);
			return rad;
		}

		else if (r.idRezervare > rad->info.idRezervare) {
			rad->dr = inserare(r, rad->dr);
			return rad;
		}
		else {
			return rad;
		}
	}
	else {
		return creareNod(r);
	}
}

void afisarePreordine(Nod* rad) {
	if (rad) {
		printf("%u, %s, %d, %.2f\n", rad->info.idRezervare, rad->info.denumireHotel, rad->info.nrCamereRezervate, rad->info.suma);
		afisarePreordine(rad->st);
		afisarePreordine(rad->dr);
	}
}

void dezalocare(Nod* rad) {
	if (rad) {
		dezalocare(rad->dr);
		dezalocare(rad->st);
		free(rad->info.denumireHotel);
		free(rad);
	}

}


Nod* gasesteMinim(Nod* rad) {
	while (rad->st != NULL) {
		rad = rad->st;
	}
	return rad;
}

//Stergere nod dupa idRezervare, mentinand proprietatea de arbore binar de cautare
Nod* stergereNod(Nod* rad, unsigned int id) {
	if (rad) {
		if (id < rad->info.idRezervare) {
			rad->st = stergereNod(rad->st, id);
		}
		else if (id > rad->info.idRezervare) {
			rad->dr = stergereNod(rad->dr, id);
		}
		else {
			if (rad->st == NULL) {
				Nod* temp = rad->dr;
				free(rad->info.denumireHotel);
				free(rad);
				return temp;
			}
			else if (rad->dr == NULL) {
				Nod* temp = rad->st;
				free(rad->info.denumireHotel);
				free(rad);
				return temp;
			}
			else {
				Nod* aux = gasesteMinim(rad->dr);
				free(rad->info.denumireHotel);
				rad->info = creareRezerve(aux->info.idRezervare, aux->info.denumireHotel, aux->info.nrCamereRezervate, aux->info.suma);
				rad->dr = stergereNod(rad->dr, aux->info.idRezervare);
			}
		}

		return rad;
	}
}

int maxim(int a, int b) {
	return (a > b ? a : b);
}

//inaltime arbore
int inaltimeArb(Nod* rad) {
	if (rad) {
		return(1 + maxim(inaltimeArb(rad->dr), inaltimeArb(rad->st)));
	}
	else {
		return 0;
	}

}

// Functie care returneaza subarborele stang sau drept cu inaltime mai mare
Nod* subarboreInalt(Nod* rad) {
	if (rad) {

		int inaltimeSt = inaltimeArb(rad->st);
		int inaltimeDr = inaltimeArb(rad->dr);

		if (inaltimeSt > inaltimeDr)
			return rad->st;
		else if (inaltimeDr > inaltimeSt)
			return rad->dr;
		else
			return 0;
	}
}

// Functie care calculeaza numarul de noduri in subordine
int numarNoduriSubordine(Nod* rad) {
	if (rad) {
		int st = numarNoduriSubordine(rad->st);
		int dr = numarNoduriSubordine(rad->dr);
		return st + dr + 1;
	}
}

// Functie care returneaza subarborele stang sau drept cu mai multe noduri in subordine
Nod* subarboreMaiMulteNoduri(Nod* rad) {
	if (rad) {
		int nrSt = numarNoduriSubordine(rad->st);
		int nrDr = numarNoduriSubordine(rad->dr);
		return (nrSt >= nrDr) ? rad->st : rad->dr;
	}
}

void main() {
	Nod* rad = NULL;

	Rezervare r;

	char buffer[100];
	char sep[] = ",\n";
	char* token;

	FILE* f = fopen("rezervari.txt", "r");
	while (fgets(buffer, 100, f)) {
		token = strtok(buffer, sep);

		r.idRezervare = atoi(token);
		token = strtok(NULL, sep);

		r.denumireHotel = (char*)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(r.denumireHotel, token);
		token = strtok(NULL, sep);
		r.nrCamereRezervate = atoi(token);
		token = strtok(NULL, sep);
		r.suma = atof(token);

		rad = inserare(r, rad);

	}
	fclose(f);
	afisarePreordine(rad);


	printf("\n--Stergere nod cu id 2-- \n");
	rad = stergereNod(rad, 2);
	afisarePreordine(rad);

	printf("\nSubarborele cu inaltime mai mare:\n");
	Nod* subarobre = subarboreInalt(rad);
	afisarePreordine(subarobre);

	printf("\nNumar noduri subordine: %d\n", numarNoduriSubordine(rad));

	printf("\nSubarborele cu mai multi descendenti: \n");
	Nod* subarbarboreDesc = subarboreMaiMulteNoduri(rad);
	afisarePreordine(subarbarboreDesc);

	dezalocare(rad);
}