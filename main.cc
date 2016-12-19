//
//  Forza4.cpp
//  
//
//  Created by Federico Giorni on 11/01/14.
//
//

#include <iostream>
#include <iomanip>
#include <string>

#include "tavolo_da_gioco.h"
#include "giocatore.h"
#include "cpu_atk.h"
#include "cpu_def.h"





int main(){
    
    cout<<"\e[0;91m"<<"||=======================================================================||\n";
    cout<<"||=======================================================================||\n";
    cout<<"||"<<"\e[0;33m"<<"          __________  ____  _____   ___       __ __                    "<<"\e[0;91m"<<"||\n";
    cout<<"||"<<"\e[0;33m"<<"         / ____/ __ \\/ __ \\/__  /  /   |     / // /                    "<<"\e[0;91m"<<"||\n";
    cout<<"||"<<"\e[0;33m"<<"        / /_  / / / / /_/ /  / /  / /| |    / // /_                    "<<"\e[0;91m"<<"||\n";
    cout<<"||"<<"\e[0;33m"<<"       / __/ / /_/ / _, _/  / /__/ ___ |   /__  __/                    "<<"\e[0;91m"<<"||\n";
    cout<<"||"<<"\e[0;33m"<<"      /_/    \\____/_/ |_|  /____/_/  |_|     /_/                       "<<"\e[0;91m||\n";
    cout<<"||                                                          "<<"\e[0;37m"<<"ver 3.1"<<"\e[0;91m"<<"      ||\n";
    cout<<"||=======================================================================||\n";
    cout<<"||=======================================================================||\n"<<"\e[0;37m";
    cout<<"\n";
	
	Giocatore player("Cinzia"); 
	Giocatore cpu("HopEngine"); 
    Giocatore player2("Giocatore 2");
    
    Cpu_def computer_IA(4);
    
	
	while((Area_di_gioco::Istanza_tavolo()->vittoria == false)&&(Area_di_gioco::Istanza_tavolo()->pareggio == false)){
        if(Area_di_gioco::Istanza_tavolo()->Ottieni_numero_mosse() < 42){
            player.Inserimento();
            if (Area_di_gioco::Istanza_tavolo()->vittoria == false) {
                Area_di_gioco::Istanza_tavolo()->Stampa_tavolo();
            }
            
        }
        else{
            Area_di_gioco::Istanza_tavolo()->pareggio = true;
            cout<<"patta !"<<endl;
        }        
        
        if ((Area_di_gioco::Istanza_tavolo()->vittoria == false)&&(Area_di_gioco::Istanza_tavolo()->pareggio == false)){
            if(Area_di_gioco::Istanza_tavolo()->Ottieni_numero_mosse() < 42){
                cpu.Inserimento(computer_IA.Mossa_successiva());
            }
            else{
                Area_di_gioco::Istanza_tavolo()->pareggio = true;
                cout<<"patta !"<<endl;            
            }
            
            if (Area_di_gioco::Istanza_tavolo()->vittoria == false) {
                Area_di_gioco::Istanza_tavolo()->Stampa_tavolo();
            }
        }
    }
    
	delete Area_di_gioco::Istanza_tavolo();
    cout<<"Forza4! sviluppato da Federico Giorni & Marco Becattini."<<endl;
    //cin.close();
	return 0;
}
