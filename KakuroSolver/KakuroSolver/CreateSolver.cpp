/***************************************************************************/
/* CreateSolver.cpp                                                        */
/* KkrCreateSolver API implementaion                                       */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/10 Initial                                                      */
/***************************************************************************/

#include <Windows.h>
#include "..\KakuroSolver_t\kkrsolv_t.h"
#include "kkrtypes.h"
#include "apiutil.h"

const unsigned int CurrentVersion = 1;

extern "C" __declspec(dllexport) kkrError_t KkrCreateSolver(void * pBoardData, kkrHandle_t * pHBoard)
{
	kkrError_t retVal = KKRERR_NO_ERROR;
	kkrHandle_t newHandle = 0;

	ClearErrorDetail();

	// argument validation
	if(pBoardData == nullptr) {
		SetErrorDetail(KKRRSN_NO_BOARD_DATA, 0,  0);
		return KKRERR_INVALID_BOARD_DATA;
	}
	if(pHBoard == nullptr)
		return KKRERR_INVALID_ARGUMENT;
	
	const boardHeader_t * pHeader = reinterpret_cast<boardHeader_t *>(pBoardData);
	if(pHeader->version != CurrentVersion) {
		SetErrorDetail(KKRRSN_INVALID_VERSION, 0, 0);
		return KKRERR_INVALID_BOARD_DATA;
	}

	const cellInfo_t * pSrc = reinterpret_cast<cellInfo_t *>(reinterpret_cast<char *>(pBoardData) + sizeof(boardHeader_t));
	const size_t size = (pHeader->cx + 1) * (pHeader->cy + 1);
	kakuro::board_t dst;
	dst.cx = pHeader->cx;
	dst.cy = pHeader->cy;
	dst.pData = new kakuro::cell_t[size];

	for(size_t i = 0; i < size; i++, pSrc++) {
		switch(*pSrc) {
		case SUM_CELL_MARKER:
			dst.pData[i].isSum = true;
			dst.pData[i].sumRight = *++pSrc;
			dst.pData[i].sumDown = *++pSrc;
			break;
		case ANSWER_CELL_MARKER:
			dst.pData[i].isSum = false;
			dst.pData[i].sumRight =
			dst.pData[i].sumDown = 0;
			break;
		default:
			SetErrorDetail(KKRRSN_INVALID_FORMAT, (i % (dst.cx + 1)), (i / (dst.cx + 1)));
			retVal = KKRERR_INVALID_BOARD_DATA;
			goto quit;
		}
	}

	try {
		kakuro::solver_t * solver = kakuro::SolverFactory(dst);
		newHandle = reinterpret_cast<kkrHandle_t>(solver);
		HandleInsert(newHandle);
	}
	catch(kakuro::slvExceptionInvlideBoardData_t & eB) {
		SetErrorDetail(eB);
		retVal = KKRERR_INVALID_BOARD_DATA;
	}
	catch(kakuro::slvEceptionSolverBug_t & eBug) {
		retVal = KKRERR_INTERNAL_ERROR;
	}
	catch(...) {
		retVal = KKRERR_SYSTEM_ERROR;
	}

quit:
	delete [] dst.pData;
	*pHBoard = newHandle;

	return retVal;
}
