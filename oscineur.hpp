///////////////////////////////////////////////////////////////////////////////
//                              oscineur.hpp
///////////////////////////////////////////////////////////////////////////////


//Ok. hier stehen die Informationen für den interessantesten Neuronentyp,
//die oszillierenden Neuronen.

//9.12.94 Fataler Fehler entdeckt, die Funktion A_i() hat nicht auf einen
//		    externalen Reiz abgeprüft ! Routine komplett gelöscht

#ifndef OSCINEUR_HPP
#define OSCINEUR_HPP

#include "basicneu.hpp"
#include "inhibit.hpp"
#include "koord.hpp"


//Parameterliste
struct huge osci_parm_list {
	double alpha; //Siehe Aktivierungsformel
	double s_he;  //dto.
	double delta; //In der Formel für den gleitenden Durchschnitt
	double g_u;   //In der Formel für Ni
	double g_l;   //dto.
	double q_0;
	double s_0;
	double s_d;
};

//Kleine Struktur für den Inputreiz
struct huge Str_Input {
	double strength; //Stärke des Input, in der jetzigen Implementation sind
  //jedoch nur Stärken zwischen 0 und 1 wirksam, siehe A_i() || vom 9.12.1994.
	unsigned long time; //Zeitpunkt, an dem der letzte Input wirksam wurde.
	//wird gebraucht, damit die Zelle ermitteln kann ob noch ein Reiz anliegt.
};

//Die Klassendeklaration
class huge osci_neuron : public basicneuron {
	private:
	osci_parm_list parms;
	basic_array*  others; //Pointer auf Array v. Pointer auf andere Neurone
	double old_G_i; //Alter Wert von G_i()
	double old_N_i; //dto. nur mit N_i();
	Str_Input input; //Externer Reiz
	koordinator* koord; //Zeiger auf das Koordinator-Objekt, für T und Ta
	double q_s0_durch_sd; //Variable zur Beschleunigung der Q-Funktion

	protected:
	double z_i(); //Der sog. Noise Term, Aktvierungsbestimmung bei Ei(t)
	double A_i(); //0.1 bei externer Erregung, ansonsten 0. (Erregungsformel)
	double G_i(); //Die gliding Average Funktion, für die best. von N(t)
	double N_i(); //Die Funktion Ni(t), Aktivitätsberechnung
	double q(double); //Kontrollfunktion bei der Gewichtsveränderung ->Formel 7&8
	double Coact(unsigned long); //Koaktivitätsfunktion, erwartet die Phasen
										  //Differenz zwischen zwei Neuronen.

	public:
	osci_neuron(osci_parm_list pa,
				double initial_activation, unsigned int numneurons,
				unsigned int number, double huge *some_weights,
				timer *some_timer_ptr, koordinator* a_koord);
					//Parameter für die oszillierenden Neurone,
					//Anfangsaktivierung, Anzahl der Neurone
					//Zuweisungsnummer für Neuron, Array mit Gewichten, Zeiger->Timer
					//Pointer auf ein Koordinator-Objekt.
	~osci_neuron();

	void InitPointers (basic_array* aothers);
	void CalcActivation();
	void SetInput (double); //Setzt einen externen Input
	void CalcWeights();
};
#endif
