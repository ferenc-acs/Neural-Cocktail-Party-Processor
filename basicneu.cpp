///////////////////////////////////////////////////////////////////////////////
//                              basicneu.cpp
///////////////////////////////////////////////////////////////////////////////



#include "basicneu.hpp"
#include <iostream.h>
#include <except.h>

////////////////////////////////////////////////////////////////////
//                 Definition der Funktionen
////////////////////////////////////////////////////////////////////


basicneuron::basicneuron(double initial_activation, unsigned int anumneurons,
				unsigned int number, double  huge *some_weights,
				timer *some_timer_ptr)
				:weights(anumneurons),new_weights(anumneurons)
{
	unsigned int i;

	activation=initial_activation;
	numneurons=anumneurons;
	mynumber=number;

	//Gewicht zu sich selbst mu� auf 1.0 stehen. Soll aber nicht terminieren.
	if (mynumber != 0){ //Keine �berpr�fung im Falle des inhibitorischen Neurons,
		if (some_weights[mynumber] != 1.0)//da es selbst alle Gewichte auf
			cerr << "\n Neuron: " << mynumber//1.0 setzt.
				  << " (Konstruktor), Gewicht zu sich selbst muss auf 1.0 stehen !";

		//Arraywerte kopieren, aber auch nicht f�r das inhibitorische Neuron
		for (i=0; i<numneurons; i++){
			weights[i]=some_weights[i];
			new_weights[i]=some_weights[i];
		}
	}

	//�berpr�fen des gelieferten Pointers f�r das Timer-Objekt
	if (some_timer_ptr->get_time() != 0) { //Wenn die Zeit zu Anfang des Prgrs.
	 //nicht Null ist, ist es wahrscheinlich, da� der Zeiger auf M�ll zeigt.
	 cerr << "Timer Pointer �bergabefehler in Neuron "<<mynumber<<" Pointer"
		<<" warscheinlich\nung�ltig. Programmabbruch !";
	 exit (1);
	}
	time = some_timer_ptr;
}

basicneuron::~basicneuron()
{
}

double basicneuron::GetActivation()
{return (activation);}

double basicneuron::GetWeight(int num)
{
	if (num > numneurons) cerr <<"Lesezugriff auf Gewichtstabelle au�er Bereich";
	return (weights[num]);
}

//Schreibt Gewichte in ein Array, wenn das Array vorhanden ist, d.h. wenn eine
//Adresse �bergeben wird.
//Sollte kein Pointer �bergeben werden, wird ein Speicherbereich reserviert.
/* double* basicneuron::GetWeight(double huge *ptr=NULL)
{
	double huge *myptr;
	int i;

	//Speicher reservieren
	if (ptr==NULL){
		try {
			myptr = new double [numneurons];}
		catch (xalloc) {
			cerr << "Konnte Speicher f�r das "
				  << "Gewichts�bergabearray nich allokieren.";
			exit (1);
		}
	}
	else myptr=ptr;

	for (i=0; i<numneurons; i++)
		myptr[i]=weights[i];

	return (myptr);
}         */

unsigned int basicneuron::GetNumber() {return (mynumber);}

void basicneuron::Update()
{
	int i;
	activation = new_activation;
	for (i=0; i<numneurons; i++)
		weights[i]=new_weights[i];
}

double basicneuron::f1(double value)
{
	if (value > 1.0) value = 1.0;
	if (value < 0.0) value = 0.0;
	return (value);
}

int basicneuron::operator == (basicneuron other)
{
#define TRUE 1
#define FALSE 0

	if (other.GetNumber() == mynumber)
		return(TRUE);
	else
		return(FALSE);
}



