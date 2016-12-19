#include <iomanip>
#include <string>

#include "tavolo_da_gioco.h"
#include "giocatore.h"


using namespace std;


// COSTRUTTORE DEI GIOCATORI //

class Giocatore;

Area_di_gioco* Area_di_gioco::area = NULL;


Area_di_gioco::Area_di_gioco() {
    for(int i=0; i<numero_righe; i++){
        for(int j=0; j<numero_colonne; j++){
            matrice_di_gioco[i][j]=-1;
        }
    }
    
    lettere_coordinate_verticali[0] = "A";
    lettere_coordinate_verticali[1] = "B";
    lettere_coordinate_verticali[2] = "C";
    lettere_coordinate_verticali[3] = "D";
    lettere_coordinate_verticali[4] = "E";
    lettere_coordinate_verticali[5] = "F";
    
    numero_mosse = 0;
    vittoria = false;
    pareggio = false;
}


// STAMPA TAVOLO SU DISPLAY //

void Area_di_gioco::Stampa_tavolo(){
    for(int riga = 0;riga < numero_righe;riga++) {
        cout << lettere_coordinate_verticali[riga];
        for(int colonna = 0;colonna < numero_colonne;colonna++) {
            if(matrice_di_gioco[riga][colonna] < 0)
				cout  << "\e[0;37m"<< "|   ";
            else if(matrice_di_gioco[riga][colonna] == 0)
				cout  << "|"<<"\e[0;33m"<<" O "<<"\e[0;37m";
            else if(matrice_di_gioco[riga][colonna] > 0)
				cout  << "|"<<"\e[0;91m"<<" X "<<"\e[0;37m";				
        }
        cout << "|"<<endl;
    }
    cout<<" ";
    
    for(int riga_ = 0; riga_ < numero_colonne; riga_++) {
        cout << setw( spaziatura_caratteri) << " " << riga_ << " ";
    }
    cout<<endl;
}


// INSERIMENTO NELLA MATRICE PER UN PLAYER UMANO //

void Area_di_gioco::Inserimento(int coordinata_colonna, Giocatore* player){
	if(numero_mosse < 42){
        if(coordinata_colonna <= 6){
            int i = 5;
            if(matrice_di_gioco[0][coordinata_colonna]  == -1){
                while((matrice_di_gioco[i][coordinata_colonna] != -1)&&(i >= 0)){
                    i--;
                }
                matrice_di_gioco[i][coordinata_colonna] = player->primo_giocatore;
                numero_mosse++;
                if(Controllo_vittoria(player) == true){
                    vittoria = true;
                }
            }
            else {
                cout<<"colonna piena!\n";
                player->Inserimento();
            }
		}
        else{
            cout<<"colonna nn valida!\n";
            player->Inserimento();
        }
	}
	else if(numero_mosse == 41){
		cout<<"Matrice Piena\n"<<endl<<"gioco terminato.";
		vittoria = true;	
	}
}


// INSERIMENTO NELLA MATRICE PER UN PLAYER CPU //


void Area_di_gioco::Inserimento(int coordinata_colonna, Giocatore* player, bool cpu){
	if(numero_mosse < 42){
        if(coordinata_colonna <= 6){
            int i = 5;
            if(matrice_di_gioco[0][coordinata_colonna]  == -1){
                while((matrice_di_gioco[i][coordinata_colonna] != -1)&&(i >= 0)){
                    i--;
                }
                matrice_di_gioco[i][coordinata_colonna] = player->primo_giocatore;
                numero_mosse++;
                if(Controllo_vittoria(player) == true){
                    vittoria = true;
                }
            }
        }
    }
}


// CONTROLLO CHE STABILISCE SE UNA PARTITA E' TERMINATA //

bool Area_di_gioco::Controllo_vittoria (Giocatore* player) {
    int k = player->primo_giocatore; // k stabilisce quale giocatore deve controllare
    
    if (numero_mosse < 7)
        return false;
    
    //controllo verticale //
    for (int r=numero_righe-1; r>=3; r--) {
        for (int c=numero_colonne-1; c>=0; c--) {
            if(matrice_di_gioco[r][c] == k){
                if((matrice_di_gioco[r-1][c]==k) && (matrice_di_gioco[r-2][c]==k) && (matrice_di_gioco[r-3][c]== k)){
                    Area_di_gioco::Istanza_tavolo()->Stampa_tavolo();
                    cout<< player->Ottieni_nome_giocatore()<<" ha vinto verticalmente\n";
                    return true;
                }
            }
        }
    }
    //controllo orizzontale //
    for (int r=numero_righe-1; r>=0; r--) {
        for (int c=numero_colonne-1; c>=3; c--) {
            if(matrice_di_gioco[r][c] == k){
                if((matrice_di_gioco[r][c-1]==k) && (matrice_di_gioco[r][c-2]==k) && (matrice_di_gioco[r][c-3]==k)){
                    Area_di_gioco::Istanza_tavolo()->Stampa_tavolo();
                    cout<< player->Ottieni_nome_giocatore()<<" ha vinto orizzontalmente\n";
                    return true;
                }
            }
        }
    }
    //controllo diagonale sinista  //
    for (int r=numero_righe-1; r>=3; r--) {
        for (int c=numero_colonne-1; c>=3; c--) {
            if(matrice_di_gioco[r][c] == k){
                if((matrice_di_gioco[r-1][c-1]==k) && (matrice_di_gioco[r-2][c-2]==k) && (matrice_di_gioco[r-3][c-3]==k)){
                    Area_di_gioco::Istanza_tavolo()->Stampa_tavolo();
                    cout<< player->Ottieni_nome_giocatore()<<" ha vinto con diagonale sinistra\n";
                    return true;
                }
            }
        }
    }
    //controllo diagonale destra //
    for (int r=numero_righe-1; r>=3; r--) {
        for (int c=numero_colonne-4; c>=0; c--) {
            if(matrice_di_gioco[r][c] == k){
                if((matrice_di_gioco[r-1][c+1]==k) && (matrice_di_gioco[r-2][c+2]==k) && (matrice_di_gioco[r-3][c+3]==k)){
                    Area_di_gioco::Istanza_tavolo()->Stampa_tavolo();
                    cout<< player->Ottieni_nome_giocatore()<<" ha vinto diagonale desta\n";
                    return true;
                }
            }
        }
    }
    
    return false;
}




