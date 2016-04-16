///////////////////////////////////////////////////////////////////////////////
//                              inhibit.hpp
///////////////////////////////////////////////////////////////////////////////


// Klasse des Inhibitorischen Neurons
// 19.8.94: Beginn der Klassenentwicklung und der Übertragung der Zeigerverwal-
//          tungsroutinen aus der Elternklasse. (basicneuron)


#ifndef INHIBIT_HPP
#define INHIBIT_HPP

#include "basicneu.hpp"

struct huge inhibit_parm_list { //Struktur für Parameter
	double beta;                 //des inhibitorischen Neurons
	double s_eh;
};


//////////////////////////////////////////////////////////////////////////
//                      Klassendefinition
//////////////////////////////////////////////////////////////////////////

class huge inhibit_neuron : public basicneuron {
	private:
	inhibit_parm_list parms; //Parameter des inhibitorischen Neurons
	basic_array* oscineurons; //Pointer auf ein Array von Ptr. auf Neuronen

	public:
	inhibit_neuron(inhibit_parm_list pa,
				double initial_activation, unsigned int numneurons,
				unsigned int number, double huge *some_weights,
				timer *some_timer_ptr);
					//Parameter für das inhibitorische Neuron,
					//Anfangsaktivierung, Anzahl der Neurone
					//Zuweisungsnummer für Neuron, Array mit Gewichten, Zeiger->Timer
	~inhibit_neuron();

	void CalcActivation();
	void InitPointers (basic_array* others); //Initialisert *otherneurons
	void SetInput(double); //Ist nur für osci_neurone !
	void CalcWeights();
};
#endif