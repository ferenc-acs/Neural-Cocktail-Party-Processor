///////////////////////////////////////////////////////////////////////////////
//                              oscineur.cpp
///////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////
//Definitionen der Elementfunktionen für die Klasse osci_neuron
//20.12 05.06 : CalcActivation verbessert, in der Summation der anderen
//              Neuronenwerte wurden die Gewichte nicht berücksichtigt.

#include "oscineur.hpp"
#include <iostream.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

osci_neuron::osci_neuron(osci_parm_list pa,
				double initial_activation, unsigned int numneurons,
				unsigned int number, double huge *some_weights,
				timer *some_timer_ptr, koordinator* a_koord) :
				basicneuron(initial_activation,numneurons,number,
				some_weights,some_timer_ptr)
{
	//Vielleicht noch eine Überprüfung der Parameter einfügen
	parms=pa;
	old_G_i=0.0; //Hilfsvariable für den gleitenden Durchschnitt auf 0.0 setzen.
	old_N_i=0.0; //dto. nur für N_i();
	//Gewicht zum Neuron 0 muß immer auf 1.0 stehen (Inhibitorisches Neuron)
	weights[0]=1.0;
	//Koordinator ID Code überprüfen
	if (a_koord->id_code != KOORD_ID){
		cerr << "\nosci_neuron::Koord Adressangabe falsch !\n";
		exit(1); //Fataler Fehler: Abbruch
	}
	koord=a_koord;
	if (parms.s_d == 0.0) {
		cerr << "\n osci_neuron::osci_neuron: "
			  <<"Der Parameter s_d darf nicht 0 sein !";
		exit (1);
	}
	q_s0_durch_sd = parms.s_0 / parms.s_d; //Beschleunigt die Q-Funktion,
						//da dieser Term nun nicht mehr in jedem Zyklus gerechnet wird
}

osci_neuron::~osci_neuron() {}

//Initialisiert die Pointer auf andere osci_neurons und auf das inhibitorische
//Neuron
void osci_neuron::InitPointers (basic_array* aothers)
{
	unsigned int errno,i;
	if ((*aothers)[0]->GetNumber() != 0){
		cerr << "\nFehler bei der Pointerinitialisierung für das Inhibitorische" <<
			" Neuron\nNummer des Objekts ist ungleich Null";
		exit(1);
	}

	//Gültigkeitscheck 1
	//zeigt Pointer-Nummer mynumber auf mich selbst ?
	if ((errno=(*aothers)[mynumber]->GetNumber()) != mynumber) {
		cerr << "\nFehler: Pointer Nummer "<<errno<<" zeigt nicht auf Neuron "
		<<mynumber<<'.';
		exit (1);
	}

	//Gültigkeitscheck 2
	//Vergleich der anderen Nummern
	for (i=0; i<numneurons; i++)
		if ((errno=(*aothers)[i]->GetNumber()) != i) {
			cerr << "\nFehler bei initialisierung des Pointerarrays bei Neuron " <<
			mynumber << ":\n Zeiger Nummer " << i << "Zeigt nicht auf Neuron "<<i
			<< " sondern auf Neuron "<<errno;
			exit(1);
		}

	//Alle Checks durchlaufen...
	others = aothers; //ok. Zeiger werden kopiert
}

#define NOISE_UPPER 0.01 //Obergrenze für den Noise Term
double osci_neuron::z_i()
{
	int rand;
	double zufall;

	rand = random(INT_MAX);
	zufall = ((double)rand/(double)INT_MAX) / (1.0/NOISE_UPPER);

	return(zufall);
}
/*
Überprüfung der Funktion z_i() durch ein kleines Testprogramm.
Die Ausgaben wurden mit SYSTAT ausgewertet :
NOISE_UPPER war auf 0.01 gesetzt

FRI 26.08.94 17:34:39   E:\PROTOCOL.SYS

 TOTAL OBSERVATIONS:  95784


							NOISE

	N OF CASES            95783
	MINIMUM               0.000
	MAXIMUM               0.010
	MEAN                  0.005
	STANDARD DEV          0.003
*/



