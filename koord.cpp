///////////////////////////////////////////////////////////////////////////////
//                              koord.cpp
///////////////////////////////////////////////////////////////////////////////



// Methoden für die Klasse Koordinator
#include "koord.hpp"
#include <stdlib.h>
#include <iostream.h>
#include <except.h>

koordinator::koordinator(unsigned int anzahl,timer* atimer
								 ,double a_schwelle, unsigned long a_window) :
	last_activation(anzahl),
	last_breakoff(anzahl),
	last_onset(anzahl),
	actual_breakoff(anzahl),
	T(anzahl),
	Ta(anzahl)
{
	unsigned long i;
	unsigned int t;
	long int oldtime; //Alte Zeit des zu testenden Timerobjekts

	nanzahl=anzahl; //Übergabe der Anzahl der Neurone. Überprüfung erfolgt
						 //später.

	//Test der Schwelle
	if ((a_schwelle < 0.0) || (a_schwelle > 1.0)) {
		cerr << "\nkoord::koord: Der Schwellenwert " << a_schwelle
			  << " ist zu groß \n"
			  << "Schwelle auf 0.05 gesetzt.";
		schwelle = 0.05;
	}
	else schwelle = a_schwelle;

	//Test des Timerobjekts
	oldtime = atimer->get_time();
	atimer->set_time(0);
	for (i=1; i<=60000L; i++){ //Zeit bis 60000 hochzählen und prüfen
		++(*atimer); //Erhöht Zeit um 1
		if (i != atimer->get_time()){
			cerr << "Koord:Timerobjekt Fehlerhaft ! (Inkrement)"; exit(1);}
	}
	for (i=60000L; i>50000L; i--){ //10000 Schritte zurück
		if (i != atimer->get_time()){
			cerr << "Koord:Timerobjekt Fehlerhaft ! (Dekrement)"; exit(1);}
		--(*atimer); //Vermindert um 1
	}
	atimer->set_time(oldtime); //Alten Wert zurückladen
	timerp = atimer; //Übergabe auf Objektvariable

		//Die Werte initialisieren
	for (t=0; t<nanzahl; t++) {
		last_activation[t]=0.0;
		last_breakoff[t]=0;
		last_onset[t]=0;
		actual_breakoff[t]=0;
		T[t]=0.0;  //Debugwerte
		Ta[t]=0.0;
	}

	//Fensterwert prüfen
	if (a_window > UINT_MAX)
	{
		cerr << "\n Fensterwert zu groß, er wurde auf "
			  << UINT_MAX << " zurückgesetzt.\n";
		window=UINT_MAX;
	}
	else window = (unsigned) a_window;

	//Mittelwertsobjekte erstellen, wenn ein Fenster angegeben wurde
	if (window){
		try{
			T_Arr= new Mittel_Array(window); // Array mit Windowwerten
			Ta_Arr= new Mittel_Array(window); // dto.
			for(t=0;t<window;t++){
				(*T_Arr)[t]=0.0;
				(*Ta_Arr)[t]=0.0;
			}
		}
		catch(xalloc){
			cerr << "\n\n\n**** Fenster zu groß, "
				  << "bitte setzten Sie das Fenster kleiner an !\n";
			exit(1);
		}
	}
	else{
		T_Arr=NULL;
		Ta_Arr=NULL;
	}
	lastpos=0L;

	mean_T=0.0; mean_Ta=0.0; //Zwecks besserer Übersicht beim Debuggen

	id_code=KOORD_ID; //Identifizierungs-ID für Koord Objekt
}

koordinator::~koordinator()
{
	delete T_Arr;
	delete Ta_Arr;
}

void koordinator::get_neuron_pointers (basic_array* array)
{
	int t;
	//Pointerarray für Neurone testen
	//Die Nummern müssen mit dem Arrayindex übereinstimmen
	for (t=0; t<nanzahl; t++)
		if (t != (*array)[t]->GetNumber()){
			cerr << "Koord:Neuronarray ist ungueltig !"; exit(1);}
	narray=array; //Übergabe der Parameter
}

double koordinator::get_T() { return (mean_T); }

double koordinator::get_Ta() { return (mean_Ta); }

