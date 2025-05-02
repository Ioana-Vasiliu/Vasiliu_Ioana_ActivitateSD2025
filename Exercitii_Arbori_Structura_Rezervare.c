#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

struct Rezervare {
	unsigned int idRezervare;
	char* denumireHotel;
	unsigned char nrCamereRezervate;
	char* numeClient;
	float sumePlataRON;
};
typedef struct Rezervare rezervare;

rezervare creareRezervare(unsigned int idRezervare, const char* denumireHotel, unsigned char nrCamereRezervate, const char* numeClient, float sumePlataRON) {
	rezervare r;

	r.idRezervare = idRezervare;
	r.denumireHotel = malloc(sizeof(char) * (strlen(denumireHotel)) + 1);
	strcpy_s(r.denumireHotel, (strlen(denumireHotel)) + 1, denumireHotel);
	r.nrCamereRezervate = nrCamereRezervate;
	r.numeClient = malloc(sizeof(char) * (strlen(numeClient)) + 1);
	strcpy_s(r.numeClient, (strlen(numeClient)) + 1, numeClient);
	r.sumePlataRON = sumePlataRON;

	return r;
}

//functie inserare
//pas 1 - facem nod  (adresa nod stang, info, adresa nod drept);
typedef struct Nod nod;
struct Nod {
	rezervare info;
	nod* nodStanga;
	nod* nodDreapta;
};


//pas 2 - creare nod

nod* CreareNod(rezervare r) {
	nod* nou = malloc(sizeof(nod));
	nou->info = creareRezervare(r.idRezervare, r.denumireHotel, r.nrCamereRezervate, r.numeClient, r.sumePlataRON);
	nou->nodStanga = NULL;
	nou->nodDreapta = NULL;
	return nou;
}

//pas 3 - inserarea efectiva (if inserare stanga, if inserare dreapta, if pentru egalitate
nod* InserareNod(rezervare r, nod* radacina) {
	if (radacina) {
		if (r.idRezervare < radacina->info.idRezervare) {
			radacina->nodStanga = InserareNod(r, radacina->nodStanga);
			return radacina;
		}
		else if (r.idRezervare > radacina->info.idRezervare)
		{
			radacina->nodDreapta = InserareNod(r, radacina->nodDreapta);
			return radacina;
		}
		else {
			return radacina;
		}
	}
	else {
		return CreareNod(r);
	}

}
//nr total frunze
void nrTotalFrunze(nod* radacina, int* nr) {
	if (radacina) {
		if (radacina->nodDreapta == NULL && radacina->nodStanga == NULL) {
			(*nr)++;
		}

		nrTotalFrunze(radacina->nodDreapta, nr);
		nrTotalFrunze(radacina->nodStanga, nr);
	}

}

//functia care determina nr total de camere rezervate din nodurile frunza -> fara fiu 
void nrTotalCamere(nod* radacina, int* nrCamere) {
	if (radacina) {
		if (radacina->nodDreapta == NULL && radacina->nodStanga == NULL) {
			*nrCamere += radacina->info.nrCamereRezervate;
		}

		nrTotalCamere(radacina->nodDreapta, nrCamere);
		nrTotalCamere(radacina->nodStanga, nrCamere);
	}

}


int maxim(int a, int b) {
	return a > b ? a : b;

}

//functia care determina inaltimea structurii arborescente ca numar de niveluri
int inaltimeArbore(nod* radacina) {
	if (radacina) {
		return (1 + maxim(inaltimeArbore(radacina->nodStanga), inaltimeArbore(radacina->nodDreapta)));
	}
	else {
		return 0;
	}

}

void afisareArborePreordine(nod* radacina) {
	if (radacina) {
		printf("%u, %s, %u, %s, %.2f \n", radacina->info.idRezervare, radacina->info.denumireHotel, radacina->info.nrCamereRezervate, radacina->info.numeClient, radacina->info.sumePlataRON);

		afisareArborePreordine(radacina->nodStanga);
		afisareArborePreordine(radacina->nodDreapta);
	}
}

void afisareArboreInordine(nod* radacina) {
	if (radacina) {

		afisareArboreInordine(radacina->nodStanga);
		printf("%u, %s, %u, %s, %.2f \n", radacina->info.idRezervare, radacina->info.denumireHotel, radacina->info.nrCamereRezervate, radacina->info.numeClient, radacina->info.sumePlataRON);
		afisareArboreInordine(radacina->nodDreapta);
	}
}

void afisareArborePostordine(nod* radacina) {
	if (radacina) {

		afisareArborePostordine(radacina->nodStanga);
		afisareArborePostordine(radacina->nodDreapta);
		printf("%u, %s, %u, %s, %.2f \n", radacina->info.idRezervare, radacina->info.denumireHotel, radacina->info.nrCamereRezervate, radacina->info.numeClient, radacina->info.sumePlataRON);
	}
}


int main() {
	rezervare r1 = creareRezervare(3, "Hilton", 3, "Ion", 125.5);
	rezervare r2 = creareRezervare(1, "Inter", 7, "Vasile", 125.5);
	rezervare r3 = creareRezervare(6, "Continental", 13, "Andrei", 125.5);
	rezervare r4 = creareRezervare(7, "Steaua", 5, "Mihai", 189.5);
	rezervare r5 = creareRezervare(2, "Ramada", 1, "Ion", 125.5);
	rezervare r6 = creareRezervare(5, "Oltenita", 12, "Ion", 125.5);


	nod* radacina = NULL;

	radacina = InserareNod(r1, radacina);
	radacina = InserareNod(r2, radacina);
	radacina = InserareNod(r3, radacina);
	radacina = InserareNod(r4, radacina);
	radacina = InserareNod(r5, radacina);
	radacina = InserareNod(r6, radacina);


	afisareArborePreordine(radacina);

	int nrFrunze = 0;

	nrTotalFrunze(radacina, &nrFrunze);
	printf("\nNumar frunze %d", nrFrunze);

	int nrCamere = 0;

	nrTotalCamere(radacina, &nrCamere);
	printf("\nNumere camere din frunze: %d", nrCamere);

	printf("\nInaltimea arborelui %d", inaltimeArbore(radacina));

	return 0;
}
