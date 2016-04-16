///////////////////////////////////////////////////////////////////////////////
//                              party.cpp
///////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////
//
// Daß inhibitorische Neuron hat die Nummer 0

#include "inhibit.hpp"
#include "koord.hpp"
#include "oscineur.hpp"
#include "timer.hpp"
#include "reizmngr.hpp"
#include <iostream.h>
#include <stdlib.h>
#include <limits.h>
#include <except.h>
//Erste Konstruktion zum Austesten der Routinen
#define NUMNEURONS 21 //mit inhibitorischem Neuron
#define TRUE 1
#define FALSE 0
//Initialisiert die Parameter, zwecks Übersicht aus dem Hauptprogramm genommen
//9.12.1994 Alle Objekte stehen ! Funktionieren (??) !! <-(25.12.94)

void InitParms (inhibit_parm_list *ip, osci_parm_list *op)
{
	ip->beta=0.63;	ip->s_eh=0.036;

	op->alpha=0.89; op->s_he=0.22; op->delta=0.35;
	op->g_u=0.4;  op->g_l=0.01; op->q_0=0.01;
	op->s_0=0.012; op->s_d=0.8;
}
//Schreibt Zufallsgewichte in Gewichtsarray
void RandomWeights(double huge *wght, unsigned int number, double min=-0.00248
						 ,double max=0.02722)
{
	int i,rand;
	double zufall,range;

	randomize();
	range = max-min;
	for (i=0; i<NUMNEURONS; i++){
		rand=random(INT_MAX);
		zufall = ((double)rand/(double)INT_MAX) * range; //In den Bereich setzen
		zufall = zufall + min; //Bereich verschieben
		wght[i]=zufall;
	}
	wght[number]=1.0; //Gewicht zu sich selbst muß auf 1.0 stehen !
}


/*void Monoweights(double huge *wght, unsigned int number, double strength)
{
	if(strength > 0.8) strength = 0.8; //Tolerante
	if(strength < -0.8) strength = -0.8; //Fehlerprüfung
	for (int i=0; i<NUMNEURONS; i++)
		wght[i]=strength;
	wght[number]=1.0; //Gewicht zu sich selbst muß auf 1.0 stehen.
} */


/////////////////////////////////////// MAIN ///////////////////////////////////

