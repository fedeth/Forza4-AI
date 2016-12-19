#include <iomanip>


#include "cpu_atk.h"
#include "tavolo_da_gioco.h"


using namespace std;

////    Funzione che ritorna al main la mossa che deve compiere l' IA    ////
int Cpu_atk::Mossa_successiva(){
    
    ++n_mosse_cpu;
    int min=90000;
    int max=-90000;
    int colonna;
    int livello=livello_diff;
    
    
    Nodo* principale = new Nodo;
    
    
    for(int riga = 0;riga < 6;riga++) {
        for(int colonna = 0;colonna < 7;colonna++) {
            principale->game_matrix[riga][colonna] = Area_di_gioco::Ottieni_area_di_gioco()->matrice_di_gioco[riga][colonna];
        }
    }
    // prima mossa viene inserita sempre nella colonna 3 //
    if(n_mosse_cpu == 1)
        return 3;
    
    Generazione_albero(principale,livello_diff,0,1);
    
    // controllo se vince imminentemente //
    for (int i=0 ; i < 7 ; i++) {
        if(Area_di_gioco::Ottieni_area_di_gioco()->matrice_di_gioco[0][i]== -1){ 
            if(principale->figlio[i]->punteggio_nodo == -90000){
                return i;                
            }
        }
    }
    
    Risali_livello(principale,1,livello_diff,min,max);
    

    // ritorna il valore minimo di tutti i figli del primo livello (questo valore viene preso dalla funzione Risali_livello) //
    for (int i=0 ; i < 7 ; i++) {
        if(Area_di_gioco::Ottieni_area_di_gioco()->matrice_di_gioco[0][i]== -1){ 
            if(principale->figlio[i]->punteggio_nodo <= min){
                min = principale->figlio[i]->punteggio_nodo;
                colonna = i;
                
            }
        }
    }
    
    // Gestione delle eccezioni //
    
    do{ 
        if(Caso_critico(principale) == true){
            
            Generazione_albero(principale,livello-1,0,1);
            Risali_livello(principale,1,livello-1,min,max);
            
            for (int i=0 ; i < 7 ; i++) {
                if(Area_di_gioco::Ottieni_area_di_gioco()->matrice_di_gioco[0][i]== -1){ 
                    if(principale->figlio[i]->punteggio_nodo < min){
                        min = principale->figlio[i]->punteggio_nodo;
                        colonna = i;
                    }
                }
            }
        }
        
        livello--;
        
    }while(Caso_critico(principale) == true);
    
    return colonna; 
}


////////////////////// Deallocazione Memoria //////////////////////

void Cpu_atk::Dealloca(Nodo* principale,int level){
    if(level > 1){
        for(int q = 0; q < 7; q++){
            Dealloca(principale->figlio[q],(level-1));
            delete (principale->figlio[q]);
            
        }
    }
}
    


////    Funzione che inserisce la giusta pedina nelle matrici dei nodi figli    ////
void Cpu_atk::Inserimento(int coordinata_colonna, Nodo* secondario,int pedina){
    int i = 5;
    if(secondario->game_matrix[0][coordinata_colonna]  == -1){
        while((secondario->game_matrix[i][coordinata_colonna] != -1)&&(i >= 0)){
            i--;
        }
        secondario->game_matrix[i][coordinata_colonna] = pedina;
        secondario->controllo_inserimento=true;
    }
    else
        secondario->controllo_inserimento=false;
}


////    Funzione che genera l'albero con tutte le tabelle inizializzate correttamente   ////
void Cpu_atk::Generazione_albero(Nodo* principale, int livello, int x,int controllo) {  
    principale->controllo_inserimento=true;
    int y;
    if (x<livello) {
        for(int k = 0; k < 7; k++){
            principale->figlio[k] = new Nodo;
            memcpy(principale->figlio[k]->game_matrix, principale->game_matrix, sizeof(principale->game_matrix));
            y=x%2;
            Inserimento(k, principale->figlio[k],(y));
            principale->figlio[k]->punteggio_nodo = Euristica(principale->figlio[k],controllo);
            Generazione_albero(principale->figlio[k],livello,x+1,controllo);
        }
    }
}



