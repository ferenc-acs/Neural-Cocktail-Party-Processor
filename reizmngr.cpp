///////////////////////////////////////////////////////////////////////////////
//                              reizmngr.cpp
///////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// ROUTINEN ///////////////////////////////
#include <cstring.h>
#include <fstream.h>
#include <limits.h>
#include <iostream.h>
#include <except.h>
#include "contains.hpp"
#include "basicneu.hpp"
#include "timer.hpp"
#include "reizmngr.hpp"

#define TRUE 1
#define FALSE 0
#define IDENT 0x0ABBA0L


Reiz_Manager::Reiz_Manager(timer* a_timer):reize(3,0,2){ //Speicher für 3 Reize
																 //in 2er Schritten erweiterbar
	BeiReizauswertung = FALSE;
	valid = IDENT;
	reizanzahl = 0;
	rparm = 0;
	numneurons = 0;
	time = a_timer;
}

Reiz_Manager::~Reiz_Manager()
{
}

void Reiz_Manager::init_pointers(basic_array* field, unsigned long numb)
{
	unsigned int i;

	//Überprüfen der Arrayangaben
	for(i=0;i<numb;i++)
		if(((*field)[i]->GetNumber()) != i) {
			cerr << "\nReiz_Manager::initpointers Übergebenes Neuronarray ist üngültig !";
			exit(1);
		}

	//Scheinen Richtig zu sein
	neurons=field;
	numneurons=numb;
}

void Reiz_Manager::kitzle(double staerke)
{
	unsigned long actual_time; //Es folgt die Zeitansage....
	unsigned items,numbers, i, c; //Items im Reizcontainer, Zähler
	unsigned long actnumb; //Zähler für Neuronennummer
	unsigned actnumb_2; //dto., nur andere Form

	if (numneurons == 0) {
		cerr << "\nReiz_Manager::kitzle - Keine Neurone übergeben worden !";
		exit(1);
	}
	if ((items=reize.GetItemsInContainer()) == 0) {
		cerr << "\nReiz_Manager::kitzle - Keine Reize vorhanden !";
		exit(1);
	}

	actual_time = time->get_time();

	for(i=0;i<items;i++){  //Reizen
		if ( (actual_time > ((reize[i]).start)) &&
														 (actual_time < ((reize[i]).ende)) ) {
			numbers=reize[i].n_array.GetItemsInContainer();
			for(c=0;c<numbers;c++){
				actnumb=reize[i].n_array[c];
				if (actnumb > UINT_MAX){ //Bereichsüberprüfung
					cerr << "Reiz_Manager::kitzle - Neuronnummer zu groß !";
					exit(1);
				}
				actnumb_2 = (unsigned)actnumb;
				if(actnumb < numneurons){ //Kleiner Beitrag zur Fehlervermeidung
					(*neurons)[actnumb_2]->SetInput(staerke);
				}
			}
		}
	}

}

void Reiz_Manager::werte_aus (string* wort_p)
{
	int minusflag=FALSE;
	long wert;
	int pos=0,inc=0,count=0;
	char ch;
	string bufferl('\0',10),bufferr('\0',10); //Puffer für L- und R-Werte in -
	long in,unter,ober,tausch; //siehe - Routine
	unsigned long se_tausch; //für Start & Ende
	T_Reiz dummy;

	wert=atol(wort_p->c_str());
	if (BeiReizauswertung) rparm++;

	if (rparm == 1) (reize[reizanzahl-1]).start = wert;
	if (rparm == 2) (reize[reizanzahl-1]).ende = wert;

	//Wenn Start>Ende wird eine Fehleingabe angenommen und die Werte
	// werden vertauscht
	if ((rparm == 2) &&
				  ( ((reize[reizanzahl-1]).start) > ((reize[reizanzahl-1]).ende) )){
		se_tausch=(reize[reizanzahl-1]).ende;
		(reize[reizanzahl-1]).ende = (reize[reizanzahl-1]).start;
		(reize[reizanzahl-1]).start = se_tausch;
	}

	if (BeiReizauswertung && (rparm > 2)){ //-Tokens aufschlüsseln
		if(wort_p->contains('-')){
			while ((ch=wort_p->operator()(pos))!='-')
				{ bufferl[inc]=ch; inc++; pos++;}
			bufferl[inc+1]='\0';
			inc=0;
			pos++;

			while (((wort_p->c_str())[count]) != '\0') count++; //Länge ermitteln

			while ( pos <= count )
					{ bufferr[inc]=(wort_p->c_str())[pos]; inc++; pos++;}
			bufferr[inc+1]='\0';

			unter = atol(bufferl.c_str());
			ober = atol(bufferr.c_str());
			if (unter > ober) {tausch = ober; ober=unter; unter=tausch;}

			for (in=unter;in<=ober;in++)
				((reize[reizanzahl-1]).n_array).Add(in);

			minusflag=TRUE;
		}
	}

	if (wert && (!minusflag)) //Nummer erkannt war aber kein - Token
		if(BeiReizauswertung && (rparm > 2))
			((reize[reizanzahl-1]).n_array).Add(wert);

	if (*wort_p == 'R'){
		cout << "\nReiz !";//Neuer Reiz
		BeiReizauswertung=TRUE;
		reize.Add(dummy); //Leeres Objekt, damit Itemcounter erhöht wird
		reizanzahl++;
		rparm=0;
	}

}

#define WS 20
int Reiz_Manager::read_data(char* filename)
{
	ifstream datafile; //Die Dateirepräsentation
	string wort(' ',WS);
	int Pause=FALSE; //Pause in dem Einleseprozeß bei einem Kommentar

	int file_vorhanden = TRUE; //Fehlervariable
	wort.skip_whitespace(TRUE); //Führende Whitespace-Zeichen überspringen

	datafile.open(filename);
	if (datafile.fail()) file_vorhanden = FALSE;

	if (file_vorhanden){
		while (!datafile.eof()) {
			//Kommentare z.B. (KOMMENTAR) überspringen
			wort.read_token(datafile); //Token einlesen
			if (wort.contains('(')) Pause++; //Klammer gefunden
			if (wort.contains(')')) {if(Pause) Pause--; //Klammerebene herabsetzten
										// ohne daß Pause kleiner 0 wird z.B. (KOMMENTAR))
				wort.read_token(datafile);}
			if (Pause) datafile.get(); //Zeichen überspringen
			else {  //Einleseprozeß weiterführen
				cout << wort << '°';
				werte_aus(&wort); //Token verarbeiten
			}
		}
	}

	/////////////////////////// Ausgabe DEBUG
	long anzreiz;
	unsigned i,o,punkte;
	cout << "\n\n Anzahl der Reize: " << reizanzahl;
	anzreiz = reize.GetItemsInContainer();
	cout << "\n Reize im Container: " << anzreiz;

	for(i=0;i<anzreiz;i++){
		cout << "\nReiz: " <<(i+1);
		punkte = ((reize[i]).n_array).GetItemsInContainer();
		cout << " S: " << reize[i].start << " E: " << reize[i].ende;
		cout << " Anz.: " << punkte << "\n   Neurone: ";
		for (o=0;o<punkte;o++) cout << reize[i].n_array[o] << ", ";
	}

	///////////////////////////7 DEBUG Ende
	datafile.close();
	return(file_vorhanden); //Eventuellen Fehler zurückgeben
}



