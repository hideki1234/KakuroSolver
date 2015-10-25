/***************************************************************************/
/* testSlv5simple.cpp                                                      */
/* test case 5 for Kakuro Solving Engine                                  */
/*    solves small problems with a unique solution                         */
/*    tests events as well                                                 */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/22 Initial                                                      */
/***************************************************************************/
#include <iostream>
#include <memory>
#include "../KakuroSolver_t/kkrsolv_t.h"
#include "../KakuroSolver_t/solvimpl.h"
#include "testSlv.h"

using namespace kakuro;

static const unsigned DATA1[] = {
	2, 2,
	1, 0, 0,	1, 0, 4,	1, 0, 7,
	1, 3, 0,	0, 0, 0,	0, 0, 0,
	1, 8, 0,	0, 0, 0,	0, 0, 0,
};
static const unsigned char SOL1[] = {
	1, 2,
	3, 5,
};

static const unsigned DATA2[] = {
	3, 3,
	1,0,0,	1,0,0,	1,0,6,	1,0,4,
	1,0,0,	1,3,7,	0,0,0,	0,0,0,
	1,6,0,	0,0,0,	0,0,0,	0,0,0,
	1,8,0,	0,0,0,	0,0,0,	1,0,0,
};
static const unsigned char SOL2[] = {
	0, 2, 1,
	2, 1, 3,
	5, 3, 0,
};

static const unsigned DATA3[] = {
	5, 3,
	1,0,0,	1,0,0,	1,0,6,	1,0,5,	1,0,9,	1,0,0,
	1,0,0,	1,7,3,	0,0,0,	0,0,0,	0,0,0,	1,0,8,
	1,15,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,
	1,4,0,	0,0,0,	0,0,0,	1,5,0,	0,0,0,	0,0,0,
};
static const unsigned char SOL3[] = {
	0, 2, 1, 4, 0,
	2, 1, 4, 3, 5,
	1, 3, 0, 2, 3,
};

static const unsigned * TestData[] = {
	DATA1,
	DATA2,
	DATA3,
};
static const unsigned char * TestSol[] = {
	SOL1,
	SOL2,
	SOL3,
};
static const size_t CTestData = sizeof(TestData)/sizeof(TestData[0]);

typedef bool (*evalEvents)();

static bool testCommon(slvEventRequest_t req, eventHandler_t eh, evalEvents fEval)
{
	bool bPassed = true;
	board_t testBoard;

	for(auto i = 0; i < CTestData; i++) {
		bool bPassedThis = true;
		const size_t boardSize = TestData[i][0] * TestData[i][1];
		std::cout << "\ttest " << i << ": ";

		ResetCounts();
		MakeDataBoard(testBoard, TestData[i]);

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
			if(numSol == 1) {
				const unsigned char * solution;
				solver.GetSolution(0, &solution);
				size_t cell;
				for(cell = 0; cell < boardSize; cell++)
					if(solution[cell] != TestSol[i][cell])
						break;
				if(cell != boardSize) {
					bPassedThis = false;
					std::cout << "Solution not match" << std::endl;
					std::cout << "\t\texpected:";
					for(cell = 0; cell < boardSize; cell++)
						std::cout << TestSol[i][cell] << ", ";
					std::cout << "\t\tacutal:";
					for(cell = 0; cell < boardSize; cell++)
						std::cout << solution[cell] << ", ";
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
						std::cout << solution[cell] << ", ";
				}
			}
		}
		catch(slvException_t & e) {
			bPassedThis = false;
			std::cout << "exception caught ";
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

static bool testSolEval()
{
	bool bPassed = true;

	if(CountSol != 1) {
		std::cout << "\t\tEVENT_SOLUTION fired " << CountSol << " times; once is expected" << std::endl;
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

static bool testSol()
{
	std::cout << "*     Solve REQEVENT_SOLUTION" << std::endl;
	return testCommon(REQEVENT_SOLUTION, testHandler, testSolEval);
}

static bool testDetEval()
{
	bool bPassed = true;

	if(CountSol != 1) {
		std::cout << "\t\tEVENT_SOLUTION fired " << CountSol << " times; once is expected" << std::endl;
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

static bool testDet()
{
	std::cout << "*     Solve REQEVENT_DETERMINED" << std::endl;
	return testCommon(REQEVENT_DETERMINED, testHandler, testDetEval);
}

static bool testTmpEval()
{
	bool bPassed = true;

	if(CountSol != 1) {
		std::cout << "\t\tEVENT_SOLUTION fired " << CountSol << " times; once is expected" << std::endl;
		bPassed = false;
	}
	std::cout << "\t\tEVENT_DETERMINED_UPDATE fired " << CountDet << " times" << std::endl;
	std::cout << "\t\tEVENT_TEMPORARY_UPDATE fired " << CountTmp << " times" << std::endl;
	std::cout << "\t\tEVENT_RESET fired " << CountReset << " times" << std::endl;

	return bPassed;
}

static bool testTmp()
{
	std::cout << "*     Solve REQEVENT_TEMPORARY" << std::endl;
	return testCommon(REQEVENT_TEMPORARY, testHandler, testTmpEval);
}

bool test5()
{
	bool bPassed = true;
	std::cout << "***** test case 5: solve small unique problems; Events" << std::endl;
	bVerbose = true;

	bPassed &= testNoEvent();
	bPassed &= testSol();
	bPassed &= testDet();
	bPassed &= testTmp();

	return bPassed;
}
