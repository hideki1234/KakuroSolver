/***************************************************************************/
/* testutil.h                                                              */
/* test utilities for the test program of KakuroSolver DLL                 */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/11 Initial                                                      */
/***************************************************************************/

/*
** common test data
*/
struct testData_t {
	boardHeader_t header;
	const cellInfo_t * pCellInfo;
};

extern const testData_t TestData1;	// small, unique solution
extern const testData_t TestData2;	// small, multiple solutions

void * MakeArgFromTestData_t(const testData_t & data);
void ReleaseArg(void * pArgData);


/*
** logging utilities
*/
void logMessage(const wchar_t * pMessage);
void logResult(const wchar_t * pMessage, bool testResult);
void logValue(size_t value);
void logValue(int value);
void logSolution(const unsigned char * pSol, size_t size);