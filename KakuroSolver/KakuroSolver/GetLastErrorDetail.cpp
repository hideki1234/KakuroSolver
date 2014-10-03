/***************************************************************************/
/* GetLastErrorDetail.cpp                                                  */
/* KkrGetLastErrorDetail API implementation                                */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/11/02 Initial                                                      */
/***************************************************************************/

#include "..\KakuroSolver_t\kkrsolv_t.h"
#include "kkrtypes.h"

static bool HasError = false;
static kkrErrorReason_t Reason;
static size_t CellX;
static size_t CellY;

void ClearErrorDetail()
{
	HasError = false;
}

void SetErrorDetail(kkrErrorReason_t reason, size_t x, size_t y)
{
	HasError = true;
	Reason = reason;
	CellX = x;
	CellY = y;
}

void SetErrorDetail(const kakuro::slvExceptionInvlideBoardData_t & e)
{
	typedef kakuro::slvExceptionInvlideBoardData_t::reason_t orgReason_t;
	switch(e.GetReason()) {
	case orgReason_t::EDGE_CELL:
		Reason = KKRRSN_EDGE_CELL;
		break;
	case orgReason_t::SUM4EMPTY:
		Reason = KKRRSN_SUM4EMPTY;
		break;
	case orgReason_t::NUM_CELL:
		Reason = KKRRSN_NUM_CELL;
		break;
	case orgReason_t::SUM:
		Reason = KKRRSN_SUM;
		break;
	case orgReason_t::BOARD_SIZE:
		Reason = KKRRSN_BOARD_SIZE;
		break;
	case orgReason_t::NO_BOARD_DATA:
		Reason = KKRRSN_NO_BOARD_DATA;
		break;
	default:
		Reason = KKRRSN_UNKNOWN;
		break;
	}
	e.GetPosition(&CellX, &CellY);
	HasError = true;
}

extern "C" __declspec(dllexport) kkrError_t KkrGetLastErrorDetail(kkrErrorReason_t * pReason, size_t * px, size_t * py)
{
	if(!HasError)
		return KKRERR_NO_REASON;

	*pReason = Reason;
	*px = CellX;
	*py = CellY;

	return KKRERR_NO_ERROR;
}
