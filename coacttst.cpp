#include <stdlib.h>
#include <iostream.h>
#include <math.h>


unsigned long Td,Tad;

double Coact(unsigned long xw)
{
	double a_I,a_II,a_III,b_II,b_III,x;
	double T,Ta;
	double return_value;
	double left_I,right_I,left_II,right_II,left_III,right_III; //Grenzen der F.
	int bereichsfehler=1;

	x=(double) xw; //Wert in Fließkomma konvertieren
	T=Td;  //koord->get_T();
	Ta=Tad;  //koord->get_Ta();

	//Parameter für die einzelnen Abschnitte (I,II,III) berechnen
	a_I=M_PI / Ta;  //Es gibt kein b bei Funktion I
	a_II= M_PI - ( (T * M_PI) / (2*T-2*Ta) );
	b_II= M_PI / (T-Ta);
	a_III= -1 * ((T * M_PI) / Ta);
	b_III= a_I; //Formel ist identisch

	//Funktion I  Grenzen: 0 <= x <= Ta/2
	left_I = 0; right_I = Ta/2;
	if ( (left_I <= x) && (x <= right_I) ) {
		return_value= cos (a_I * x); bereichsfehler=0;
	}

	//Funktion II Grenzen: Ta/2 < x <= (2T-T*Ta)/(T^2-2*Ta)+(T-Ta/2)
	left_II = right_I; right_II = ((T*T - T*Ta) / (2*T - 2*Ta)) + ((T-Ta)/2);
																				 //***********
			//Veränderung vom 14.12.94, Term wurde einfach vergessen !!
			//und T^2 wurde ursprünglich als T notiert
	if ( (left_II < x) && (x <= right_II) ) {
		return_value= cos (b_II * x + a_II); bereichsfehler=0;
	}

	//Funktion III Grenzen: (2T-T*Ta)/(2T-2*Ta) < x <= Ta+T
	left_III=right_II; right_III = Ta+T;
	if ( (left_III < x) && (x <= right_III) ) {
		return_value= cos (b_III * x + a_III); bereichsfehler=0;
	}

	//Fehlerüberprüfung
	if (bereichsfehler) {
		cerr<<"\nosci_neuron: Bereichfehler bei der Koaktivitätsfunktion !\n";
		exit(1);
	}
	return(return_value);
}


int main(int argc,char * argv[])
{
		unsigned long start,ende,i;

		if (argc<5) {
			cout << "\n coacttst [Start] [Ende] [T] [Ta]"; exit(1);}

		start = atol(argv[1]); ende = atol(argv[2]);
		Td = atol(argv[3]); Tad = atol(argv[4]);

		if (start >= ende) {
			cout << "\n start < ende, muß sein\n"; exit(1);}
		if (Tad > Td) {
			cout << "\n T > Ta, muß sein\n"; exit(1);}

		for(i=start; i<=ende; i++)
			cout << i <<" ~ "<< Coact(i) << '\n';

		return(0);
}