/***************************************************************************/
/* SetEventHandler.cpp                                                     */
/* KkrSetEventHandler API implementaion                                    */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/10 Initial                                                      */
/***************************************************************************/

#include <Windows.h>
#include "..\KakuroSolver_t\kkrsolv_t.h"
#include "kkrtypes.h"
#include "apiutil.h"

extern "C" __declspec(dllexport) kkrError_t KkrSetEventHandler(kkrHandle_t hBoard, kkrSolveEventRequest_t req, kkrSolveEventHandler_t cb, void * pUser)
{
	kkrError_t retVal = KKRERR_NO_ERROR;

	if(!HandleExists(hBoard))
		return KKRERR_INVALID_HANDLE;

	kakuro::slvEventRequest_t kReq;
	switch(req) {
	case KKRREQEVENT_NONE:
		kReq = kakuro::REQEVENT_NONE;
		break;
	case KKRREQEVENT_SOLUTION:
		kReq = kakuro::REQEVENT_SOLUTION;
		break;
	case KKRREQEVENT_DETERMINED:
		kReq = kakuro::REQEVENT_DETERMINED;
		break;
	case KKRREQEVENT_TEMPORARY:
		kReq = kakuro::REQEVENT_TEMPORARY;
		break;
	default:
		retVal = KKRERR_INVALID_ARGUMENT;
		goto quit;
	}

	kakuro::solver_t * pSolver = reinterpret_cast<kakuro::solver_t *>(hBoard);
	try {
		pSolver->SetEventHandler(kReq, cb, pUser);
	}
	catch(kakuro::slvExceptionInvalidArgument_t & eArg) {
		retVal = KKRERR_INVALID_ARGUMENT;
	}
	catch(kakuro::slvException_t & e) {
		retVal = KKRERR_INTERNAL_ERROR;
	}
	catch(...) {
		retVal = KKRERR_SYSTEM_ERROR;
	}

quit:
	return retVal;
}