/***************************************************************************/
/* GetSolution.cpp                                                         */
/* KkrGetSolution API implementation                                       */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/10 Initial                                                      */
/***************************************************************************/

#include <Windows.h>
#include "..\KakuroSolver_t\kkrsolv_t.h"
#include "kkrtypes.h"
#include "apiutil.h"

extern "C" __declspec(dllexport) kkrError_t KkrGetSolution(kkrHandle_t hBoard, size_t idx, const unsigned char ** ppData)
{
	kkrError_t retVal = KKRERR_NO_ERROR;

	if(!HandleExists(hBoard))
		return KKRERR_INVALID_HANDLE;

	kakuro::solver_t * pSolver = reinterpret_cast<kakuro::solver_t *>(hBoard);
	try {
		pSolver->GetSolution(idx, ppData);
	}
	catch(kakuro::slvExceptionNotSolvedYet_t & eNotYet) {
		retVal = KKRERR_NOT_SOLVED_YET;
	}
	catch(kakuro::slvExceptionInvalidArgument_t & eArg) {
		retVal = KKRERR_INVALID_INDEX;
	}
	catch(kakuro::slvEceptionSolverBug_t & eBug) {
		retVal = KKRERR_INTERNAL_ERROR;
	}
	catch(...) {
		retVal = KKRERR_SYSTEM_ERROR;
	}

	return retVal;
}
