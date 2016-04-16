///////////////////////////////////////////////////////////////////////////////
//                              timer.cpp
///////////////////////////////////////////////////////////////////////////////

///////////////// Funktionendefinition
#include "timer.hpp"

timer::timer()
{
	millsec=0;
}

timer::~timer()
{}

long int timer::get_time()
{
 return (millsec);
}

void timer::set_time(long int atime)
{
	millsec=atime;
}

timer& timer::operator++()
{
	millsec++;
	return ((timer&) *this);
}

timer& timer::operator--()
{
	millsec--;
	return ((timer&) *this);
}

