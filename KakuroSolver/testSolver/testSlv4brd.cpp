/***************************************************************************/
/* testSlv4brd.cpp                                                         */
/* test case 4 for Kakuro Solving Engine: solverImple_t                    */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/20 Initial                                                      */
/***************************************************************************/

#include <iostream>
#include <memory>
#include "..\KakuroSolver_t\kkrsolv_t.h"
#include "..\KakuroSolver_t\solvimpl.h"
#include "testSlv.h"

using namespace kakuro;

typedef std::set<size_t> indexSet_t;


class solverImple_t_test : public solverImpl_t {
public:
	solverImple_t_test(const board_t & boardData) : solverImpl_t(boardData) {}
	~solverImple_t_test();

	void GetIndecesOfUndet(indexSet_t & result) const;
	size_t GetNumOfSets(void) const {return m_initialBoard->cellSetSet.size();}
};

solverImple_t_test::~solverImple_t_test()
{
}

void solverImple_t_test::GetIndecesOfUndet(indexSet_t & result) const
{
	for(auto item : m_initialBoard->undetCellSet)
		result.insert(item->GetIndex());
}

static const unsigned DATA1[] = {
	2, 2,
	1, 0, 0,	1, 0, 4,	1, 0, 7,
	1, 3, 0,	0, 0, 0,	0, 0, 0,
	1, 8, 0,	0, 0, 0,	0, 0, 0,
};
static const size_t DATA1INDECES[] = {4, 0, 1, 2, 3};
static const size_t DATA1SUMS = 4;

static const unsigned DATA2[] = {
	3, 3,
	1,0,0,	1,0,0,	1,0,6,	1,0,4,
	1,0,0,	1,3,4,	0,0,0,	0,0,0,
	1,6,0,	0,0,0,	0,0,0,	0,0,0,
	1,8,0,	0,0,0,	0,0,0,	1,0,0,
};
static const size_t DATA2INDECES[] = {7, 1, 2, 3, 4, 5, 6, 7};
static const size_t DATA2SUMS = 6;

static const unsigned DATA3[] = {
	5, 3,
	1,0,0,	1,0,0,	1,0,6,	1,0,5,	1,0,9,	1,0,0,
	1,0,0,	1,7,3,	0,0,0,	0,0,0,	0,0,0,	1,0,8,
	1,15,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,
	1,4,0,	0,0,0,	0,0,0,	1,5,0,	0,0,0,	0,0,0,
};
static const size_t DATA3INDECES[] = {12, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 13, 14};
static const size_t DATA3SUMS = 9;

static const unsigned * TestData[] = {
	DATA1,
	DATA2,
	DATA3,
};
static const size_t * TestIndeces[] = {
	DATA1INDECES,
	DATA2INDECES,
	DATA3INDECES,
};
static const size_t TestSums[] = {
	DATA1SUMS,
	DATA2SUMS,
	DATA3SUMS,
};

static const size_t CTestData = sizeof(TestData)/sizeof(TestData[0]);

static bool testACtor(const unsigned * const data, const size_t * const indeces, const size_t sums)
{
	bool bPassed = true;
	size_t c;
	board_t testBoard;

	MakeDataBoard(testBoard, data);

	const unsigned * val;
	indexSet_t indecesExpected;
	c = indeces[0];
	for(val = indeces + 1; c-- > 0; ++val)
		indecesExpected.insert(*val);

	try {
		indexSet_t indecesResult;
		solverImple_t_test solver(testBoard);
		solver.GetIndecesOfUndet(indecesResult);

		std::cout << "\tIndeces: ";
		if(indecesExpected == indecesResult)
			std::cout << "succeeded" << std::endl;
		else {
			auto pl = [](const indexSet_t & s) {
				for(auto i: s)
					std::cout << " " << i;
			};
			std::cout << "!!!! FAILED !!!! expected:";
			pl(indecesExpected);
			std::cout << " Actual:";
			pl(indecesResult);
			std::cout << std::endl;
			bPassed = false;
		}

		std::cout << "\tSums: ";
		if(sums == solver.GetNumOfSets())
			std::cout << "succeeded" << std::endl;
		else {
			std::cout << "!!!! FAILED !!!! expected: " << sums
				<< " actual: " << solver.GetNumOfSets()
				<< std::endl;
			bPassed = false;
		}
	}
	catch(slvExceptionInvlideBoardData_t & e) {
		bPassed = false;
		std::cout << " !!!! FAILED !!!! slvExceptionInvlideBoardData_t caught; reason="
			<< e.GetReason();
	}

	delete [] testBoard.pData;
	return bPassed;
}

