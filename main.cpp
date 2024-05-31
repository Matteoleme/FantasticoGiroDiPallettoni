#include<iostream>
#include<cstring>
#include<cstdio>
#include<ctime>
#include <unistd.h>
#include <cstdlib>
#include <math.h>

using namespace std;

struct Giocatore{
	char nome[5];
	int vita;
};

struct Fucile{
	bool* caricatore;			// ricordati di allocare memoria altrimenti crasha
	int colpiTotali;
	//int colpiSparati = 0;		// lo devi incrementare ogni colpo
	int colpiVeri;
};

struct Partita{
	Giocatore player;
	Giocatore ai;
	Fucile fucile;
};


// Stampa le informazioni di benvenuto e introduce il gioco al giocatore
void stampaBenvenuto();

// Permette di scegliere il nome
void inserisciNome(char nome[]);

void stampaGiocatore(Giocatore g);

void mostraInformazioni(Partita partita);

void inizializzaVite(Partita &partita, int vite);

void inizializzaRound(Partita &partita, int colpi);

void caricaFucile(Fucile &fucile);

bool giaInserito(int indici[], int dim, int randomico);

void stampaProiettili(Fucile fucile);

void stampaCaricatore(bool* caricatore, int d);

int main(){
	// inizializzazione partita
	Partita partita;
	stampaBenvenuto();
	// inizializzo il seed
	srand(time(0));
	// inizializzazione giocatori
	Giocatore noi;
	Giocatore computer;
	inserisciNome(noi.nome);
	partita.player = noi;
	partita.ai = computer;
	strcpy(partita.ai.nome, "AI");
	// inizializzo le vite a 2 all'inizio
	inizializzaVite(partita, 2);
	mostraInformazioni(partita);

	// La partita termina
	int round = 1;
	int buffer;
	do{
		inizializzaRound(partita, round+2);
		round++;
		stampaCaricatore(partita.fucile.caricatore, partita.fucile.colpiTotali);
		cin >> buffer;

		free(partita.fucile.caricatore);
	}while(partita.player.vita > 0 && partita.ai.vita > 0);
	
	return 0;
}


void stampaBenvenuto(){
	cout << "Benvenuto ora giocheremo insieme... :)\n";
	cout << "Inizializzazione partita...\n";
}

void inserisciNome(char nome[]){
	cout << "Inserire un nome: ";
	cin >> nome;
}

void mostraInformazioni(Partita partita){
	cout << "Tu: \n";
	stampaGiocatore(partita.player);
	cout << "Avversario: \n";
	stampaGiocatore(partita.ai);
	cout << endl;
}

void stampaGiocatore(Giocatore g){
	cout << "Nome: " << g.nome;
	cout << " - Vite: " << g.vita << endl;
}

void inizializzaVite(Partita &partita, int vite){
	partita.ai.vita = vite;
	partita.player.vita = vite;
}

void inizializzaRound(Partita &partita, int colpi){
	// in questa funzione devo caricare il fucile 
	partita.fucile.caricatore = (bool*) malloc(sizeof(bool)*colpi);
	partita.fucile.colpiTotali = colpi;
	caricaFucile(partita.fucile);		// Ora il fucile è carico

	stampaProiettili(partita.fucile);
	// Eventuale scelta di gadget
}

void caricaFucile(Fucile &fucile){
	// prendo in input un array già allocato di n elementi
	bool *caricatore = fucile.caricatore;
	int colpi = fucile.colpiTotali;
	// prima creo un fucile "deterministico"
	bool* deterministico = (bool*) malloc(sizeof(bool)*colpi);
	// carico l'array con i colpi in modo deterministico e poi dopo li mischio casualmente
	
	for(int i=0; i<colpi; i++){
		if(i<ceil(colpi/2))
			deterministico[i] = false;
		else
			deterministico[i] = true;
	}
	fucile.colpiVeri = fucile.colpiTotali - (colpi/2);		// serve per stampare quanti colpi ho a salve e veri

	// inizializzo l'array degli indici
	int indici[colpi];
	// adesso carico l'array di colpi vero
	for(int i=0; i<colpi; i++){
		// mi scorro l'array deterministico
		int indiceRandomico = rand() % colpi;
		// scorro l'array di colpi
		if(giaInserito(indici, i, indiceRandomico))
			i--;			// soluzione sporca, al limite implementa un while
		else
			caricatore[indiceRandomico] = deterministico[i];
	}
}


bool giaInserito(int indici[], int dim, int randomico){
	for(int i=0; i<dim; i++){
		if(indici[i]==randomico)
			return true;
	}
	return false;
}


void stampaProiettili(Fucile fucile){
	cout << "Proiettili totali: " << fucile.colpiTotali << endl;
	cout << "     veri: " << fucile.colpiVeri << endl;
	cout << "     a salve: " << fucile.colpiTotali - fucile.colpiVeri << endl;
}


void stampaCaricatore(bool* caricatore, int d){
	for(int i=0; i<d; i++){
		cout << "[" << caricatore[i] << "]";
	}
	cout << endl;
}
