/***************************************************************************/
/* commondata.cpp                                                          */
/* common test data for the test program of KakuroSolver DLL               */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/25 Initial                                                      */
/***************************************************************************/

#include "..\KakuroSolver\kkrsolv.h"
#include "testutil.h"

static cellInfo_t C1[] = {
	SUM_CELL_MARKER,  0,  0,
	SUM_CELL_MARKER,  0,  4,
	SUM_CELL_MARKER,  0,  7,

	SUM_CELL_MARKER,  3,  0,
	0,
	0,

	SUM_CELL_MARKER,  8,  0,
	0,
	0,
};
const testData_t TestData1 = {
	{ 1,  2,  2, },
	C1,
};

static cellInfo_t C2[] = {
	SUM_CELL_MARKER,  0,  0,
	SUM_CELL_MARKER,  0,  0,
	SUM_CELL_MARKER,  0,  7,
	SUM_CELL_MARKER,  0,  4,

	SUM_CELL_MARKER,  0,  0,
	SUM_CELL_MARKER,  3,  5,
	0,
	0,

	SUM_CELL_MARKER,  8,  0,
	0,
	0,
	0,

	SUM_CELL_MARKER,  5,  0,
	0,
	0,
	SUM_CELL_MARKER,  0,  0,
};
const testData_t TestData2 = {
	{ 1,  3,  3, },
	C2,
};
