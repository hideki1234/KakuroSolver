/***************************************************************************/
/* apiutil.cpp                                                             */
/* implementations of utilities for kakuro solver APIs                    */
/*                                                                         */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/11 Initial                                                      */
/***************************************************************************/

#include <set>
#include "..\KakuroSolver_t\kkrsolv_t.h"
#include "kkrtypes.h"
#include "apiutil.h"

static std::set<kkrHandle_t> HandleStore;

APIError_t HandleInsert(kkrHandle_t newHandle)
{
	HandleStore.insert(newHandle);
	return APIERR_NO_ERROR;
}

bool HandleExists(kkrHandle_t aHandle)
{
	return HandleStore.end() != HandleStore.find(aHandle);
}

APIError_t HandleErase(kkrHandle_t aHandle)
{
	auto retVal = HandleStore.erase(aHandle);
	return (retVal > 0 ? APIERR_NO_ERROR : APIERR_HANDLE_NOT_EXISTS);
}
