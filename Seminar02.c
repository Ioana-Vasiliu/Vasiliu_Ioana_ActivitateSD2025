#include<stdio.h>
#include<malloc.h>

struct Masina {
	int id;
	char* model;
	float tonaj;
	char serie;
}Masina;

struct Masina initializare(int idNou, const char* modelNou, float tonajNou, char serieNou) {
	struct Masina m;
	m.id = idNou;
	m.model = malloc(sizeof(char) * (strlen(modelNou)) + 1);
	strcpy_s(m.model, (strlen(modelNou)) + 1, modelNou);
	m.tonaj = tonajNou;
	m.serie = serieNou;
	return m;
}

void afisare(struct Masina m) {
	printf("Id: %d \n Model: %s \n Tonaj:%.2f \n Serie: %c \n", m.id, m.model, m.tonaj, m.serie);
}

void afisareVector(struct Masina* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}

struct Masina* copiazaPrimeleMasini(struct Masina* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna, primele nrElementeCopiate
	struct Masina* vectorNou = malloc(sizeof(struct Masina) * nrElementeCopiate);
	for (int i = 0; i < nrElementeCopiate; i++) {
		vectorNou[i] = vector[i];
		vectorNou[i].model = malloc(sizeof(char) * (strlen(vector[i].model) + 1));
		strcpy_s(vectorNou[i].model, (strlen(vector[i].model) + 1), vector[i].model);
	}

	return vectorNou;
}


void dezalocare(struct Masina** vector, int* nrElemente) {
	//dezalocam elementele din vector si vectorul
	for (int i = 0;i < *nrElemente;i++) {
		free((*vector)[i].model);
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaMasiniUsoare(struct Masina* vector, char nrElemente, float prag, struct Masina** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie

	(*dimensiune) = 0;

	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].tonaj < prag) {
			(*dimensiune)++;
		}
	}
	(*vectorNou) = malloc(sizeof(struct Masina) * (*dimensiune));

	int k = 0;

	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].tonaj < prag) {
			(*vectorNou)[k] = vector[i];
			(*vectorNou)[k].model = malloc(sizeof(char) * (strlen(vector[i].model) + 1));
			strcpy_s((*vectorNou)[k].model, (strlen(vector[i].model) + 1), vector[i].model);
			k++;
		}
	}


}

//trebuie cautat elementul care indeplineste o conditie
//dupa atributul de tip char*, acesta trebuie returnat

struct Masina getPrimaMasina(struct Masina* vector, int nrElemente, const char* conditie) {
	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(vector[i].model, conditie) == 0) {
			return vector[i];

		}


	}
	struct Masina masinaInvalida;
	masinaInvalida.id = -1;
	masinaInvalida.model = malloc(sizeof(char));
	masinaInvalida.model[0] = ' ';
	masinaInvalida.tonaj = 0.0;
	masinaInvalida.serie = ' ';

	return masinaInvalida;
	

}

int main() {

	struct Masina m;
	m = initializare(1, "BMW", 2.3, '1');
	afisare(m);

	int nrMasini = 4;

	struct Masina* vectorMasini;
	vectorMasini = malloc(sizeof(struct Masina) * nrMasini);

	vectorMasini[0] = initializare(2, "Mercedes", 2.2, 'C');
	vectorMasini[1] = initializare(3, "Audi", 2.6, 'S');
	vectorMasini[2] = initializare(4, "BMW", 1.8, '5');
	vectorMasini[3] = initializare(5, "BMW", 2.2, '3');

	afisareVector(vectorMasini, nrMasini);

	struct Masina* vectorMasiniCopiate;
	vectorMasiniCopiate = NULL;
	int nrElementeCopiate = 2;
	///copiazaPrimeleNElemente(struct Masina* vector, int nrElemente, int nrElementeCopiate)
	vectorMasiniCopiate = copiazaPrimeleMasini(vectorMasini, nrMasini, nrElementeCopiate);
	printf("\n Masini copiate \n");
	afisareVector(vectorMasiniCopiate, nrElementeCopiate);

	dezalocare(&vectorMasiniCopiate, &nrElementeCopiate);


	printf("\n Masini usoare \n");
	copiazaMasiniUsoare(vectorMasini, nrMasini, 2.5, &vectorMasiniCopiate, &nrElementeCopiate);
	afisareVector(vectorMasiniCopiate, nrElementeCopiate);

	dezalocare(&vectorMasiniCopiate, &nrElementeCopiate);

	printf("\n Prima masina gasita:  \n");
	struct Masina masinaGasita = getPrimaMasina(vectorMasini, nrMasini, "BMW");
	afisare(masinaGasita);



	return 0;
}