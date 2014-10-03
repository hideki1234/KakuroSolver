/***************************************************************************/
/* testDLL2.cpp                                                            */
/* Quick test for whole process                                            */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/25 Initial                                                      */
/***************************************************************************/

#include <Windows.h>
#include "..\KakuroSolver\kkrsolv.h"
#include "testutil.h"

bool test2()
{
	bool bPassed = true;
	void * pArgData = MakeArgFromTestData_t(TestData1);
	kkrError_t apiRet;
	kkrHandle_t hSolver;
	size_t cSols;
	const unsigned char * pData;
	const size_t cData = TestData1.header.cx * TestData1.header.cy;

	logMessage(L"***** Test case 2: Solve a small problem *****\n");

	logMessage(L"* Create a solver\n");
	apiRet = KkrCreateSolver(pArgData, &hSolver);
	if(apiRet != KKRERR_NO_ERROR) {
		bPassed = false;
		hSolver = 0;
		logMessage(L"\t\t!!!! FAILED !!!!\n");
	}

	if(bPassed) {
		logMessage(L"* Get # of solution: should return error\n");
		apiRet = KkrGetSolutionCount(hSolver, &cSols);
		if(apiRet != KKRERR_NOT_SOLVED_YET) {
			bPassed = false;
			logMessage(L"\t\t!!!! FAILED !!!!\n");
		}
	}

	if(bPassed) {
		logMessage(L"* Get a solution: should return error\n");
		apiRet = KkrGetSolution(hSolver, 0, &pData);
		if(apiRet != KKRERR_NOT_SOLVED_YET) {
			bPassed = false;
			logMessage(L"\t\t!!!! FAILED !!!!\n");
		}
	}

	if(bPassed) {
		logMessage(L"* Solve it!\n");
		apiRet = KkrSolve(hSolver);
		if(apiRet != KKRERR_NO_ERROR) {
			bPassed = false;
			logMessage(L"\t\t!!!! FAILED !!!!\n");
		}
	}

	if(bPassed) {
		logMessage(L"* Get # of solution: shoud be 1\n");
		apiRet = KkrGetSolutionCount(hSolver, &cSols);
		if(apiRet != KKRERR_NO_ERROR || cSols != 1) {
			bPassed = false;
			logMessage(L"\t\t!!!! FAILED !!!!\n");
		}
	}

	if(bPassed) {
		logMessage(L"* Get a solution\n");
		apiRet = KkrGetSolution(hSolver, 0, &pData);
		if(apiRet != KKRERR_NO_ERROR) {
			bPassed = false;
			logMessage(L"\t\t!!!! FAILED !!!!\n");
		} else {
			logSolution(pData, cData);
			logMessage(L"\n");
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