static bool testCtor_solverImpl_t()
{
	bool bPassed = true;

	std::cout << "*     test ctor of solverImple_t" << std::endl;

	for(size_t testId = 0; testId < CTestData; testId++) {
		bool bTmp;
		std::cout << "\tTestcase " << (testId + 1) << " ";
		bTmp = testACtor(TestData[testId], TestIndeces[testId], TestSums[testId]);
		std::cout << "\t\t" << (bTmp ? "succeeded" : "!!!! FAILED !!!!") << std::endl << std::endl;
		bPassed &= bTmp;
	}

	return bPassed;
}

// cy < 2
static const unsigned ERR1[] = {
	2, 1,
	1, 0, 0,	1, 0, 4,	1, 0, 7,
	1, 3, 0,	0, 0, 0,	0, 0, 0,
};
// cx < 2
static const unsigned ERR2[] = {
	1, 2,
	1, 0, 0,	1, 0, 4,
	1, 3, 0,	0, 0, 0,
	1, 8, 0,	0, 0, 0,
};
// answer cell in topmost
static const unsigned ERR3[] = {
	2, 2,     /*V  V  V*/
	1, 0, 0,	0, 0, 0,	1, 0, 7,
	1, 3, 0,	0, 0, 0,	0, 0, 0,
	1, 8, 0,	0, 0, 0,	0, 0, 0,
};
// answer cell in leftmost
static const unsigned ERR4[] = {
	2, 2,
	1, 0, 0,	1, 0, 4,	1, 0, 7,
/*  V  V  V */
	0, 0, 0,	0, 0, 0,	0, 0, 0,
	1, 8, 0,	0, 0, 0,	0, 0, 0,
};
// sum is given for sum cell with no cells; right
static const unsigned ERR5[] = {
	5, 3,
	1,0,0,	1,0,0,	1,0,6,	1,0,5,	1,0,9,	1,0,0,
/*	  V */
	1,5,0,	1,7,3,	0,0,0,	0,0,0,	0,0,0,	1,0,8,
	1,15,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,
	1,4,0,	0,0,0,	0,0,0,	1,5,0,	0,0,0,	0,0,0,
};
// sum is given for sum cell with no cells; down
static const unsigned ERR6[] = {
	5, 3,	/*  V */
	1,0,0,	1,0,9,	1,0,6,	1,0,5,	1,0,9,	1,0,0,
	1,0,0,	1,7,3,	0,0,0,	0,0,0,	0,0,0,	1,0,8,
	1,15,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,
	1,4,0,	0,0,0,	0,0,0,	1,5,0,	0,0,0,	0,0,0,
};
// # of cells < 2; right
static const unsigned ERR7[] = {
	5, 3,
	1,0,0,	1,0,0,	1,0,6,	1,0,5,	1,0,9,	1,0,7,
	1,0,0,	1,11,3,	0,0,0,	0,0,0,	0,0,0,	0,0,0,
	1,15,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,
	1,4,0,	0,0,0,	0,0,0,	1,5,0,	0,0,0,	1,0,0,
};
// # of cells < 2; down
static const unsigned ERR8[] = {
	5, 3,
	1,0,0,	1,0,0,	1,0,6,	1,0,7,	1,0,9,	1,0,0,
	1,0,0,	1,7,3,	0,0,0,	0,0,0,	0,0,0,	1,0,8,
	1,15,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,
	1,11,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	1,0,0,
};
// # of cells > 9; right
static const unsigned ERR9[] = {
	10, 2,
	1,0,0,	1,0,3,	1,0,6,	1,0,5,	1,0,4,	1,0,7,	1,0,11,	1,0,12,	1,0,9,	1,0,15,	1,0,17,
	1,45,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,
	1,45,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,	0,0,0,
};
// # of cells > 9; down
static const unsigned ERR10[] = {
	2, 10,
	1,0,0,	1,0,45,	1,0,45,
	1,3,0,	0,0,0,	0,0,0,
	1,4,0,	0,0,0,	0,0,0,
	1,5,0,	0,0,0,	0,0,0,
	1,6,0,	0,0,0,	0,0,0,
	1,7,0,	0,0,0,	0,0,0,
	1,8,0,	0,0,0,	0,0,0,
	1,9,0,	0,0,0,	0,0,0,
	1,10,0,	0,0,0,	0,0,0,
	1,11,0,	0,0,0,	0,0,0,
	1,12,0,	0,0,0,	0,0,0,
};
// impossible sum; right
static const unsigned ERR11[] = {
	2, 2,
	1, 0, 0,	1, 0, 4,	1, 0, 7,
/*	   VV */
	1, 45, 0,	0, 0, 0,	0, 0, 0,
	1, 8, 0,	0, 0, 0,	0, 0, 0,
};
// impossible sum; down
static const unsigned ERR12[] = {
	2, 2,		/*    VV */
	1, 0, 0,	1, 0, 18,	1, 0, 7,
	1, 3, 0,	0, 0, 0,	0, 0, 0,
	1, 8, 0,	0, 0, 0,	0, 0, 0,
};

