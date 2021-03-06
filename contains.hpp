///////////////////////////////////////////////////////////////////////////////
//                              contains.hpp
///////////////////////////////////////////////////////////////////////////////



/////////////////// Containerklassen f�r
//   1. unsigned long Werte
//   2. Array von T_Reizobjekten

#include <classlib\arrays.h>

#ifndef CONTAINS_HPP
#define CONTAINS_HPP

///////////////////////////////////////////////////////////////////////////////
class huge T_Reiz{
	public:
	unsigned long start;
	unsigned long ende;
	TArrayAsVector<unsigned long> n_array;

	T_Reiz(); //Initialisiert die long Array Parameter
	~T_Reiz();
	int operator == (T_Reiz);
};
///////////////////////////////////////////////////////////////////////////////

typedef TArrayAsVector<T_Reiz> T_ReizArray;

#endif
