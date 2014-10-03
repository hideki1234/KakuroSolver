/***************************************************************************/
/* testSlv2CellSet.cpp                                                     */
/* test case 2 for Kakuro Solving Engine: innerCellSet_t                   */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/16 Initial                                                      */
/***************************************************************************/

#include <iostream>
#include <array>
#include <memory>
#include "..\KakuroSolver_t\kkrsolv_t.h"
#include "..\KakuroSolver_t\innerbrd.h"
#include "testSlv.h"

struct cCellSum_t {
	size_t cCell;
	size_t sum;
	unsigned long bits;
};

static bool testACaseCtorException(const cCellSum_t & data)
{
	bool bPassed = false;
	const size_t xExpected = 1, yExpected = 2;

	std::cout << "\tcells: " << data.cCell << ", sum: " << data.sum;
	try {
        std::unique_ptr<kakuro::innerCellSet_t> aCellSet(new kakuro::innerCellSet_t(xExpected, yExpected, data.cCell, data.sum));
	}
	catch(kakuro::slvExceptionInvlideBoardData_t & e) {
		size_t x, y;
        const kakuro::slvExceptionInvlideBoardData_t::reason_t reasonExpected =
            (data.cCell < 2 || data.cCell > 9) ? kakuro::slvExceptionInvlideBoardData_t::NUM_CELL : kakuro::slvExceptionInvlideBoardData_t::SUM;
		if(e.GetReason() != reasonExpected)
			std::cout << " !!!! WARNING !!!! unexpected exception reason";
		e.GetPosition(&x, &y);
		if(x != xExpected || y != yExpected)
			std::cout << " !!!! WARNING !!!! unexpected cell position(" << x << "," << y << ")";

		bPassed = true;
	}

	std::cout << (bPassed ? " - Succeeded" : " - !!!! FAILED !!!!") << std::endl;

	return bPassed;
}

static bool testCtorException()
{
	bool bPassed = true;
	cCellSum_t data[] = {
		{1, 1, 0},
		{2, 2, 0}, {2, 18, 0},
		{3, 5, 0}, {3, 25, 0},
		{4, 9, 0}, {4, 31, 0},
		{5, 14, 0}, {5, 36, 0},
		{6, 20, 0}, {6, 40, 0},
		{7, 27, 0}, {7, 43, 0},
		{8, 35, 0}, {8, 45, 0},
		{9, 44, 0}, {9, 46, 0},
		{10, 55, 0}
	};

	std::cout << "*     invalid argument for ctor" << std::endl;
	for(auto & item : data)
		bPassed &= testACaseCtorException(item);

	return bPassed;
}

static bool testACaseCtor(const cCellSum_t & data)
{
	bool bPassed;
	const size_t xExpected = 10, yExpected = 8;

	std::cout << "\tcells: " << data.cCell << ", sum: " << data.sum;
	try {
		std::unique_ptr<kakuro::innerCellSet_t> aCellSet(new kakuro::innerCellSet_t(xExpected, yExpected, data.cCell, data.sum)); // dummy x and y
		std::cout << " - Expected: " << data.bits << ", Actual: " << aCellSet->to_ulong();
		bPassed = (data.bits == aCellSet->to_ulong());
	}
	catch(kakuro::slvExceptionInvlideBoardData_t & e) {
		bPassed = false;
		std::cout << " - !!!! Invalid board data exception !!!!; reason="
			<< e.GetReason();;
	}

	std::cout << (bPassed ? " - Succeeded" : " - !!!! FAILED !!!!") << std::endl;

	return bPassed;
}

static bool testCtor()
{
	bool bPassed = true;
	cCellSum_t data[] = {
		{2, 3, 0x3}, {2, 6, 0x1b}, {2, 17, 0x180},
		{3, 6, 0x7}, {3, 10, 0x7f}, {3, 24, 0x1c0},
		{4, 10, 0xf}, {4, 20, 0x1ff}, {4, 30, 0x1e0},
		{5, 15, 0x1f}, {5, 18, 0xff}, {5, 35, 0x1f0},
		{6, 21, 0x3f}, {6, 36, 0x1ff}, {6, 39, 0x1f8},
		{7, 28, 0x7f}, {7, 42, 0x1fc},
		{8, 36, 0xff}, {8, 44, 0x1fe},
		{9, 45, 0x1ff},
	};

	std::cout << "*     test ctor" << std::endl;
	for(auto & item : data)
		bPassed &= testACaseCtor(item);

	return bPassed;
}