void koordinator::calc_means()
{
	if (window) { calc_window_means(); return; }  //Wenn ein Fenster angegeben
					//wurde die entsprechende Routine aufrufen.

	unsigned int i; //Zähler
	double act; //Zwischenvariable der aktuellen Aktivierung
	double nm_T,nm_Ta; nm_T=nm_Ta=0.0;//Mittelwerte für diesen Zyklus
	unsigned long act_time; //Aktueller Zeitpunkt

	for (i=1; i<nanzahl; i++){ //Inhibitorisches Neuron spielt hier keine Rolle
		act = (*narray)[i]->GetActivation();
		if ( (last_activation[i] >= schwelle) && (act < schwelle) ) //Breakoff-
			actual_breakoff[i]=timerp->get_time();          //Bedingung erfüllt ?

		if ( (last_activation[i] < schwelle) && (act >= schwelle) ) //Onset-
			last_onset[i]=timerp->get_time();               //Bedingung erfüllt ?
		last_activation[i]=act; //Aktivationswert aktualisieren
	}

	//T und Ta berechnen
	for (i=1; i<nanzahl; i++) //Inhibitorisches Neuron spielt hier keine Rolle
		if( !(actual_breakoff[i] == last_breakoff[i]) ) {
			T[i] = actual_breakoff[i] - last_breakoff[i]; //T neu berechnen
			Ta[i]= actual_breakoff[i] - last_onset[i]; //Ta neu berechnen
		}

	//Mittelwerte für diesen Zyklus berechnen
	for (i=1; i<nanzahl; i++) //Inhibitorisches Neuron spielt hier keine Rolle
		nm_T += T[i];
	nm_T = nm_T / (nanzahl-1); //Mittelwert. n Anzahl-1 wegen inhibitorischem N.

	for (i=1; i<nanzahl; i++) //Inhibitorisches Neuron spielt hier keine Rolle
		nm_Ta += Ta[i];
	nm_Ta = nm_Ta / (nanzahl-1);//Mittelwert. n Anzahl-1 wegen inhibitorischem N.

	//Die absoluten Mittelwerte für diesen Zyklus (running ensemble averages)
	//ermitteln.
	act_time = timerp->get_time();
	mean_T = (mean_T * (double)act_time + nm_T) / (((double)act_time)+1);
	mean_Ta = (mean_Ta * (double)act_time + nm_Ta) / (((double)act_time)+1);

	for (i=1; i<nanzahl; i++) //Inhibitorisches Neuron spielt hier keine Rolle
		last_breakoff[i] = actual_breakoff[i]; //Breakoff-Werte aktualisieren
}

unsigned long koordinator::get_last_breakoff(unsigned int neuron_number)
{
	if (neuron_number > nanzahl){
		cerr << "koordinator: get_last_breakoff: Unzulässige Neuronennummer";
		exit (1); //Fataler Fehler, Abbruch !
	}
	return(last_breakoff[neuron_number]);
}

unsigned long koordinator::get_last_onset(unsigned int neuron_number)
{
	if (neuron_number > nanzahl){
		cerr << "koordinator: get_last_onset: Unzulässige Neuronennummer";
		exit (1); //Fataler Fehler, Abbruch !
	}
	return(last_onset[neuron_number]);
}

unsigned long koordinator::get_actual_breakoff(unsigned int neuron_number)
{
	if (neuron_number > nanzahl){
		cerr << "koordinator: get_actual_breakoff: Unzulässige Neuronennummer";
		exit (1); //Fataler Fehler, Abbruch !
	}
	return(actual_breakoff[neuron_number]);
}


void koordinator::calc_window_means()
{
	if (!window) {calc_means(); return;} //Kein Fenster -> entsprechende Routine

	unsigned int i,c; //Zähler
	double act; //Zwischenvariable der aktuellen Aktivierung
	double nm_T,nm_Ta; nm_T=nm_Ta=0.0;//Mittelwerte für diesen Zyklus

	for (i=1; i<nanzahl; i++){ //Inhibitorisches Neuron spielt hier keine Rolle
		act = (*narray)[i]->GetActivation();
		if ( (last_activation[i] > schwelle) && (act <= schwelle) ) //Breakoff-
			actual_breakoff[i]=timerp->get_time();          //Bedingung erfüllt ?

		if ( (last_activation[i] <= schwelle) && (act > schwelle) ) //Onset-
			last_onset[i]=timerp->get_time();               //Bedingung erfüllt ?
		last_activation[i]=act; //Aktivationswert aktualisieren
	}

	//T und Ta berechnen
	for (i=1; i<nanzahl; i++) //Inhibitorisches Neuron spielt hier keine Rolle
		if( !(actual_breakoff[i] == last_breakoff[i]) ) {
			T[i] = actual_breakoff[i] - last_breakoff[i]; //T neu berechnen
			Ta[i]= actual_breakoff[i] - last_onset[i]; //Ta neu berechnen
		}

	//Mittelwerte für diesen Zyklus berechnen
	for (i=1; i<nanzahl; i++) //Inhibitorisches Neuron spielt hier keine Rolle
		nm_T += T[i];
	nm_T = nm_T / (nanzahl-1);

	for (i=1; i<nanzahl; i++) //Inhibitorisches Neuron spielt hier keine Rolle
		nm_Ta += Ta[i];
	nm_Ta = nm_Ta / (nanzahl-1);


	//Die absoluten Mittelwerte für diesen Zyklus (running ensemble averages)
	//ermitteln. Die Zeitfenster-Version
	//Kontrolle der Zeigerposition für das Array
	if (lastpos == (window-1)) lastpos=0L; //Überprüfen ob letzte Position
														// im Array erreicht ist
	else lastpos++;
	//Werte in den Arrays aktualisieren
	(*T_Arr)[lastpos]=nm_T;
	(*Ta_Arr)[lastpos]=nm_Ta;
	//Mittelwerte berechnen
	mean_T=0.0; mean_Ta=0.0;
	for(c=0L;c<window;c++){
		mean_T += (*T_Arr)[c];
		mean_Ta += (*Ta_Arr)[c];
	}
	mean_T = mean_T / window;
	mean_Ta = mean_Ta / window;

	for (i=1; i<nanzahl; i++) //Inhibitorisches Neuron spielt hier keine Rolle
		last_breakoff[i] = actual_breakoff[i]; //Breakoff Werte aktualisieren

}
