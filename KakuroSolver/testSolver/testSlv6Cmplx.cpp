/***************************************************************************/
/* testSlv7Cmplx.cpp                                                       */
/* test case 6 for Kakuro Solving Engine: solve complex problems           */
/*    solves small problems with multiple solutions                        */
/*    tests events as well                                                 */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/23 Initial                                                      */
/***************************************************************************/
#include <iostream>
#include <memory>
#include "../KakuroSolver_t/kkrsolv_t.h"
#include "../KakuroSolver_t/solvimpl.h"
#include "testSlv.h"

using namespace kakuro;
typedef struct {
	const unsigned * data;
	const size_t cSols;
	const unsigned char * sols[2];
} testData_t;

static const unsigned D1[] = {
	2, 2,
	1, 0, 0,	1, 0, 3,	1, 0, 3,
	1, 3, 0,	0, 0, 0,	0, 0, 0,
	1, 3, 0,	0, 0, 0,	0, 0, 0,
};
static const unsigned char S1_1[] = {
	1, 2,
	2, 1,
};
static const unsigned char S1_2[] = {
	2, 1,
	1, 2,
};

static const unsigned D2[] = {
	3, 3,
	1, 0, 0,	1, 0, 0,	1, 0, 6,	1, 0, 3,
	1, 0, 0,	1, 3, 3,	0, 0, 0,	0, 0, 0,
	1, 6, 0,	0, 0, 0,	0, 0, 0,	0, 0, 0,
	1, 3, 0,	0, 0, 0,	0, 0, 0,	1, 0, 0,
};
static const unsigned char S2_1[] = {
	0, 1, 2,
	2, 3, 1,
	1, 2, 0,
};
static const unsigned char S2_2[] = {
	0, 2, 1,
	1, 3, 2,
	2, 1, 0,
};

static const unsigned D3[] = {
	3, 3,
	1, 0, 0,	1, 0, 0,	1, 0, 7,	1, 0, 4,
	1, 0, 0,	1, 3, 5,	0, 0, 0,	0, 0, 0,
	1, 8, 0,	0, 0, 0,	0, 0, 0,	0, 0, 0,
	1, 5, 0,	0, 0, 0,	0, 0, 0,	1, 0, 0,
};
static const unsigned char S3_1[] = {
	0, 2, 1,
	1, 4, 3,
	4, 1, 0,
};
static const unsigned char S3_2[] = {
	0, 2, 1,
	4, 1, 3,
	1, 4, 0,
};

static const testData_t DATA[] = {
	{D1, 2, {S1_1, S1_2}},
	{D2, 2, {S2_1, S2_2}},
	{D3, 2, {S3_1, S3_2}},
};
static const size_t CDATA = sizeof(DATA)/sizeof(DATA[0]);

static size_t CExpected;

typedef bool (*evalEvents)();

bool testSol(const testData_t & data, const unsigned char * sol, size_t size)
{
	for(size_t solId = 0; solId < data.cSols; ++solId) {
		size_t i;
		for(i = 0; i < size; ++i) {
			if(data.sols[solId][i] != sol[i])
				break;
		}
		if(i == size)
			return true;
	}

	return false;
}

static bool testCommon(slvEventRequest_t req, eventHandler_t eh, evalEvents fEval)
{
	bool bPassed = true;
	board_t testBoard;

	for(auto i = 0; i < CDATA; i++) {
		bool bPassedThis = true;
		const size_t boardSize = DATA[i].data[0] * DATA[i].data[1];
		std::cout << "\ttest " << i << ": ";

		ResetCounts();
		CExpected = DATA[i].cSols;
		MakeDataBoard(testBoard, DATA[i].data);

		try {
			solverImpl_t solver(testBoard);
			if(req != REQEVENT_NONE) {
				Size = boardSize;
				solver.SetEventHandler(req, eh, nullptr);
			}
			solver.Solve();
			if(fEval != nullptr)
				bPassedThis &= fEval();
			const size_t numSol = solver.GetSolutionCount();
			if(numSol == DATA[i].cSols) {
				for(size_t solId = 0; solId < numSol; ++solId) {
					const unsigned char * solution;
					solver.GetSolution(solId, &solution);
					if(!testSol(DATA[i], solution, boardSize)) {
						bPassedThis = false;
						std::cout << "Solution not match" << std::endl;
						std::cout << "\t\tacutal:\n\t\t";
						size_t cell;
						for(cell = 0; cell < boardSize; cell++)
							std::cout << static_cast<unsigned>(solution[cell]) << ", ";
						std::cout << "\t\texpected:";
						for(size_t solJ = 0; solJ < DATA[i].cSols; ++solJ) {
							std::cout << "\n\t\t";
							for(cell = 0; cell < boardSize; cell++)
								std::cout << static_cast<unsigned>(DATA[i].sols[solId][cell]) << ", ";
						}
					}
				}
			} else {
				// doesn't have a unique solution
				bPassedThis = false;
				std::cout << "# of Solutions: " << numSol << std::endl;
				for(size_t solId = 0; solId < numSol; solId++) {
					const unsigned char * solution;
					solver.GetSolution(solId, &solution);
					std::cout << "\t\t";
					for(size_t cell = 0; cell < boardSize; cell++)
						std::cout << static_cast<unsigned>(solution[cell]) << ", ";
				}
			}
		}
		catch(slvEceptionSolverBug_t & eBug) {
			bPassedThis = false;
			std::cout << "Bug exception caught: line=" << eBug.GetLine()
				<< ", filename=" <<eBug.GetFileName();
		}
		catch(slvException_t & e) {
			bPassedThis = false;
			std::cout << "exception caught " << e.GetType();
		}

		delete [] testBoard.pData;

		std::cout << (bPassedThis ? "succeeded" : "!!!! FAILED !!!!")
			<< std::endl;
		bPassed &= bPassedThis;
	}

	return bPassed;
}

