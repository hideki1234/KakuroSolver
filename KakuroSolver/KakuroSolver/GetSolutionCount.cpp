/***************************************************************************/
/* GetSolutionCount.cpp                                                    */
/* KkrGetSolutionCount API implementation                                  */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/10 Initial                                                      */
/***************************************************************************/

#include <Windows.h>
#include "..\KakuroSolver_t\kkrsolv_t.h"
#include "kkrtypes.h"
#include "apiutil.h"

extern "C" __declspec(dllexport) kkrError_t KkrGetSolutionCount(kkrHandle_t HBoard, size_t * pCount)
{
	kkrError_t retVal = KKRERR_NO_ERROR;

	if(!HandleExists(HBoard))
		return KKRERR_INVALID_HANDLE;

	kakuro::solver_t * pSolver = reinterpret_cast<kakuro::solver_t *>(HBoard);
	try {
		*pCount = pSolver->GetSolutionCount();
	}
	catch(kakuro::slvExceptionNotSolvedYet_t & e) {
		retVal = KKRERR_NOT_SOLVED_YET;
	}
	catch(...) {
		retVal = KKRERR_SYSTEM_ERROR;
	}

	return retVal;
}
