#include <iostream>
#include "..\KakuroSolver\kkrsolv.h"
#include "testutil.h"

using namespace std;

/*
** common test data
*/
void * MakeArgFromTestData_t(const testData_t & data)
{
	const size_t brdSize = (data.header.cx + 1) * (data.header.cy + 1);
	size_t cSum = 0;
	size_t cAns = 0;
	const cellInfo_t * pCurrent;
	size_t i;
	for(pCurrent = data.pCellInfo, i = 0; i < brdSize; i++, pCurrent++) {
		if(*pCurrent == SUM_CELL_MARKER) {
			cSum++;
			pCurrent += 2;
		} else
			cAns++;
	}

	const size_t cellInfoSize = 3 * cSum + cAns;
	cellInfo_t * const pArgData = new cellInfo_t[sizeof(boardHeader_t) + cellInfoSize];
	cellInfo_t * pTarget = pArgData;
	for(i = 0; i < sizeof(boardHeader_t); i++)
		*(pTarget++) = *(reinterpret_cast<const cellInfo_t *>(&data.header) + i);
	for(i = 0; i < cellInfoSize; i++)
		*(pTarget++) = *(data.pCellInfo + i);

	return pArgData;
}

void ReleaseArg(void * pArgData)
{
	cellInfo_t * pToDelete = reinterpret_cast<cellInfo_t *>(pArgData);
	delete [] pToDelete;
}


/*
** logging utilities
*/
void logMessage(const wchar_t * pMessage)
{
	wcout << pMessage;
}

void logResult(const wchar_t * pMessage, bool testResult)
{
	if(pMessage != nullptr)
		wcout << pMessage;
	wcout << (testResult ? L" Succeeded" : L" !!!! FAILED !!!!") << endl;
}

void logValue(size_t value)
{
	wcout << value;
}

void logValue(int value)
{
	wcout << value;
}

void logSolution(const unsigned char * pSol, size_t size)
{
	for(size_t i = 0; i < size; i++)
		cout << static_cast<unsigned int>(*(pSol + i)) << ", ";
}
