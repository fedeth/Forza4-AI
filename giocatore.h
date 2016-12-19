#ifndef Giocatore_H
#define Giocatore_H
#define null 0

#include <iostream>
#include <string>

class Giocatore {	
    
private:
    std::string nome_giocatore;
	
public:
	
	bool cpu;
	int primo_giocatore;
	
	Giocatore(std::string nome_giocatore_);
	void Inserimento();
	void Inserimento(int inserimento_cpu);
    std::string Ottieni_nome_giocatore(){
        return nome_giocatore;
    }
};

#endif