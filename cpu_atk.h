#ifndef _cpu_atk_h
#define _cpu_atk_h

#include <iostream>
#include "tavolo_da_gioco.h"

class Tavolo_da_gioco; 

using namespace std;

class Cpu_atk {
    
    friend class Tavolo_da_gioco;
    
private:
    
    int n_mosse_cpu;
    int livello_diff;
    
public:
    
    class Nodo {
        
    public:
        ~Nodo(){}
        Nodo* figlio[7];
        short int game_matrix[6][7];
        float punteggio_nodo;
        bool controllo_inserimento;
    };
    
    Cpu_atk(int livello_diff_){
        livello_diff = livello_diff_;
        n_mosse_cpu = 0;
    }
    
      int Mossa_successiva();
     void Stampa_tavolo(Nodo* node);
     void Risali_livello(Nodo* principale,int x, int level, int min, int max);
     void Inserimento(int coordinata_colonna, Nodo* secondario, int pedina);
     void Dealloca(Nodo* principale,int level);
     void Generazione_albero(Nodo* principale, int livello, int x,int controllo);
     bool Caso_critico(Nodo* principale);
    float Euristica(Nodo* secondario, int x);
    
};

#endif