static bool testNoEvent()
{
	std::cout << "*     Solve without events" << std::endl;
	return testCommon(REQEVENT_NONE, nullptr, nullptr);
}

bool testSolEval()
{
	bool bPassed = true;

	if(CountSol != CExpected) {
		std::cout << "\t\tEVENT_SOLUTION fired " << CountSol << " times; "
			<< CExpected << " is expected" << std::endl;
		bPassed = false;
	}
	if(CountDet != 0) {
		std::cout << "\t\tEVENT_DETERMINED_UPDATE falsely fired" << std::endl;
		bPassed = false;
	}
	if(CountTmp != 0) {
		std::cout << "\t\tEVENT_TEMPORARY_UPDATE falsely fired" << std::endl;
		bPassed = false;
	}
	if(CountReset != 0) {
		std::cout << "\t\tEVENT_RESET falsely fired" << std::endl;
		bPassed = false;
	}

	return bPassed;
}

bool testSol()
{
	std::cout << "*     Solve REQEVENT_SOLUTION" << std::endl;
	return testCommon(REQEVENT_SOLUTION, testHandler, testSolEval);
}

bool testDetEval()
{
	bool bPassed = true;

	if(CountSol != CExpected) {
		std::cout << "\t\tEVENT_SOLUTION fired " << CountSol << " times; "
			<< CExpected << " is expected" << std::endl;
		bPassed = false;
	}
	std::cout << "\t\tEVENT_DETERMINED_UPDATE fired " << CountDet <<" times" << std::endl;
	if(CountTmp != 0) {
		std::cout << "\t\tEVENT_TEMPORARY_UPDATE falsely fired" << std::endl;
		bPassed = false;
	}
	if(CountReset != 0) {
		std::cout << "\t\tEVENT_RESET falsely fired" << std::endl;
		bPassed = false;
	}

	return bPassed;
}

bool testDet()
{
	std::cout << "*     Solve REQEVENT_DETERMINED" << std::endl;
	return testCommon(REQEVENT_DETERMINED, testHandler, testDetEval);
}

bool testTmpEval()
{
	bool bPassed = true;

	if(CountSol != CExpected) {
		std::cout << "\t\tEVENT_SOLUTION fired " << CountSol << " times; "
			<< CExpected << " is expected" << std::endl;
		bPassed = false;
	}
	std::cout << "\t\tEVENT_DETERMINED_UPDATE fired " << CountDet << " times" << std::endl;
	std::cout << "\t\tEVENT_TEMPORARY_UPDATE fired " << CountTmp << " times" << std::endl;
	std::cout << "\t\tEVENT_RESET fired " << CountReset << " times" << std::endl;

	return bPassed;
}

bool testTmp()
{
	std::cout << "*     Solve REQEVENT_TEMPORARY" << std::endl;
	return testCommon(REQEVENT_TEMPORARY, testHandler, testTmpEval);
}

bool test6()
{
	bool bPassed = true;
	std::cout << "***** test case 6: solve small problems with muliple solutions; Events" << std::endl;
	bVerbose = true;

	bPassed &= testNoEvent();
	bPassed &= testSol();
	bPassed &= testDet();
	bPassed &= testTmp();

	return bPassed;
}
