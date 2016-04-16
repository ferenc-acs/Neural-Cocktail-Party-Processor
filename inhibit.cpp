///////////////////////////////////////////////////////////////////////////////
//                              inhibit.cpp
///////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//                      Funktionsdefinitionen zu Inhibit.hpp

#include "inhibit.hpp"
#include <iostream.h>
#include <stdlib.h>

inhibit_neuron::inhibit_neuron(inhibit_parm_list pa,
				double initial_activation, unsigned int numneurons,
				unsigned int number, double huge *some_weights,
				timer *some_timer_ptr):
				basicneuron(initial_activation,numneurons,number,
				some_weights,some_timer_ptr)
{
	int i;
// - - - Hier sollte vielleicht noch eine Parameterüberprüfung hinein - - -
	parms = pa;
	if (number != 0) {
		cerr << "\nInibitorische Neuron muss die Nummer 0 haben !\n";
		exit (1);
	}
	//Falls Gewichte übergeben wurden, werden sie nun auf 1.0 gesetzt.
	//Sie werden sowieso nicht gebraucht.
	for (i=0;i<numneurons;i++)
		weights[i]=1.0;
}

inhibit_neuron::~inhibit_neuron()
{
}

//Initialisert *oscineurons
//Führt einen Gültigkeitscheck des Arrays durch
void inhibit_neuron::InitPointers (basic_array* others)
{
	unsigned int i;

	//Vergleich der anderen Nummern
	for (i=1; i<numneurons; i++) //Nullte Stelle bleibt frei, da nach den
		if (((*others)[i]->GetNumber()) != i) {//Konvertionen die Null für das
			cerr << "\nFehler bei initialisierung des Pointerarrays beim "//inhi-
			<< "inhibitorischem Neuron " <<//bitorische Neuron reserviert ist
			mynumber << ":\n Zeiger Nummer " << i << "Zeigt nicht auf Neuron "<<i
			<< " sondern auf Neuron "<<(*others)[i]->GetNumber();
			exit(1);
		}

	if((*others)[0]->GetNumber() != 0) {
		cerr<<"\nInhibit Neuron zeigt nicht auf sich selbst\n";
		exit(1);
	}

	oscineurons = others; //o.k., Checks bestanden
}

void inhibit_neuron::CalcActivation(){ //Rechnet die aktuelle Aktivierung des
	double term,sum = 0.0;	// inhibitorischen Neurons aus
	int i;

	for (i=1; i<numneurons; i++) //wieder ab 1, da 0 das inhibitorische Neuron
		sum += ((*oscineurons)[i])->GetActivation(); //Summiert die Aktivierungen
	// der oszillierenden Neurone.

	term = parms.beta * activation + parms.s_eh * sum;

	new_activation=f1(term); //f1 siehe Basicneuron
}

void inhibit_neuron::SetInput(double dummy)
{
	dummy--;
	cerr << "\ninhibit_neuron::Nur Osci Neurone dürfen einen Input bekommen !\n"
		  << "Es darf kein Input an Neuron 0 (Inhibitorisches) gegeben werden !\n";
}

void inhibit_neuron::CalcWeights()
{
	cerr << "\ninhibit_neuron::Gewichte berechnen unnötig ! Daß inhibitorische\n"
			<<"Neuron hat zu allen anderen Neuronen das Gewicht 1.0 (maximal)\n";
}

