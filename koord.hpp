///////////////////////////////////////////////////////////////////////////////
//                              koord.hpp
///////////////////////////////////////////////////////////////////////////////




// Die koordienierende Klasse, diejenige, welche für alles zuständig ist.
// Hauptfunktion ist die Verwaltung und Ermittlung der Mittelwerte
// 28.10.94 Beginn der Entwicklung
// 30.10.94 Weiterführende Entwicklung
// 15.12.94 Fehler entdeckt: Für T[] und Ta[] wurde kein Speicher reserviert
//                 Folge: Ins Nirvana zeigende Pointer verursachten ständig
//                        Pointerfehler
// 16.12.94 Fehler aufgrund der Tatsache, daß T und Ta long Werte waren, kleine
//			   Ts wurden gerundet und führten zu Fehlern, wenn T=Ta war.
// 21.12.94 Breakoff Kriterium von Schwellen abhängig gemacht, da
//				Neuronenaktivitäten manchmal nicht auf 0 zurückfallen
// 30.12.94 Neue Berechnung für T und Ta eingeführt, sie werden nun durch ein
//          variables Fenster (soundsoviele Reize zurück) berechnet

#include "timer.hpp"
#include "basicneu.hpp"
#include <classlib/arrays.h>

#ifndef KOORD_HPP
#define KOORD_HPP
#define KOORD_ID 333000333L //ID Code für Koord Objekt

typedef TArrayAsVector<double> Mittel_Array;

class huge koordinator {
	private:
	timer* timerp; //Pointer auf das Timerobjekt
	basic_array* narray; //Pointer auf das Array mit Neuronpointern
	unsigned int nanzahl; //Anzahl der Neurone
	unsigned window; //Die Größe des Zeitfensters
	double mean_T; //Mittelwert der Feuerperiode aller Neurone
	double mean_Ta;//Mittelwert der Feuerdauer aller Neorone
	double schwelle; //Ab wann wird ein Neuron als in der
						  //refraktorischen Phase betrachtet ?
	Mittel_Array *T_Arr; //-> Mittelwerte im Fenster
	Mittel_Array *Ta_Arr; //-> Mittelwerte im Fenster
	unsigned lastpos; //Letzte Position im Array bei der Mittelwertsbestimmung

	protected:
	TArrayAsVector<double> last_activation; //-> Array der letzten
														 //   Aktivierungswerte
	TArrayAsVector<unsigned long> last_breakoff; //-> Array der letzten
																//   Breakoff-Zeiten
											//Aktivität: /|_/|_/|_/|_/|_/|_/|_ ->Zeit
											//^:Breakoff  ^  ^  ^  ^  ^  ^  ^
											// `:Onset    `  `  `  `  `  `  `
	TArrayAsVector<unsigned long> last_onset;
	TArrayAsVector<unsigned long> actual_breakoff; //-> Array: Aktuelle Breakoff-
																  //          Zeit
	TArrayAsVector<unsigned long> T; //-> Array: Abstand zwischen Breakoffs
												//          für Neuron n
	TArrayAsVector<unsigned long> Ta;//-> Array: Abstand zwischen Onset & Break.
												//          für Neuron n

	public:
	koordinator(unsigned int,timer*,double,unsigned long window = 0L);
	//Konstruktor braucht Anzahl
	//der Neurone, Pointer auf das Timerobjekt, Schwelle um Neuron als nicht
	//aktiv anzusehen, Zeitfenster für die Variable Mittelwertsberechnung, wenn
	//window=0, wird die Funktion calc_window_means aufgerufen.
	~koordinator();
	void get_neuron_pointers (basic_array*); //Neuronenpointer übergeben
	long id_code; //Variable für ID_Code zur Objektüberprüfung

	double get_T();  //Liefern T oder
	double get_Ta(); //Ta zurück
	unsigned long get_last_breakoff(unsigned int); //Liefert die letzte Break-
						//off-Zeit eines best. Neurons.
	unsigned long get_last_onset(unsigned int); //dto. nur für last_onset
	unsigned long get_actual_breakoff(unsigned int);//dto. aber actual_breakoff
	void calc_means(); //Rechnet die Mittelwerte aus, d a r f erst nach Aufruf
							 //der Update-Funktion aller Neurone durchgeführt werden !
	void calc_window_means(); //Rechnet die Mittelwerte aus, jedoch nicht über
									  //alle Zeiten sondern in einem gewissen Zeitfenster
									  // -> window
};
#endif