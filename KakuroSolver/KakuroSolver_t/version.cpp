/***************************************************************************/
/* version.cpp                                                             */
/* version implementation                                                  */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/25 Initial                                                      */
/***************************************************************************/

#include "kkrsolv_t.h"

const int Major = 1;
const int Minor = 2;
const int Release = 4;

namespace kakuro {
	void GetVersionNumber(int * major, int * minor, int * release)
	{
		if(major != nullptr)
			*major = Major;
		if(minor != nullptr)
			*minor = Minor;
		if(release != nullptr)
			*release = Release;
	}
}