/***************************************************************************/
/* DeleteSolver.cpp                                                        */
/* KkrDeleteSolver API implementation                                      */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/10 Initial                                                      */
/***************************************************************************/

#include <Windows.h>
#include "..\KakuroSolver_t\kkrsolv_t.h"
#include "kkrtypes.h"
#include "apiutil.h"

extern "C" __declspec(dllexport) kkrError_t KkrDeleteSolver(kkrHandle_t hBoard)
{
	if(!HandleExists(hBoard))
		return KKRERR_INVALID_HANDLE;
	kakuro::solver_t * pSolver = reinterpret_cast<kakuro::solver_t *>(hBoard);
	delete pSolver;
	HandleErase(hBoard);

	return KKRERR_NO_ERROR;
}
