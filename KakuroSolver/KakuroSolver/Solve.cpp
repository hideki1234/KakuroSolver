/***************************************************************************/
/* Solve.cpp                                                               */
/* KkrSolve API implementaion                                              */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/10 Initial                                                      */
/***************************************************************************/

#include <Windows.h>
#include "..\KakuroSolver_t\kkrsolv_t.h"
#include "kkrtypes.h"
#include "apiutil.h"

extern "C" __declspec(dllexport) kkrError_t KkrSolve(kkrHandle_t HBoard)
{
	kkrError_t retVal = KKRERR_NO_ERROR;

	if(!HandleExists(HBoard))
		return KKRERR_INVALID_HANDLE;

	kakuro::solver_t * pSolver = reinterpret_cast<kakuro::solver_t *>(HBoard);
	try {
		pSolver->Solve();
	}
	catch(kakuro::slvException_t & eKkr) {
		retVal = KKRERR_INTERNAL_ERROR;
	}
	catch(...) {
		retVal = KKRERR_SYSTEM_ERROR;
	}

	return retVal;
}