int main ( int argc, char *argv[] ) //Simulationszeit in ms
{
	int i;
	unsigned long timelimit,timecounter; //Maximale Simulationszeit, Zähler

	inhibit_parm_list inhibitparms;
	osci_parm_list osciparms;
	timer time; //Timerobjekt
	timer* time_ptr; //Zeiger auf das Objekt
	double reserve_somewghts[NUMNEURONS]; //Anfangsgewichte
	double huge * somewghts = reserve_somewghts;
	double out; //Hilfsvariable
	double schwelle; //Schwelle, ab der ein Neuron
						  //als nicht mehr aktiv angesehen wird
	unsigned long fenster; //Größe des Zeitfensters zur Mittelwertsberechnung

	basic_array neuronptr(NUMNEURONS); //Pointer auf Neurone
	i=neuronptr.OwnsElements(); //Nur aus Interesse, zum DEBUGGEN
	neuronptr.OwnsElements(FALSE); //Die Containerklasse ist nicht Besitzer
			//der Elemente (Neurone), d.h. diese müssen von "Hand" gelöscht werden
	basicneuron* del_ptr; //Pointer zum späteren Löschen der Neurone
	i=neuronptr.OwnsElements(); //Nur aus Interesse, zum DEBUGGEN


	//Eingabeparameter der Zeit bearbeiten
	if (argc != 5){ //Hilfe ausgeben
		 cout << "\n--> party [simulationszeit in ms] [Inputfile] [Schwelle] [Fenster]\n";
		 exit(0);
	}
	timelimit = atol(argv[1]);
	schwelle = atof(argv[3]);
	fenster = atol(argv[4]);


	//Pointer auf das Timer-Objekt initialisieren
	time_ptr=&time;
	Reiz_Manager reizmngr(time_ptr); //Das Objekt für Dateneinlesen
												//und Reizverwaltung
	if(reizmngr.read_data(argv[2]) == FALSE) {
		cerr << "\nKonnte die Eingabedatei " << argv[2] << " nicht öffnen";
		exit(1);
	}

	//Definition der Parameter
	double ia=0.0; //Initial-Aktivation
	InitParms(&inhibitparms, &osciparms);

	koordinator koord(NUMNEURONS,&time,schwelle,fenster); //Koord. Objekt

	//Neurone initialisieren
	//1. inhibitorisches Neuron
	inhibit_neuron inhibit(inhibitparms,ia,NUMNEURONS,0,somewghts,time_ptr);
				//Somewghts Übergabe ist ein Artefakt, da dieseNeuron immer 1.0 hat
	neuronptr[0]=&inhibit;
	//2. Osci Neurone
	try{
	for (i=1;i<NUMNEURONS;i++) {
		RandomWeights(somewghts,i); //Gewichtstabelle für Osc.-Neurone
		neuronptr[i]=new osci_neuron(osciparms,ia,NUMNEURONS,i,somewghts,&time,
											  &koord);
	}}
	catch(xalloc) {cerr<<"MAIN:Speicherfehler !"; exit(1);}

	//3. Pointer auf andere Neurone an alle Neurone bekanntgeben
	for(i=0;i<NUMNEURONS;i++) neuronptr[i]->InitPointers(&neuronptr);
	koord.get_neuron_pointers(&neuronptr); //Adressen an das Koord-Objekt
	reizmngr.init_pointers(&neuronptr,NUMNEURONS); //Adressen an den Reizmanager

//////////////////////////////////////////////////////////////////////////////
//////////////       Der Hauptteil    ////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
	cout.precision(15); //Gleitkomma auf 15 Stellen genau ausgeben,
							  //double ist bis 15 Stellen genau
	cout.setf(ios::fixed); //Normalzahlenausgabe im Format 5.7465764
	cout << "\n\n\n Eingabedatei: " << argv[2];
	cout << "\n Anzahl der Läufe: " << timelimit;
	cout << "\n Schwelle für T & Ta berechnungen: " << schwelle;
	cout << "\n Zeitfenster (0, wenn kein Fenster): " << fenster;
	cout << "\n\n\nZeit, T-Wert, Ta-Wert,"
		  << " Inhibit.Aktivität, Oszill.Aktivitäten\n";


	for (timecounter=1; timecounter <= timelimit; timecounter++)
	{
		cout << '\n'; //Neue Zeile beginnen
		++(*time_ptr); //Simulationszeit um 1 erhöhen
		cout << (time_ptr->get_time()) << " ~ ";

		//### REIZEN ###
		reizmngr.kitzle(1.0);

		cout << koord.get_T() << " ~ " << koord.get_Ta();

		for (i=0;i<NUMNEURONS;i++) {
			neuronptr[i]->CalcActivation();
			if (i!=0) neuronptr[i]->CalcWeights(); //Nicht für Inhibit-Neuron !
		}

		for (i=0;i<NUMNEURONS;i++)
			neuronptr[i]->Update(); //Neue Aktivierungen & Gewichte annehmen

		for (i=0;i<NUMNEURONS;i++){
			out = (double)i + (neuronptr[i]->GetActivation());
			cout << " ~ " << out;
		}
				//Addition mit i wegen besserer Darstellbarkeit in MS Excel

		koord.calc_means(); //Phasendifferenzen, Ensemble Averages usw. ermitteln

	}

/////////////////////////////////////////////////////////////////////////////
//              ENDE Alles schließen ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
	for (i=1;i<NUMNEURONS;i++){ //Neurone mit den Nummern 1-NUM... werden
		del_ptr=neuronptr[i];  //gelöscht. Neuron 0 (Inhibit) wird vom Haupt-
		delete del_ptr;
	}
									//programm gelöscht, da nicht mit neu erstellt.
	return (0);
}