void osci_neuron::CalcActivation()
{
	double one_or_not; //Ergebnis der 1er Funktion in basicneuron
	double sum; //Summe der gewichteten Aktivierungen aller Neurone
	double feed; //Füttert die 1er Funktion
	unsigned int nc; //Neuron-Counter, zum Durchzählen der Neurone

	//Summation der Aktivierungen der anderen oszillierenden Neurone
	sum = 0.0;
	for (nc=1;nc<numneurons;nc++)
		if (nc != mynumber) //Den eigenen Wert nicht mit aufsummieren
			sum += ( (weights[nc]) * ((*others)[nc]->GetActivation()) );

	//Berechnung der Aktivierung gemäß Formel 1
	feed = A_i() + parms.alpha * activation + sum -
						  parms.s_he * (*others)[0]->GetActivation() +
						  z_i();//others[0]->GetActivation(): liefert die Aktivität
								  //des inhibitorischen Neurons.
	one_or_not = f1( feed );
	new_activation = N_i() * one_or_not;
}

double osci_neuron::A_i() //Wird auf 0.1 gesetzt, wenn die Zelle externen Input
{                         //bekommt
	double return_value = 0.0;
	//Liegt eine aktuelle Reizung vor ?
	if (time->get_time() == input.time)
		if (input.strength > 0.0) return_value = 0.1;
	return (return_value);
}

double osci_neuron::G_i()
{
	double result;

	result = ( 1 - parms.delta ) * old_G_i + parms.delta * activation;
	old_G_i=result; //Zuweisung für den nächsten Zyklus
	return (result);
}

double osci_neuron::N_i()
{
	double N=1.0; //Der Rückgabewert
	double Gparm; //Nur G_i...

	Gparm = G_i(); //... um G-i() nicht zweimal aufzurufen ! (sonst falscher MW)

	if (Gparm > parms.g_u) N=0.0;
	if ( (old_N_i == 0) && (Gparm > parms.g_l) ) N=0.0;

	old_N_i = N;
	return (N);
}

void osci_neuron::SetInput(double external) //Aktualisiert den Input
{
	//Fehlerüberprüfung:Input da im aktuellen Modell nur zwischen 0 und 1 liegen
	if ( (external < 0.0) || (external > 1.0) )
		cerr << "\noscineur: Achtung ! Input darf nur zwischen 0 und 1 liegen !\n"
			  << "Aktueller Input: " << external << " ,Simulationszeit: "
			  << time->get_time();

	//Input setzen und Simulationszeit ermitteln
	input.strength = external;
	input.time = time->get_time();
}


//Dies ist die Koactivitätsfunktion, sie wird aus drei abschnittsweise de-
//finierten Funktionen von Sinuskurven zusammengesetzt. Die Sinusfunktionen
//selbst werden in den einzelnen Abschnitten gestreckt und verschoben (Abszisse)
double osci_neuron::Coact(unsigned long xw)
{
	double a_I,a_II,a_III,b_II,b_III,x;
	double T,Ta;
	double return_value;
	double left_I,right_I,left_II,right_II,left_III,right_III; //Grenzen der F.
//	int bereichsfehler=1;

	x=(double) xw; //Wert in Fließkomma konvertieren
	T=koord->get_T();
	Ta=koord->get_Ta();

	//Parameter für die einzelnen Abschnitte (I,II,III) berechnen
	a_I=M_PI / Ta;  //Es gibt kein b bei Funktion I
	a_II= M_PI - ( (T * M_PI) / (2*T-2*Ta) );
	b_II= M_PI / (T-Ta);
	a_III= -1 * ((T * M_PI) / Ta);
	b_III= a_I; //Formel ist identisch

	//Funktion I  Grenzen: 0 <= x <= Ta/2
	left_I = 0; right_I = Ta/2;
	if ( (left_I <= x) && (x <= right_I) ) {
		return_value= cos (a_I * x); //bereichsfehler=0;
	}

	//Funktion II Grenzen: Ta/2 < x <= (2T-T*Ta)/(T^2-2*Ta)+(T-Ta/2)
	left_II = right_I; right_II = ((T*T - T*Ta) / (2*T - 2*Ta)) + ((T-Ta)/2);
																				 //***********
			//Veränderung vom 14.12.94, Term wurde einfach vergessen !!
			//und T^2 wurde ursprünglich als T notiert
	if ( (left_II < x) && (x <= right_II) ) {
		return_value= cos (b_II * x + a_II); //bereichsfehler=0;
	}

	//Funktion III Grenzen: (2T-T*Ta)/(2T-2*Ta) < x <= Ta+T
	left_III=right_II; right_III = Ta+T;
	if ( (left_III < x) && (x <= right_III) ) {
		return_value= cos (b_III * x + a_III); //bereichsfehler=0;
	}

	//Fehlerüberprüfung
/*	if (bereichsfehler) {
		cerr<<"\nosci_neuron: Bereichfehler bei der Koaktivitätsfunktion !\n";
		exit(1);
	}                  */
	return(return_value);
}

