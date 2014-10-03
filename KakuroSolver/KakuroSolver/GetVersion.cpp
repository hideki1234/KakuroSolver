/***************************************************************************/
/* GetVersion.cpp                                                          */
/* KkrGetVersion API implementation                                        */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/25 Initial                                                      */
/***************************************************************************/

#include "..\KakuroSolver_t\kkrsolv_t.h"

extern "C" __declspec(dllexport) void KkrGetVersion(int * pMajor, int * pMinor, int * pRelease)
{
	kakuro::GetVersionNumber(pMajor, pMinor, pRelease);
}