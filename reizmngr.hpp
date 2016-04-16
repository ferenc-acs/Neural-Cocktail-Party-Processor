///////////////////////////////////////////////////////////////////////////////
//                              reizmngr.hpp
///////////////////////////////////////////////////////////////////////////////



// Soll die Reize aus einer Datei einlesen und verwalten


////////////////////////////////////// HEADER ////////////////////////////////////////
#include <fstream.h>
#include <cstring.h>
#include "contains.hpp"
#include "basicneu.hpp"
#include "timer.hpp"

#ifndef REIZMGR_HPP
#define REIZMGR_HPP

class huge Reiz_Manager{
	private:
	int BeiReizauswertung; //Gibt an, ob gerade ein Reiz ausgewertet wird
	int rparm; //Nummer des Reizparameters
	unsigned int reizanzahl;
	void werte_aus(string*); //Wertet den Ausdruck aus
	T_ReizArray reize; //Reize
	basic_array* neurons;   //Pointer auf Neurone
	unsigned long numneurons; //Anzahl der Neurone
	timer* time; //Pointer auf das Timerobjekt

	public:
	long int valid; //Ist das Objekt inititalisiert ? (Objektkennung)
	Reiz_Manager(timer*); //Pointer auf das Timerobjekt muß übergeben werden.
	~Reiz_Manager();
	int read_data(char*);
	void init_pointers(basic_array* ,unsigned long); //Initialisiert die Pointer
	void kitzle(double staerke = 1.0);

};

#endif  //Ende Doppel-include Check