double osci_neuron::q(double s) //Kontrollfunktion zum aktuellen Gewicht
{
	double q;
	q = parms.q_0 * ( 1 - ((s-parms.s_0)/q_s0_durch_sd) *
								  ((s-parms.s_0)/q_s0_durch_sd));
	return (q);
}

void osci_neuron::CalcWeights()
#define UPP_CLIP 1.0
#define LOW_CLIP -1.0 //Siehe Kommentar weiter unten
{
	//Das Neuron darf seine Gewichte nur verändern, wenn es sich in einer
	//"interburst" Phase befindet
	//Folgend mögliches Problem ist noch nicht gelöst (taucht es überhaupt auf ?)
	//Wenn ein Neuron die Coaktivität mit einem Neuron ausrechnet, welches noch
	//nicht gefeuert hat, so kann es zu einer Bereichsüberschreitung bei Coact
	//kommen.
	long delay_ij_t; //Phasenverschiebung dieses Neurons mit einem Neuen
	unsigned long delay_ij; //dto., nur ohne Vorzeichen, da Abstand immer positiv
	unsigned long k1,k2,krit;
	double change_weight; //Gewichtsveränderung
	unsigned int c; //Schleifenzähler
	long comp; //Nur zum Debuggen

	k1=time->get_time() - koord->get_actual_breakoff(mynumber);
	krit= (koord->get_T() + koord->get_Ta()) / 2;

/*	//Soll nur berechnet werden wenn Neuron in der Interburst Phase, oder wenn
	//   actual_breakoff - last_onset > 0
	comp=koord->get_actual_breakoff(mynumber) - koord->get_last_onset(mynumber);
	if(comp>0L)  */
	if (koord->get_actual_breakoff(mynumber) != 0L) //Keine Gewichtsbestimmung
																	//am Anfang
		for (c=1; c<numneurons; c++)//Wenn Inhibitorisches Neuron dann
											 //Gewicht nicht verändern !
			if( c != mynumber) { //Gewicht zu sich selbst nicht verändern,
										// immer 1.0
				k2=time->get_time() - koord->get_actual_breakoff(c);
//				if ( (k1<=krit) && (k2<=krit) ){ //Gewichte nur verändern
															//wenn innerhalb krit gefeuert.
					delay_ij_t = koord->get_last_breakoff(mynumber)
									 - koord->get_last_breakoff(c);
					//Absolutwert nehmen, weil Abstand immer postitiv:
					delay_ij = labs(delay_ij_t);
					change_weight = (q(weights[c])) * (Coact(delay_ij)); //Formel 7
					new_weights[c] = weights[c] + change_weight;
					//Gewichtsverändeung addieren.
					//Clipping des neuen Gewichtswertes, falls er größer
					//oder kleiner UPP_CLIP||LOW_CLIP
					//sein sollte. Dies kann durch Extremwerte (meistens negativ)
					//von q verursacht werden.
					if (new_weights[c] > UPP_CLIP) new_weights[c] = UPP_CLIP;
					if (new_weights[c] < LOW_CLIP) new_weights[c] = LOW_CLIP;
//				}
			}
			 else if (weights[mynumber] != 1.0) {
				cerr << "\nosci_neuron::calc_weights- Gewicht zu sich selbst nicht mehr 1.0\n"
					  << "Fehler während der Laufzeit behoben, Neuron: " << mynumber
					  << "  Zeitpunkt: " << time->get_time();
				weights[mynumber] = 1.0;
			 }
}





