#ifndef _cpu_def_h
#define _cpu_def_h

#include <iostream>
#include "tavolo_da_gioco.h"

class Tavolo_da_gioco; 


class Cpu_def {
    
    friend class Tavolo_da_gioco;
    
private:
    
    
    int n_mosse_cpu;
    int livello_diff;
        
    class Nodo {
        
    public:
        
        Nodo* figlio[7];
        short int game_matrix[6][7];
        float punteggio_nodo;
        bool controllo_inserimento;
        Nodo(){
            memset(this, 0, sizeof(Nodo));
        }
        ~Nodo(){
            for(int i=0;i<7;i++){
                if(figlio[i] != NULL)
                    delete figlio[i];
                }
        }
    };
    
public:
    Cpu_def(int livello_diff_){
        livello_diff=livello_diff_;
        n_mosse_cpu = 0;
        numer = 0;
    }
    
     int numer;
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