static bool testForNew()
{
	bool bPassed = true;

	bPassed &= testCtorException();
	bPassed &= testCtor();

	return bPassed;
}

static bool testCalc()
{
	kakuro::valueSet_t expected;
    std::unique_ptr<kakuro::innerCellSet_t> pTestSet, pDummySet;
    std::array<std::unique_ptr<kakuro::innerCell_t>, 3> pCellArray;
	bool bPassed = true;

	/*
	**
	*/
	std::cout << "*     initial value";
	// 3, 10 {1,2,7} {1,3,6} {1,4,5} {2,3,5}
	pTestSet.reset(new kakuro::innerCellSet_t(1, 1, 3, 10)); // dummy x and y
	expected.reset().set(1-1).set(2-1).set(3-1).set(4-1).set(5-1).set(6-1).set(7-1);
	if(expected == *pTestSet)
		std::cout << " succeeded";
	else {
		std::cout << " !!!! FAILED !!!!; expected: " << expected.to_string()
			<< ", actual: " << pTestSet->to_string();
	}
	std::cout << std::endl;

	/*
	**
	*/
	std::cout << "*     one determined";
	// 3, 10 {1,2,7} {1,3,6} {1,4,5} {2,3,5}
	pTestSet.reset(new kakuro::innerCellSet_t(1, 1, 3, 10)); // dummy x and y
	pDummySet.reset(new kakuro::innerCellSet_t(1, 1, 9, 45)); // dummy x and y
	for(size_t i = 0; i < pCellArray.size(); i++) {
		pCellArray[i].reset(new kakuro::innerCell_t);
		pCellArray[i]->SetIndex(i);
	}

	for(auto & aCell : pCellArray) {
		pTestSet->AddACell(aCell.get());
		aCell->AddACellSet(pTestSet.get());

		pDummySet->AddACell(aCell.get());
		aCell->AddACellSet(pDummySet.get());
	}

	pCellArray[0]->reset().set(5-1).set(9-1);
	// 3, 10 with 5 set {1,4,5} {2,3,5}
	// don't set 5 because it is a determined value
	expected.reset().set(1-1).set(2-1).set(3-1).set(4-1);
	pCellArray[0]->UpdateStatus();

	if(expected == *pTestSet)
		std::cout << " succeeded";
	else {
		std::cout << " !!!! FAILED !!!!; expected: " << expected.to_string()
			<< ", actual: " << pTestSet->to_string();
	}
	std::cout << std::endl;

	/*
	**
	*/
	std::cout << "*     no 5";
	// 3, 10 {1,2,7} {1,3,6} {1,4,5} {2,3,5}
	pTestSet.reset(new kakuro::innerCellSet_t(1, 1, 3, 10)); // dummy x and y
	pDummySet.reset(new kakuro::innerCellSet_t(1, 1, 9, 45)); // dummy x and y
	for(size_t i = 0; i < pCellArray.size(); i++) {
		pCellArray[i].reset(new kakuro::innerCell_t);
		pCellArray[i]->SetIndex(i);
	}

	for(auto & aCell : pCellArray) {
		pTestSet->AddACell(aCell.get());
		aCell->AddACellSet(pTestSet.get());

		pDummySet->AddACell(aCell.get());
		aCell->AddACellSet(pDummySet.get());

		aCell->reset(5-1);
	}

	// 3, 10 with no 5 {1,2,7} {1,3,6}
	expected.reset().set(1-1).set(2-1).set(3-1).set(6-1).set(7-1);
	pCellArray[0]->UpdateStatus();

	if(expected == *pTestSet)
		std::cout << " succeeded";
	else {
		std::cout << " !!!! FAILED !!!!; expected: " << expected.to_string()
			<< ", actual: " << pTestSet->to_string();
	}
	std::cout << std::endl;

	return bPassed;
}

bool test2()
{
	bool bPassed = true;

	std::cout << "***** test case 2: innerCellSet_t" << std::endl;

	bPassed &= testForNew();
	bPassed &= testCalc();

	return bPassed;
}