////    Funzione che genera risale l'albero assegnando valori corretti ////
void Cpu_atk::Risali_livello(Nodo* principale,int x, int level, int min, int max){
    
    if(x <= level){
        for(int q = 0; q < 7; q++){
            Risali_livello(principale->figlio[q],(x+1),level, 90000, -90000);
            if(Area_di_gioco::Ottieni_area_di_gioco()->matrice_di_gioco[0][q]== -1){ 
                if((x%2) == 1){
                    
                    if(principale->figlio[q]->punteggio_nodo < min){
                        min = principale->figlio[q]->punteggio_nodo;
                    }
                }
                else{
                   
                    if(principale->figlio[q]->punteggio_nodo > max){
                        max = principale->figlio[q]->punteggio_nodo;
                    }
                }
            }
            if((x%2) == 1){
                principale->punteggio_nodo=min;
            }
            else{
                principale->punteggio_nodo=max;
            }
        }
        max=-90000;
        min=90000;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// EURISTICA ///////////////////////////////////////

float Cpu_atk::Euristica(Nodo* secondario, int x){
    float min=0;
    float vittoria= -90000;
    float sconfitta=90000;
    
    ////////////////////////////////////////    CONTROLLO SCONFITTA     ////////////////////////////////////////
    
    //controllo verticale //
    for (int r=5; r>=3; r--) {
        for (int c=6; c>=0; c--) {
            if(secondario->game_matrix[r][c] == 1){
                if((secondario->game_matrix[r-1][c]==1) && (secondario->game_matrix[r-2][c]==1) && (secondario->game_matrix[r-3][c]==1)){
                    return sconfitta;
                }
            }
        }
    }
    //controllo orizzontale //
    for (int r=5; r>=0; r--) {
        for (int c=6; c>=3; c--) {
            if(secondario->game_matrix[r][c] == 1){
                if((secondario->game_matrix[r][c-1]==1) && (secondario->game_matrix[r][c-2]==1) && (secondario->game_matrix[r][c-3]==1)){
                    return sconfitta;                }
            }
        }
    }
    //controllo diagonale sinista  //
    for (int r=5; r>=3; r--) {
        for (int c=6; c>=3; c--) {
            if(secondario->game_matrix[r][c] == 1){
                if((secondario->game_matrix[r-1][c-1]==1) && (secondario->game_matrix[r-2][c-2]==1) && (secondario->game_matrix[r-3][c-3]==1))
                    return sconfitta;;                
            }
        }
    }
    //controllo diagonale destra //
    for (int r=5; r>=3; r--) {
        for (int c=3; c>=0; c--) {
            if(secondario->game_matrix[r][c] == 1){
                if((secondario->game_matrix[r-1][c+1]==1) && (secondario->game_matrix[r-2][c+2]==1) && (secondario->game_matrix[r-3][c+3]==1) )
                    return sconfitta;               
            }
            
        }
    }
    
    
    ////////////////////////////////////////    CONTROLLO VITTORIA     ////////////////////////////////////////
    
    //controllo verticale //
    for (int r=5; r>=3; r--) {
        for (int c=6; c>=0; c--) {
            if(secondario->game_matrix[r][c] == 0){
                if((secondario->game_matrix[r-1][c]==0) && (secondario->game_matrix[r-2][c]==0) && (secondario->game_matrix[r-3][c]==0)){
                    return vittoria;
                }
            }
        }
    }
    //controllo orizzontale //
    for (int r=5; r>=0; r--) {
        for (int c=6; c>=3; c--) {
            if(secondario->game_matrix[r][c] == 0){
                if((secondario->game_matrix[r][c-1]==0) && (secondario->game_matrix[r][c-2]==0) && (secondario->game_matrix[r][c-3]==0)){
                    return vittoria;                }
            }
        }
    }
    //controllo diagonale sinista  //
    for (int r=5; r>=3; r--) {
        for (int c=6; c>=3; c--) {
            if(secondario->game_matrix[r][c] == 0){
                if((secondario->game_matrix[r-1][c-1]==0) && (secondario->game_matrix[r-2][c-2]==0) && (secondario->game_matrix[r-3][c-3]==0))
                    return vittoria;            }
        }
    }
    //controllo diagonale destra //
    for (int r=5; r>=3; r--) {
        for (int c=3; c>=0; c--) {
            if(secondario->game_matrix[r][c] == 0){
                if((secondario->game_matrix[r-1][c+1]==0) && (secondario->game_matrix[r-2][c+2]==0) && (secondario->game_matrix[r-3][c+3]==0) )
                    return vittoria;            }
            
        }
    }

   
    
    
    /////////////////////////////////////////////////// COMBINAZIONI VINCENTI  ////////////////////////////////////////////////////////////
    
    int r;
        for (int c=6; c>=4; c--) {
            r=5;
            if(secondario->game_matrix[r][c] == -1){
                if((secondario->game_matrix[r][c-1]==0) && (secondario->game_matrix[r][c-2]==0)&& (secondario->game_matrix[r][c-3]==0)&& (secondario->game_matrix[r][c-4]==-1))
                    return -50000;  
            }
        }
    
    for (int r=4; r>=0; r--) {
        for (int c=6; c>=4; c--) {
            if(secondario->game_matrix[r][c] == -1){
                if((secondario->game_matrix[r][c-1]==0) && (secondario->game_matrix[r][c-2]==0)&& (secondario->game_matrix[r][c-3]==0)&& (secondario->game_matrix[r][c-4]==-1)&& (secondario->game_matrix[r+1][c-4]!=-1)&& (secondario->game_matrix[r+1][c]!=-1))
                    return -50000;  
            }
        }
    }
    
    //controllo diagonale sinista  
    
        for (int c=6; c>=4; c--) {
            r=5;
            if(secondario->game_matrix[r][c] == -1){
                if((secondario->game_matrix[r-1][c-1]==0) && (secondario->game_matrix[r-2][c-2]==0)&& (secondario->game_matrix[r-3][c-3]==0)&& (secondario->game_matrix[r-4][c-4]==-1)&& (secondario->game_matrix[r-3][c-4]!=-1))
                    return -50000;               
            }
        }
    
    for (int r=4; r>=4; r--) {
        for (int c=6; c>=4; c--) {
            if(secondario->game_matrix[r][c] == -1){
                if((secondario->game_matrix[r-1][c-1]==0) && (secondario->game_matrix[r-2][c-2]==0)&& (secondario->game_matrix[r-3][c-3]==0)&& (secondario->game_matrix[r-4][c-4]==-1)&& (secondario->game_matrix[r-3][c-4]!=-1)&& (secondario->game_matrix[r+1][c]!=-1))
                    return -50000;                  
            }
        }
    }
    
    
    //controllo diagonale destra
    
    
        for (int c=2; c>=0; c--) {
            r=5;
            if(secondario->game_matrix[r][c] == -1){
                if((secondario->game_matrix[r-1][c+1]==0) && (secondario->game_matrix[r-2][c+2]==0) && (secondario->game_matrix[r-3][c+3]==0)&& (secondario->game_matrix[r-4][c+4]==-1)&& (secondario->game_matrix[r-3][c+4]!=-1) )
                    return -50000;                  
            }
            
        }
    
    for (int r=4; r>=4; r--) {
        for (int c=2; c>=0; c--) {
            if(secondario->game_matrix[r][c] == -1){
                if((secondario->game_matrix[r-1][c+1]==0) && (secondario->game_matrix[r-2][c+2]==0) && (secondario->game_matrix[r-3][c+3]==0)&& (secondario->game_matrix[r-4][c+4]==-1)&& (secondario->game_matrix[r-3][c+4]!=-1)&& (secondario->game_matrix[r+1][c+4]!=-1) )
                    return -50000;                  
            }
            
        }
    }

    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    
     /////////////////////////////////////////////           SPAZIO DI STATO         ////////////////////////////////////////////////
    
    
    //////////////////////////////////////////////////           DIFESA         ///////////////////////////////////////////////////////
    
       if ((n_mosse_cpu==2)&&(livello_diff<3)) {
        //verticale
        for (int r=5; r>=0; r--) {
            for (int c=6; c>=0; c--) {
                if ((secondario->game_matrix[r][c]== x)&& (secondario->game_matrix[r-1][c]== -1)&& (secondario->game_matrix[r-2][c]== -1) ) 
                    min=min+1;
                if ((secondario->game_matrix[r][c]== x)&& (secondario->game_matrix[r-1][c]== x)&& (secondario->game_matrix[r-2][c]== -1) ) 
                    min=min+4;
                //orizzontale
                if ((secondario->game_matrix[r][c]== x)&& (secondario->game_matrix[r][c-1]== -1)&& (secondario->game_matrix[r][c-2]== -1)&&(c-2>=0) ) 
                    min=min+1;
                if ((secondario->game_matrix[r][c]== x)&& (secondario->game_matrix[r][c+1]== -1)&& (secondario->game_matrix[r][c+2]== -1)&&(c+2<=6) ) 
                    min=min+1;
                if ((secondario->game_matrix[r][c]== x)&& (secondario->game_matrix[r][c-1]== x)&& (secondario->game_matrix[r][c-2]== -1)&&(c-2>=0) ) 
                    min=min+4;
                if ((secondario->game_matrix[r][c]== x)&& (secondario->game_matrix[r][c+1]== x)&& (secondario->game_matrix[r][c+2]== -1)&&(c+2<=6) ) 
                    min=min+4;
                if ((secondario->game_matrix[r][c]== x)&& (secondario->game_matrix[r][c-1]== -1)&& (secondario->game_matrix[r][c-2]== x)&&(c-2>=0) ) 
                    min=min+4;
                if ((secondario->game_matrix[r][c]== x)&& (secondario->game_matrix[r][c+1]== -1)&& (secondario->game_matrix[r][c+2]== x)&&(c+2<=6) ) 
                    min=min+4;
                //diagonale 
                if ((secondario->game_matrix[r][c]== x)&& (secondario->game_matrix[r-1][c-1]== -1)&& (secondario->game_matrix[r-2][c-2]== -1)&&(r-2<=3)&&(c-2>=0) ) 
                    min=min+1;
                if ((secondario->game_matrix[r][c]== x)&& (secondario->game_matrix[r-1][c+1]== -1)&& (secondario->game_matrix[r-2][c+2]== -1)&&(r-2<=3)&&(c+2<=6) ) 
                    min=min+1;
                if ((secondario->game_matrix[r][c]== x)&& (secondario->game_matrix[r-1][c-1]== x)&& (secondario->game_matrix[r-2][c-2]== -1)&&(r-2<=3)&&(c-2>=0) ) 
                    min=min+4;
                if ((secondario->game_matrix[r][c]== x)&& (secondario->game_matrix[r-1][c+1]== x)&& (secondario->game_matrix[r-2][c+2]== -1)&&(r-2<=3)&&(c+2<=6) ) 
                    min=min+4;
                
                
            }
        }
    } 
    
    if (n_mosse_cpu>=0) {
        //verticale
        for (int r=5; r>=0; r--) {
            for (int c=6; c>=0; c--) {
                if ((secondario->game_matrix[r][c]== x)&& (secondario->game_matrix[r-1][c]== -1)&& (secondario->game_matrix[r-2][c]== -1)&& (secondario->game_matrix[r-3][c]== -1)&&(r-3>=0)) 
                    min=min+1;
                if ((secondario->game_matrix[r][c]== x)&& (secondario->game_matrix[r-1][c]== x)&& (secondario->game_matrix[r-2][c]== -1)&& (secondario->game_matrix[r-3][c]== -1)&&(r-3>=0)) 
                    min=min+4;
                if ((secondario->game_matrix[r][c]== x)&& (secondario->game_matrix[r-1][c]== x)&& (secondario->game_matrix[r-2][c]== x)&& (secondario->game_matrix[r-3][c]== -1)&&(r-3>=0)) 
                    min=min+9;
                //orizzontale sinistra
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r][c-1]==-1 )&&(secondario->game_matrix[r][c-2]== -1)&&(secondario->game_matrix[r][c-3]== -1)&&(c-3>=0)) {
                    min=min+1;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r][c-1]== x )&&(secondario->game_matrix[r][c-2]== -1)&&(secondario->game_matrix[r][c-3]== -1)&&(c-3>=0)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r][c-1]== -1 )&&(secondario->game_matrix[r][c-2]== x)&&(secondario->game_matrix[r][c-3]== -1)&&(c-3>=0)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r][c-1]== -1 )&&(secondario->game_matrix[r][c-2]== -1)&&(secondario->game_matrix[r][c-3]== x)&&(c-3>=0)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r][c-1]== x )&&(secondario->game_matrix[r][c-2]==  x)&&(secondario->game_matrix[r][c-3]== -1)&&(c-3>=0)) {
                    min=min+9;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r][c-1]== -1 )&&(secondario->game_matrix[r][c-2]==  x)&&(secondario->game_matrix[r][c-3]== x)&&(c-3>=0)) {
                    min=min+9;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r][c-1]== x )&&(secondario->game_matrix[r][c-2]== -1)&&(secondario->game_matrix[r][c-3]== x)&&(c-3>=0)) {
                    min=min+9;
                }
                //orizzontale destra 
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r][c+1]==-1 )&&(secondario->game_matrix[r][c+2]== -1)&&(secondario->game_matrix[r][c+3]== -1)&&(c+3<=6)) {
                    min=min+1;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r][c+1]== x )&&(secondario->game_matrix[r][c+2]== -1)&&(secondario->game_matrix[r][c+3]== -1)&&(c+3<=6)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r][c+1]== -1 )&&(secondario->game_matrix[r][c+2]== x)&&(secondario->game_matrix[r][c+3]== -1)&&(c+3<=6)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r][c+1]== -1 )&&(secondario->game_matrix[r][c+2]== -1)&&(secondario->game_matrix[r][c+3]== x)&&(c+3<=6)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r][c+1]== x )&&(secondario->game_matrix[r][c+2]==  x)&&(secondario->game_matrix[r][c+3]== -1)&&(c+3<=6)) {
                    min=min+9;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r][c+1]== -1 )&&(secondario->game_matrix[r][c+2]==  x)&&(secondario->game_matrix[r][c+3]== x)&&(c+3<=6)) {
                    min=min+9;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r][c+1]== x )&&(secondario->game_matrix[r][c+2]== -1)&&(secondario->game_matrix[r][c+3]== x)&&(c+3<=6)) {
                    min=min+9;
                }
                //diagonale sinistra alto
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r-1][c-1]==-1 )&&(secondario->game_matrix[r-2][c-2]== -1)&&(secondario->game_matrix[r-3][c-3]== -1)&&(r-3>=0)&&(c-3>=0)) {
                    min=min+1;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r-1][c-1]== x )&&(secondario->game_matrix[r-2][c-2]== -1)&&(secondario->game_matrix[r-3][c-3]== -1)&&(r-3>=0)&&(c-3>=0)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r-1][c-1]== -1 )&&(secondario->game_matrix[r-2][c-2]== x)&&(secondario->game_matrix[r-3][c-3]== -1)&&(r-3>=0)&&(c-3>=0)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r-1][c-1]== -1)&&(secondario->game_matrix[r-2][c-2]== -1)&&(secondario->game_matrix[r-3][c-3]== x)&&(r-3>=0)&&(c-3>=0)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r-1][c-1]== x )&&(secondario->game_matrix[r-2][c-2]==  x)&&(secondario->game_matrix[r-3][c-3]== -1)&&(r-3>=0)&&(c-3>=0)) {
                    min=min+9;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r-1][c-1]== -1)&&(secondario->game_matrix[r-2][c-2]==  x)&&(secondario->game_matrix[r-3][c-3]== x)&&(r-3>=0)&&(c-3>=0)) {
                    min=min+9;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r-1][c-1]== x )&&(secondario->game_matrix[r-2][c-2]== -1)&&(secondario->game_matrix[r-3][c-3]== x)&&(r-3>=0)&&(c-3>=0)) {
                    min=min+9;
                }
                //diagonale destra alto
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r-1][c+1]==-1 )&&(secondario->game_matrix[r-2][c+2]== -1)&&(secondario->game_matrix[r-3][c+3]== -1)&&(r-3>=0)&&(c+3<=6)) {
                    min=min+1;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r-1][c+1]== x )&&(secondario->game_matrix[r-2][c+2]== -1)&&(secondario->game_matrix[r-3][c+3]== -1)&&(r-3>=0)&&(c+3<=6)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r-1][c+1]== -1 )&&(secondario->game_matrix[r-2][c+2]== x)&&(secondario->game_matrix[r-3][c+3]== -1)&&(r-3>=0)&&(c+3<=6)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r-1][c+1]== -1)&&(secondario->game_matrix[r-2][c+2]== -1)&&(secondario->game_matrix[r-3][c+3]== x)&&(r-3>=0)&&(c+3<=6)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r-1][c+1]== x )&&(secondario->game_matrix[r-2][c+2]==  x)&&(secondario->game_matrix[r-3][c+3]== -1)&&(r-3>=0)&&(c+3<=6)) {
                    min=min+9;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r-1][c+1]== -1 )&&(secondario->game_matrix[r-2][c+2]== x)&&(secondario->game_matrix[r-3][c+3]== x)&&(r-3>=0)&&(c+3<=6)) {
                    min=min+9;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r-1][c+1]== x )&&(secondario->game_matrix[r-2][c+2]== -1)&&(secondario->game_matrix[r-3][c+3]== x)&&(r-3>=0)&&(c+3<=6)) {
                    min=min+9;
                }
                //diagonale sinistra basso
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r+1][c-1]==-1 )&&(secondario->game_matrix[r+2][c-2]== -1)&&(secondario->game_matrix[r+3][c-3]== -1)&&(r+3<=5)&&(c-3>=0)) {
                    min=min+1;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r+1][c-1]== x )&&(secondario->game_matrix[r+2][c-2]== -1)&&(secondario->game_matrix[r+3][c-3]== -1)&&(r+3<=5)&&(c-3>=0)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r+1][c-1]== -1 )&&(secondario->game_matrix[r+2][c-2]== x)&&(secondario->game_matrix[r+3][c-3]== -1)&&(r+3<=5)&&(c-3>=0)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r+1][c-1]== -1)&&(secondario->game_matrix[r+2][c-2]== -1)&&(secondario->game_matrix[r+3][c-3]== x)&&(r+3<=5)&&(c-3>=0)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r+1][c-1]== x )&&(secondario->game_matrix[r+2][c-2]==  x)&&(secondario->game_matrix[r+3][c-3]== -1)&&(r+3<=5)&&(c-3>=0)) {
                    min=min+9;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r+1][c-1]== -1)&&(secondario->game_matrix[r+2][c-2]==  x)&&(secondario->game_matrix[r+3][c-3]== x)&&(r+3<=5)&&(c-3>=0)) {
                    min=min+9;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r+1][c-1]== x )&&(secondario->game_matrix[r+2][c-2]== -1)&&(secondario->game_matrix[r+3][c-3]== x)&&(r+3<=5)&&(c-3>=0)) {
                    min=min+9;
                }
                //diagonale destra basso
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r+1][c+1]==-1 )&&(secondario->game_matrix[r+2][c+2]== -1)&&(secondario->game_matrix[r+3][c+3]== -1)&&(r+3<=5)&&(c+3<=6)) {
                    min=min+1;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r+1][c+1]== x )&&(secondario->game_matrix[r+2][c+2]== -1)&&(secondario->game_matrix[r+3][c+3]== -1)&&(r+3<=5)&&(c+3<=6)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r+1][c+1]== -1 )&&(secondario->game_matrix[r+2][c+2]== x)&&(secondario->game_matrix[r+3][c+3]== -1)&&(r+3<=5)&&(c+3<=6)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r+1][c+1]== -1)&&(secondario->game_matrix[r+2][c+2]== -1)&&(secondario->game_matrix[r+3][c+3]== x)&&(r+3<=5)&&(c+3<=6)) {
                    min=min+4;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r+1][c+1]== x )&&(secondario->game_matrix[r+2][c+2]==  x)&&(secondario->game_matrix[r+3][c+3]== -1)&&(r+3<=5)&&(c+3<=6)) {
                    min=min+9;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r+1][c+1]== -1 )&&(secondario->game_matrix[r+2][c+2]== x)&&(secondario->game_matrix[r+3][c+3]== x)&&(r+3<=5)&&(c+3<=6)) {
                    min=min+9;
                }
                if ((secondario->game_matrix[r][c]== x)&&(secondario->game_matrix[r+1][c+1]== x )&&(secondario->game_matrix[r+2][c+2]== -1)&&(secondario->game_matrix[r+3][c+3]== x)&&(r+3<=5)&&(c+3<=6)) {
                    min=min+9;
                }
            }
        }
        
        //////////////////////////////////////////////////           ATTACCO         //////////////////////////////////////////////////
        
        for (int r=5; r>=0; r--) {
            for (int c=6; c>=0; c--) {
                if ((secondario->game_matrix[r][c]== x-1)&& (secondario->game_matrix[r-1][c]== -1)&& (secondario->game_matrix[r-2][c]== -1)&& (secondario->game_matrix[r-3][c]== -1)&&(r-3>=0)) 
                    min=min-1;
                if ((secondario->game_matrix[r][c]== x-1)&& (secondario->game_matrix[r-1][c]== x-1)&& (secondario->game_matrix[r-2][c]== -1)&& (secondario->game_matrix[r-3][c]== -1)&&(r-3>=0)) 
                    min=min-8;
                if ((secondario->game_matrix[r][c]== x-1)&& (secondario->game_matrix[r-1][c]== x-1)&& (secondario->game_matrix[r-2][c]== x-1)&& (secondario->game_matrix[r-3][c]== -1)&&(r-3>=0)) 
                    min=min-27;
                //orizzontale sinistra
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r][c-1]==-1 )&&(secondario->game_matrix[r][c-2]== -1)&&(secondario->game_matrix[r][c-3]== -1)&&(c-3>=0)) {
                    min=min-1;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r][c-1]== x-1 )&&(secondario->game_matrix[r][c-2]== -1)&&(secondario->game_matrix[r][c-3]== -1)&&(c-3>=0)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r][c-1]== -1 )&&(secondario->game_matrix[r][c-2]== x-1)&&(secondario->game_matrix[r][c-3]== -1)&&(c-3>=0)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r][c-1]== -1 )&&(secondario->game_matrix[r][c-2]== -1)&&(secondario->game_matrix[r][c-3]== x-1)&&(c-3>=0)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r][c-1]== x-1 )&&(secondario->game_matrix[r][c-2]==  x-1)&&(secondario->game_matrix[r][c-3]== -1)&&(c-3>=0)) {
                    min=min-27;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r][c-1]== -1 )&&(secondario->game_matrix[r][c-2]==  x-1)&&(secondario->game_matrix[r][c-3]== x-1)&&(c-3>=0)) {
                    min=min-27;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r][c-1]== x-1 )&&(secondario->game_matrix[r][c-2]== -1)&&(secondario->game_matrix[r][c-3]== x-1)&&(c-3>=0)) {
                    min=min-27;
                }
                //orizzontale destra 
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r][c+1]==-1 )&&(secondario->game_matrix[r][c+2]== -1)&&(secondario->game_matrix[r][c+3]== -1)&&(c+3<=6)) {
                    min=min-1;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r][c+1]== x-1 )&&(secondario->game_matrix[r][c+2]== -1)&&(secondario->game_matrix[r][c+3]== -1)&&(c+3<=6)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r][c+1]== -1 )&&(secondario->game_matrix[r][c+2]== x-1)&&(secondario->game_matrix[r][c+3]== -1)&&(c+3<=6)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r][c+1]== -1 )&&(secondario->game_matrix[r][c+2]== -1)&&(secondario->game_matrix[r][c+3]== x-1)&&(c+3<=6)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r][c+1]== x-1 )&&(secondario->game_matrix[r][c+2]==  x-1)&&(secondario->game_matrix[r][c+3]== -1)&&(c+3<=6)) {
                    min=min-27;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r][c+1]== -1 )&&(secondario->game_matrix[r][c+2]==  x-1)&&(secondario->game_matrix[r][c+3]== x-1)&&(c+3<=6)) {
                    min=min-27;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r][c+1]== x-1 )&&(secondario->game_matrix[r][c+2]== -1)&&(secondario->game_matrix[r][c+3]== x-1)&&(c+3<=6)) {
                    min=min-27;
                }
                //diagonale sinistra alto
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r-1][c-1]==-1 )&&(secondario->game_matrix[r-2][c-2]== -1)&&(secondario->game_matrix[r-3][c-3]== -1)&&(r-3>=0)&&(c-3>=0)) {
                    min=min-1;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r-1][c-1]== x-1 )&&(secondario->game_matrix[r-2][c-2]== -1)&&(secondario->game_matrix[r-3][c-3]== -1)&&(r-3>=0)&&(c-3>=0)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r-1][c-1]== -1 )&&(secondario->game_matrix[r-2][c-2]== x-1)&&(secondario->game_matrix[r-3][c-3]== -1)&&(r-3>=0)&&(c-3>=0)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r-1][c-1]== -1)&&(secondario->game_matrix[r-2][c-2]== -1)&&(secondario->game_matrix[r-3][c-3]== x-1)&&(r-3>=0)&&(c-3>=0)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r-1][c-1]== x-1 )&&(secondario->game_matrix[r-2][c-2]==  x-1)&&(secondario->game_matrix[r-3][c-3]== -1)&&(r-3>=0)&&(c-3>=0)) {
                    min=min-27;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r-1][c-1]== -1)&&(secondario->game_matrix[r-2][c-2]==  x-1)&&(secondario->game_matrix[r-3][c-3]== x-1)&&(r-3>=0)&&(c-3>=0)) {
                    min=min-27;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r-1][c-1]== x-1 )&&(secondario->game_matrix[r-2][c-2]== -1)&&(secondario->game_matrix[r-3][c-3]== x-1)&&(r-3>=0)&&(c-3>=0)) {
                    min=min-27;
                }
                //diagonale destra alto
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r-1][c+1]==-1 )&&(secondario->game_matrix[r-2][c+2]== -1)&&(secondario->game_matrix[r-3][c+3]== -1)&&(r-3>=0)&&(c+3<=6)) {
                    min=min-1;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r-1][c+1]== x-1 )&&(secondario->game_matrix[r-2][c+2]== -1)&&(secondario->game_matrix[r-3][c+3]== -1)&&(r-3>=0)&&(c+3<=6)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r-1][c+1]== -1 )&&(secondario->game_matrix[r-2][c+2]== x-1)&&(secondario->game_matrix[r-3][c+3]== -1)&&(r-3>=0)&&(c+3<=6)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r-1][c+1]== -1)&&(secondario->game_matrix[r-2][c+2]== -1)&&(secondario->game_matrix[r-3][c+3]== x-1)&&(r-3>=0)&&(c+3<=6)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r-1][c+1]== x-1 )&&(secondario->game_matrix[r-2][c+2]==  x-1)&&(secondario->game_matrix[r-3][c+3]== -1)&&(r-3>=0)&&(c+3<=6)) {
                    min=min-27;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r-1][c+1]== -1 )&&(secondario->game_matrix[r-2][c+2]== x-1)&&(secondario->game_matrix[r-3][c+3]== x-1)&&(r-3>=0)&&(c+3<=6)) {
                    min=min-27;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r-1][c+1]== x-1 )&&(secondario->game_matrix[r-2][c+2]== -1)&&(secondario->game_matrix[r-3][c+3]== x-1)&&(r-3>=0)&&(c+3<=6)) {
                    min=min-27;
                }
                //diagonale sinistra basso
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r+1][c-1]==-1 )&&(secondario->game_matrix[r+2][c-2]== -1)&&(secondario->game_matrix[r+3][c-3]== -1)&&(r+3<=5)&&(c-3>=0)) {
                    min=min-1;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r+1][c-1]== x-1 )&&(secondario->game_matrix[r+2][c-2]== -1)&&(secondario->game_matrix[r+3][c-3]== -1)&&(r+3<=5)&&(c-3>=0)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r+1][c-1]== -1 )&&(secondario->game_matrix[r+2][c-2]== x-1)&&(secondario->game_matrix[r+3][c-3]== -1)&&(r+3<=5)&&(c-3>=0)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r+1][c-1]== -1)&&(secondario->game_matrix[r+2][c-2]== -1)&&(secondario->game_matrix[r+3][c-3]== x-1)&&(r+3<=5)&&(c-3>=0)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r+1][c-1]== x-1 )&&(secondario->game_matrix[r+2][c-2]==  x-1)&&(secondario->game_matrix[r+3][c-3]== -1)&&(r+3<=5)&&(c-3>=0)) {
                    min=min-27;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r+1][c-1]== -1)&&(secondario->game_matrix[r+2][c-2]==  x-1)&&(secondario->game_matrix[r+3][c-3]== x-1)&&(r+3<=5)&&(c-3>=0)) {
                    min=min-27;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r+1][c-1]== x-1 )&&(secondario->game_matrix[r+2][c-2]== -1)&&(secondario->game_matrix[r+3][c-3]== x-1)&&(r+3<=5)&&(c-3>=0)) {
                    min=min-27;
                }
                //diagonale destra basso
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r+1][c+1]==-1 )&&(secondario->game_matrix[r+2][c+2]== -1)&&(secondario->game_matrix[r+3][c+3]== -1)&&(r+3<=5)&&(c+3<=6)) {
                    min=min-1;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r+1][c+1]== x-1 )&&(secondario->game_matrix[r+2][c+2]== -1)&&(secondario->game_matrix[r+3][c+3]== -1)&&(r+3<=5)&&(c+3<=6)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r+1][c+1]== -1 )&&(secondario->game_matrix[r+2][c+2]== x-1)&&(secondario->game_matrix[r+3][c+3]== -1)&&(r+3<=5)&&(c+3<=6)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r+1][c+1]== -1)&&(secondario->game_matrix[r+2][c+2]== -1)&&(secondario->game_matrix[r+3][c+3]== x-1)&&(r+3<=5)&&(c+3<=6)) {
                    min=min-8;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r+1][c+1]== x-1 )&&(secondario->game_matrix[r+2][c+2]==  x-1)&&(secondario->game_matrix[r+3][c+3]== -1)&&(r+3<=5)&&(c+3<=6)) {
                    min=min-27;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r+1][c+1]== -1 )&&(secondario->game_matrix[r+2][c+2]== x-1)&&(secondario->game_matrix[r+3][c+3]== x-1)&&(r+3<=5)&&(c+3<=6)) {
                    min=min-27;
                }
                if ((secondario->game_matrix[r][c]== x-1)&&(secondario->game_matrix[r+1][c+1]== x-1 )&&(secondario->game_matrix[r+2][c+2]== -1)&&(secondario->game_matrix[r+3][c+3]== x-1)&&(r+3<=5)&&(c+3<=6)) {
                    min=min-27;
                }
            }
        }
    }
    
    return min;
}

