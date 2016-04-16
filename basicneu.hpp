///////////////////////////////////////////////////////////////////////////////
//                              basicneu.hpp
///////////////////////////////////////////////////////////////////////////////


// Die grundlegende Klasse für die Neuronen.
// Dient als Basisgerüst für das inhibitorische Neuron und die oszillierenden
// Neurone.

// 17.8.94: Start der Klassenentwicklung
// 18.8.94: Entscheidung, jedem Neuron eine individuelle Nummer zu geben.
//          Anfang, die erforderlichen Strukturen zu implementieren
//				Das inhibitorische Neuron sollte die Nummer 0 haben.
// 19.8.94: Übertragen der Gewichteverwaltung in die Kinderklassen
//          da eine Spezifizierung des Pointers nicht umgangen werden kann.
// 24.8.94: Update-Routine hinzugefügt, damit das Neuron nach jedem Zyklus
//          Die neu ermittelten Werte annehmen kann.
// 25.8.94: Funktion f1() hinzugefügt, welche einen Wertebereich zwischen 0
//          und 1 eingrenzen soll. Verwendet in den Funktionen, die die Akti-
//          vierung berechnen

#ifndef BASICNEU_HPP
#define BASICNEU_HPP

#include "timer.hpp"
#include <classlib/arrays.h>



class huge basicneuron {
private:
protected:
	timer	*time; //Zeiger auf das Zeitgeberobjekt
	double activation; //Aktivierung des Neurons
	double new_activation; //Neue Aktivierung
	TArrayAsVector<double> weights; //Zeiger auf ein Array mit Gewichten
	TArrayAsVector<double> new_weights; //Zeiger auf ein Array mit
													//neuen Gewichten
	unsigned int numneurons; //Anzahl der Neurone
	unsigned int mynumber; //Eigene Neuronnummer
	double f1(double); //Liefert einen zwischen 0 und 1 liegenden Wert zurück


public:
basicneuron(double initial_activation, unsigned int numneurons,
				unsigned int number, double huge *some_weights,
				timer *some_timer_ptr);
					//Anfangsaktivierung, Anzahl der Neurone
					//Zuweisungsnummer für Neuron, Array mit Gewichten, Zeiger->Timer

~basicneuron();

virtual void CalcActivation() {};
double GetActivation(); //Liefert Aktivierung
double GetWeight(int); //Liefert Gewicht zu einem best. Neuron
double huge *GetWeight(double huge *); //Schreibt Gewichte in ein Array

typedef TIArrayAsVector<huge basicneuron> basic_array;

virtual void InitPointers (basic_array* ) {};
virtual void CalcWeights() {}; //Berechnet die Gewichte
unsigned int GetNumber(); //Liefert die Neuronnummer zurück
void Update(); //Neuron nimmt die neuen Aktivierungen & Gewichte an.
virtual void SetInput(double) {}; //Input für osci neurone setzen
int operator == (basicneuron);
};

typedef TIArrayAsVector<huge basicneuron> basic_array;


#endif
