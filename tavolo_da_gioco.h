#ifndef Tavolo_da_gioco_H
#define Tavolo_da_gioco_H
#define null 0

#include <iostream>


class Giocatore;

class Area_di_gioco{
    
    friend class Cpu_atk;
    friend class Cpu_def;
	
private:
	
	short int matrice_di_gioco[6][7];
	static const int numero_righe = 6;
	static const int numero_colonne = 7;
	static const int spaziatura_caratteri= 2;
	int numero_mosse;
	
	static Area_di_gioco* area;
	
    std::string lettere_coordinate_verticali[6];
	
	Area_di_gioco();
    
public: 
    
	bool vittoria;
    bool pareggio;
	
    ~Area_di_gioco() {};
	
	void Stampa_tavolo();
	void Inserimento(int coordinata_colonna, Giocatore* player);	
	void Inserimento(int coordinata_colonna, Giocatore* player, bool cpu);
	bool Controllo_vittoria(Giocatore* player);
	
	inline int Ottieni_numero_mosse(){
		return numero_mosse;
	}
	
	static Area_di_gioco* Istanza_tavolo() {
        if (area == NULL) {
            area = new Area_di_gioco;
            
        }
        return area;
	}
    
    static Area_di_gioco* Ottieni_area_di_gioco(){
       static Area_di_gioco* puntatore_area_di_gioco = Istanza_tavolo();
        return puntatore_area_di_gioco;
        
    }

   

};



#endif