static const unsigned * TestErr[] = {
	ERR1,
	ERR2,
	ERR3,
	ERR4,
	ERR5,
	ERR6,
	ERR7,
	ERR8,
	ERR9,
	ERR10,
	ERR11,
	ERR12,
	nullptr,
};

typedef slvExceptionInvlideBoardData_t::reason_t r_t;
static const r_t TestReason[] = {
	slvExceptionInvlideBoardData_t::BOARD_SIZE,
	slvExceptionInvlideBoardData_t::BOARD_SIZE,
	slvExceptionInvlideBoardData_t::EDGE_CELL,
	slvExceptionInvlideBoardData_t::EDGE_CELL,
	slvExceptionInvlideBoardData_t::SUM4EMPTY,
	slvExceptionInvlideBoardData_t::SUM4EMPTY,
	slvExceptionInvlideBoardData_t::NUM_CELL,
	slvExceptionInvlideBoardData_t::NUM_CELL,
	slvExceptionInvlideBoardData_t::NUM_CELL,
	slvExceptionInvlideBoardData_t::NUM_CELL,
	slvExceptionInvlideBoardData_t::SUM,
	slvExceptionInvlideBoardData_t::SUM,
	slvExceptionInvlideBoardData_t::NO_BOARD_DATA,
};

static const size_t CTestErr = sizeof(TestErr)/sizeof(TestErr[0]);

static bool testAnIntentional(const unsigned data[], r_t reason)
{
	bool bPassed = false;
	board_t testBoard;

	MakeDataBoard(testBoard, data);

	try {
		std::unique_ptr<solverImpl_t>pSolve(new solverImpl_t(testBoard));
	}
	catch(slvExceptionInvlideBoardData_t & e) {
		if(e.GetReason() == reason)
			bPassed = true;
		else {
			std::cout << std::endl << "\t\tincorrect reason: expected=" << reason
				<< " actual=" << e.GetReason();
		}
	}

	return bPassed;
}

static bool testIntentionalError()
{
	bool bPassed = true;

	std::cout << "*     test ctor of solverImple_t: intentional errors" << std::endl;

	for(size_t testId = 0; testId < CTestErr; testId++) {
		bool bTmp;
		std::cout << "\tTestcase " << (testId + 1) << " ";
		bTmp = testAnIntentional(TestErr[testId], TestReason[testId]);
		std::cout << (bTmp ? "succeeded" : "!!!! FAILED !!!!") << std::endl;
		bPassed &= bTmp;
	}

	return bPassed;
}

bool test4()
{
	bool bPassed = true;
	std::cout << "***** test case 4: solverImpl_t" << std::endl;

	bPassed &= testCtor_solverImpl_t();
	bPassed &= testIntentionalError();

	return bPassed;
}