/***************************************************************************/
/* testSlv3Cell.cpp                                                        */
/* test case 3 for Kakuro Solving Engine: innerCell_t                      */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/17 Initial                                                      */
/***************************************************************************/

#include <iostream>
#include <array>
#include <memory>
#include "..\KakuroSolver_t\kkrsolv_t.h"
#include "..\KakuroSolver_t\innerbrd.h"
#include "testSlv.h"

static bool testCtor()
{
	const size_t testIndex = 3;
	bool bPassed = true;
	kakuro::valueSet_t defaultValue;
	kakuro::innerCell_t aCell;
	aCell.SetIndex(testIndex);

	defaultValue.set();

	std::cout << "*     test ctor" << std::endl;
	std::cout << "\tinnerCell_t (" << testIndex << "): Initial value: ";
	if(aCell == defaultValue)
		std::cout << "succeeded ";
	else {
		bPassed = false;
		std::cout << "!!!! FAILED !!!! ";
	}

	std::cout << "Index: ";
	if(aCell.GetIndex() == testIndex)
		std::cout << "succeeded ";
	else {
		bPassed = false;
		std::cout << "!!!! FAILED !!!! ";
	}
	std::cout << std::endl;

	return bPassed;
}

static bool testDetermined()
{
	bool bPassed = true;
	std::array<std::unique_ptr<kakuro::innerCellSet_t>, 2> pSetArray;
	std::array<std::unique_ptr<kakuro::innerCell_t>, 1> pCellArray;
	kakuro::statusChange_t statusChange;

	std::cout << "*     test determination" << std::endl;
	
	std::cout << "\t{1,2} and {1,3}: ";
	pSetArray[0].reset(new kakuro::innerCellSet_t(1, 1, 2, 3)); // dummy x and y
	pSetArray[1].reset(new kakuro::innerCellSet_t(1, 1, 2, 4)); // dummy x and y
	pCellArray[0].reset(new kakuro::innerCell_t);
	pCellArray[0]->SetIndex(0);

	for(auto & aSet : pSetArray) {
		aSet->AddACell(pCellArray[0].get());
		pCellArray[0]->AddACellSet(aSet.get());
	}
	statusChange = pCellArray[0]->UpdateStatus();
	switch(statusChange) {
	case kakuro::STS_DETERMINED:
		std::cout << " succeeded" << std::endl;
		break;

	default:
		std::cout << " !!!! FAILED !!!! "
			<< (statusChange == kakuro::STS_NONE ? "no change" : (statusChange == kakuro::STS_CHANGED ? "just changed" : "no solution"))
			<< std::endl;
		bPassed = false;
	}

	return bPassed;
}

static bool testChanged()
{
	bool bPassed = true;
	kakuro::innerCellSet_t * pSetArray[2];
	kakuro::innerCell_t * pCellArray[2];
	kakuro::statusChange_t statusChange;
	kakuro::valueSet_t expectedValue(0x180);

	std::cout << "*     test change" << std::endl;
	
	std::cout << "\t{9, 8, 7} and {9, 8, 6}: ";
	pSetArray[0] = new kakuro::innerCellSet_t(1, 1, 3, 24); // dummy x and y
	pSetArray[1] = new kakuro::innerCellSet_t(1, 1, 3, 23); // dummy x and y
	pCellArray[0] = new kakuro::innerCell_t;
	pCellArray[1] = new kakuro::innerCell_t;

	pCellArray[0]->SetIndex(0);
	pCellArray[1]->SetIndex(1);

	for(auto aSet : pSetArray) {
		aSet->AddACell(pCellArray[0]);
		aSet->AddACell(pCellArray[1]);	// never update the status of pCellArray[1]
		aSet->AddACell(pCellArray[1]);	// so reuse
		pCellArray[0]->AddACellSet(aSet);
	}
	statusChange = pCellArray[0]->UpdateStatus();
	switch(statusChange) {
	case kakuro::STS_CHANGED:
		if(*pCellArray[0] == expectedValue)
			std::cout << " succeeded" << std::endl;
		else {
			std::cout << " !!!! FAILED !!!! unexpected value: " << pCellArray[0]->to_string() << std::endl;
			bPassed = false;
		}
		break;

	default:
		std::cout << " !!!! FAILED !!!! "
			<< (statusChange == kakuro::STS_NONE ? "no change" : (statusChange == kakuro::STS_DETERMINED ? "determined" : "no solution"))
			<< std::endl;
		bPassed = false;
	}

	std::cout << "\tCalling UpdateStatus() again: ";
	statusChange = pCellArray[0]->UpdateStatus();
	switch(statusChange) {
	case kakuro::STS_NONE:
		std::cout << " succeeded" << std::endl;
		break;

	default:
		std::cout << " !!!! FAILED !!!! "
			<< (statusChange == kakuro::STS_CHANGED ? "changed" : (statusChange == kakuro::STS_DETERMINED ? "determined" : "no solution"))
			<< std::endl;
		bPassed = false;
	}

	for(auto aSet : pSetArray)
		delete aSet;
	for(auto aCell : pCellArray)
		delete aCell;

	return bPassed;
}

static bool testNoSolution()
{
	bool bPassed = true;
	kakuro::innerCellSet_t * pSetArray[2];
	kakuro::innerCell_t * pCellArray[1];
	kakuro::statusChange_t statusChange;

	std::cout << "*     test no solution" << std::endl;
	
	std::cout << "\t{1,2} and {8,9}: ";
	pSetArray[0] = new kakuro::innerCellSet_t(1, 1, 2, 3); // dummy x and y
	pSetArray[1] = new kakuro::innerCellSet_t(1, 1, 2, 17); // dummy x and y
	pCellArray[0] = new kakuro::innerCell_t;
	pCellArray[0]->SetIndex(0);

	for(auto aSet : pSetArray) {
		aSet->AddACell(pCellArray[0]);
		pCellArray[0]->AddACellSet(aSet);
	}
	statusChange = pCellArray[0]->UpdateStatus();
	switch(statusChange) {
	case kakuro::STS_NO_SOLUTION:
		std::cout << " succeeded" << std::endl;
		break;

	default:
		std::cout << " !!!! FAILED !!!! "
			<< (statusChange == kakuro::STS_NONE ? "no change" : (statusChange == kakuro::STS_CHANGED ? "just changed" : "determined"))
			<< std::endl;
		bPassed = false;
	}

	for(auto aSet : pSetArray)
		delete aSet;
	for(auto aCell : pCellArray)
		delete aCell;


	return bPassed;
}

bool test3()
{
	bool bPassed;

	std::cout << "***** test case 3: innerCell_t" << std::endl;

	bPassed = testCtor();
	bPassed &= testDetermined();
	bPassed &= testChanged();
	bPassed &= testNoSolution();

	return bPassed;
}
