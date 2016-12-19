#include <iostream>

#include "giocatore.h"
#include "tavolo_da_gioco.h"


#define null 0

using namespace std;

class Giocatore;

Giocatore::Giocatore(string nome_giocatore_){
    
    nome_giocatore = nome_giocatore_;
    
    // stabilisce e distingue il primo giocatore dall'altro //
    static int numero_giocatore;
    if(numero_giocatore == 0)
		primo_giocatore = 1;
    else
		primo_giocatore = 0;
    
    numero_giocatore++;
}


void Giocatore::Inserimento(){
    
    char coordinata_colonna;
    int coordinata;
    do{
        cout<<"inserisci colonna:  ";
        cin>>coordinata_colonna;
        coordinata = static_cast<int>(coordinata_colonna);
        coordinata = coordinata - 48;
        
        if ((coordinata>=0)&&(coordinata<=6)) {
            Area_di_gioco::Istanza_tavolo()->Inserimento(coordinata, this);
            
        }
    } while((coordinata < 0) || (coordinata >6));
}

void Giocatore::Inserimento(int inserimento_cpu){
    
    int coordinata_colonna = inserimento_cpu;
    Area_di_gioco::Istanza_tavolo()->Inserimento(coordinata_colonna, this, cpu);
}