// controllo se la matrice ha delle colonne piene, e tutte le colonne libere ritornano un valore corrispondente alla sconfitta // 

bool Cpu_atk::Caso_critico(Nodo* principale){
    
    int AA = 0;
    int BB = 0;
    int CC = 0;
    
    for(int b = 0; b<7;b++){
        if((Area_di_gioco::Ottieni_area_di_gioco()->matrice_di_gioco[0][b]== -1)&&(principale->figlio[b]->punteggio_nodo == 90000)){
            AA++;
        }
        if(Area_di_gioco::Ottieni_area_di_gioco()->matrice_di_gioco[0][b]== -1){
            BB++;   
        }
        if(principale->figlio[b]->punteggio_nodo == 90000){
            CC++;
        }
        
    }
    
    if((AA == BB)&&(BB == CC)){
        return true;
    }
    else {
        return false;
    }
}


/// stampa per il debug ///

void Cpu_atk::Stampa_tavolo(Nodo* node){
    
    
    for(int riga = 0;riga < 6;riga++) {
        for(int colonna = 0;colonna < 7;colonna++) {
            if(node->game_matrix[riga][colonna] < 0)
				cout  << "\e[0;37m"<< "|   ";
            else if(node->game_matrix[riga][colonna] == 0)
				cout  << "|"<<"\e[0;33m"<<" O "<<"\e[0;37m";
            else if(node->game_matrix[riga][colonna] > 0)
				cout  << "|"<<"\e[0;91m"<<" X "<<"\e[0;37m";				
        }
        cout << "|"<<endl;
    }
    cout<<" ";
    
    for(int righ = 0; righ < 7; righ++) {
        cout << setw(2) << " " << righ << " ";
    }
    cout<<endl;
}

