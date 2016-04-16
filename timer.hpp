/////////////////////////////////////////////////////////////////////////
//        							timer.hpp                     
/////////////////////////////////////////////////////////////////////////


// Diese Klasse stellt Funktionen zur Verwaltung der globalen Simulations-
// zeit zur Verf�gung
// 8.8.94: Start der Klassenentwicklung
// 17.8.94: Klassenentwicklung fertiggestellt

//////////////////// Klassendefinition
#ifndef TIMER_HPP
#define TIMER_HPP //Doppel include Check
class huge timer {
	private:
	unsigned long millsec;//Die eigentliche Timervariable (Zeit in Millisekunden)

	public:
	timer(); //setzt die Zeit auf 0;
	~timer(); //Nur da, weil der Konstruktor da ist
	long int get_time(); //Liefert die aktuelle Zeit
	void set_time(long int); //Setzt die aktuelle Zeit
	timer& operator++(); //Erh�ht die Zeit um 1
	timer& operator--(); //Vermindet die Zeit um 1
};  
#endif
