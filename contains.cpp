///////////////////////////////////////////////////////////////////////////////
//                              contains.cpp
///////////////////////////////////////////////////////////////////////////////



#include "contains.hpp"
#include <iostream.h>
#include <except.h>

#define TRUE 1
#define FALSE 0
////////////////////////// Konstuktor zu T_Reiz  //////////////////////////////
T_Reiz::T_Reiz():n_array(5,0,5) {}  //LongArray hat 5 Speicherplätze und wird
										//jeweils um 5 erweitert. Basis bei Null
T_Reiz::~T_Reiz() {}

T_Reiz::operator == (T_Reiz anderer)
{
	int erg=TRUE;
	unsigned c,i;

	if ((start != anderer.start) && (ende != anderer.ende))
		erg=FALSE;
	else
		if (((c=n_array.GetItemsInContainer())!=
													  anderer.n_array.GetItemsInContainer()))
			erg=FALSE;
		else
			for(i=0;i<c;i++)
				if(n_array[c] != anderer.n_array[c])
					{erg=FALSE; break;}

	return(erg);
}
