/***************************************************************************/
/* testDLL2.cpp                                                            */
/* Events                                                                  */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/25 Initial                                                      */
/***************************************************************************/

#include <Windows.h>
#include "..\KakuroSolver\kkrsolv.h"
#include "testutil.h"

static size_t	UserData = 1;

static size_t	CSols;
static size_t	CDets;
static size_t	CTmps;
static size_t	CRsts;
static size_t	CUnkwn;

static void ResetCounters()
{
	CSols = CDets = CTmps = CRsts = CUnkwn = 0;
}

static void eh(kkrSolveEvent_t ev, void * user, void * data)
{
	switch(ev) {
	case KKREVENT_SOLUTION:
		CSols++;
		logMessage(L"\t\tKKREVENT_SOLUTION");
		break;
	case KKREVENT_DETERMINED_UPDATE:
		CDets++;
		logMessage(L"\t\tKKREVENT_DETERMINED_UPDATE");
		break;
	case KKREVENT_TEMPORARY_UPDATE:
		CTmps++;
		logMessage(L"\t\tKKREVENT_TEMPORARY_UPDATE");
		break;
	case KKREVENT_RESET:
		CRsts++;
		logMessage(L"\t\tKKREVENT_RESET");
		break;
	default:
		CUnkwn++;
		logMessage(L"\t\t!!!! Unknown event !!!!");
		break;
	}

	wchar_t * pUserMsg;
	pUserMsg = (reinterpret_cast<size_t *>(user) == &UserData ? L": UserData correct\n" : L": UserData !!!! FAILED !!!!");
	logMessage(pUserMsg);
}

bool test3()
{
	bool bPassed = true;

	logMessage(L"***** Test case 3: Event test *****\n");

	void * pArgData = MakeArgFromTestData_t(TestData2);
	kkrError_t apiRet;
	kkrHandle_t hSolver;
	size_t cSols;
	const unsigned char * pData;
	const size_t cData = TestData2.header.cx * TestData2.header.cy;

	logMessage(L"* Create a solver\n");
	apiRet = KkrCreateSolver(pArgData, &hSolver);
	if(apiRet != KKRERR_NO_ERROR) {
		bPassed = false;
		hSolver = 0;
		logMessage(L"\t\t!!!! FAILED !!!!\n");
	}

	if(bPassed) {
		logMessage(L"* Set event handler\n");
		apiRet = KkrSetEventHandler(hSolver, KKRREQEVENT_TEMPORARY, eh, reinterpret_cast<void*>(&UserData));
		if(apiRet != KKRERR_NO_ERROR) {
			bPassed = false;
			logMessage(L"\t\t!!!! FAILED !!!!\n");
		}
	}

	if(bPassed) {
		logMessage(L"* Solve it!\n");
		ResetCounters();
		apiRet = KkrSolve(hSolver);
		if(apiRet != KKRERR_NO_ERROR) {
			bPassed = false;
			logMessage(L"\t\t!!!! FAILED !!!!\n");
		}
	}

	if(bPassed) {
		logMessage(L"* Event summary\n");
		logMessage(L"\t\tKKREVENT_SOLUTION: ");
		logValue(CSols);

		logMessage(L"\t\tKKREVENT_DETERMINED_UPDATE: ");
		logValue(CDets);

		logMessage(L"\t\tKKREVENT_TEMPORARY_UPDATE: ");
		logValue(CTmps);

		logMessage(L"\t\tKKREVENT_RESET: ");
		logValue(CRsts);

		if(CUnkwn != 0) {
			bPassed = false;
			logMessage(L"\t\tUnknown events");
			logMessage(L"\t\t!!!! FAILED !!!!\n");
		}
	}

	if(bPassed) {
		logMessage(L"* Get # of solution: shoud be 2\n");
		apiRet = KkrGetSolutionCount(hSolver, &cSols);
		if(apiRet != KKRERR_NO_ERROR || cSols != 2) {
			bPassed = false;
			logMessage(L"\t\t!!!! FAILED !!!!\n");
		}
	}

	if(bPassed) {
		logMessage(L"* Get the first solution\n");
		apiRet = KkrGetSolution(hSolver, 0, &pData);
		if(apiRet != KKRERR_NO_ERROR) {
			bPassed = false;
			logMessage(L"\t\t!!!! FAILED !!!!\n");
		} else {
			logSolution(pData, cData);
			logMessage(L"\n");
		}
	}

	if(bPassed) {
		logMessage(L"* Get the second solution\n");
		apiRet = KkrGetSolution(hSolver, 1, &pData);
		if(apiRet != KKRERR_NO_ERROR) {
			bPassed = false;
			logMessage(L"\t\t!!!! FAILED !!!!\n");
		} else {
			logSolution(pData, cData);
			logMessage(L"\n");
		}
	}

	if(bPassed) {
		logMessage(L"* Get the third solution; should be error\n");
		apiRet = KkrGetSolution(hSolver, 2, &pData);
		if(apiRet != KKRERR_INVALID_INDEX) {
			bPassed = false;
			logMessage(L"\t\t!!!! FAILED !!!!\n");
		}
	}

	if(hSolver != 0) {
	logMessage(L"* Delete the solver\n");
		apiRet = KkrDeleteSolver(hSolver);
		if(apiRet != KKRERR_NO_ERROR) {
			bPassed = false;
			logMessage(L"\t\t!!!! FAILED !!!!\n");
		}
	}

	ReleaseArg(pArgData);
	return bPassed;
}
