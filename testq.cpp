//Soll zum Testen der Funktion q_i von osci_neuron.cpp dienen.

#include <stdlib.h>
#include <iostream.h>
#include <math.h>

struct osci_parm_list {
	double alpha; //Siehe Aktivierungsformel
	double s_he;  //dto.
	double delta; //In der Formel für den Gleitenden Durchschnitt
	double g_u;   //In der Formel für Ni
	double g_l;   //dto.
	double q_0;
	double s_0;
	double s_d;
} parms;



double q(double s) //Kontrollfunktion zum aktuellen Gewicht
{
	double q;
	q = parms.q_0 * ( 1 -
	((s-parms.s_0)/(parms.s_0/parms.s_d)) * ((s-parms.s_0)/(parms.s_0/parms.s_d)));
		//          ^^^^^^^^^^^^^^^^^^^^^                   ^^^^^^^^^^^^^^^^^^^^^
		// Können noch durch eine Konstante, die während Runtime ermittelt wird er-
		// setzt werden. Führt zu höherer Geschwindigkeit
	return (q);
}


int main(int argc,char * argv[])
{
		double start,ende,weite,i;

		if (argc<4) {
			cout << "\n testq [Start] [Ende] [Schrittweite]"; exit(1);}

		parms.alpha=0.89; parms.s_he=0.22; parms.delta=0.35;
		parms.g_u=0.4;  parms.g_l=0.01; parms.q_0=0.01;
		parms.s_0=0.012; parms.s_d=0.8;

		start = atof(argv[1]); ende = atof(argv[2]);
		weite = atof(argv[3]);

		if (start >= ende) {
			cout << "\n start < ende, muß sein\n"; exit(1);}
		if (weite >= (ende - start)) {
			cout << "\n Die Schrittweite macht keinen Sinn\n"; exit(1);}

		for(i=start; i<=ende; i += weite)
			cout << i <<" ~ "<< q(i) << '\n';

		return(0);
}