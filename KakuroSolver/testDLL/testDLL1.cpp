#include <Windows.h>
#include "..\KakuroSolver\kkrsolv.h"
#include "testutil.h"

bool test1()
{
	bool testResult;
	kkrError_t apiRet;
	size_t testCount = 0;
	size_t succeeded = 0;

	kkrHandle_t hKkr;

	logMessage(L"***** Test case 1: Invalid parameter *****\n");

	logMessage(L"* KkrCreateSolver invalid board data");
	apiRet = KkrCreateSolver(NULL, &hKkr);
	testResult = (apiRet == KKRERR_INVALID_BOARD_DATA);
	logResult(nullptr, testResult);
	testCount++;
	if(testResult)
		succeeded++;

	logMessage(L"* KkrCreateSolver null pointer to handle");
	apiRet = KkrCreateSolver("dummy", nullptr);
	testResult = (apiRet == KKRERR_INVALID_ARGUMENT);
	logResult(nullptr, testResult);
	testCount++;
	if(testResult)
		succeeded++;

	return true;
}
