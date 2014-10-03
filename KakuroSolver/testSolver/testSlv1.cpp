/***************************************************************************/
/* testSlv1.cpp                                                            */
/* test case 1 for Kakuro Solving Engine                                   */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/13 Initial                                                      */
/***************************************************************************/

#include <iostream>
#include <memory>
#include "..\KakuroSolver_t\kkrsolv_t.h"
#include "testSlv.h"

bool test1()
{
	std::cout << "***** test case 1" << std::endl;
	kakuro::board_t boardData;
	kakuro::cell_t dummyCell;
	bool bSucceeded;

	std::cout << "* Invalid test data to SolverFactory" << std::endl;

	std::cout << "pData = nullptr;";
	boardData.cx = boardData.cy = 2;
	boardData.pData = nullptr;
	bSucceeded = false;
	try {
        std::unique_ptr<kakuro::solver_t>aSolver(kakuro::SolverFactory(boardData));
	}
	catch(kakuro::slvExceptionInvlideBoardData_t & eInvalidArg) {
		if(eInvalidArg.GetReason() != kakuro::slvExceptionInvlideBoardData_t::NO_BOARD_DATA)
			std::cout << " !!!! WARNING !!!! unexpected exception reason";
		bSucceeded = true;
	}
	std::cout << (bSucceeded ? " correctly an exception thrown" : " !!!! FAILED !!!! no exception")
		<< std::endl;

	std::cout << "cx < 2;";
	boardData.cx = 1;
	boardData.cy = 2;
	boardData.pData = &dummyCell;
	bSucceeded = false;
	try {
        std::unique_ptr<kakuro::solver_t>aSolver(kakuro::SolverFactory(boardData));
	}
	catch(kakuro::slvExceptionInvlideBoardData_t & eInvalidArg) {
		if(eInvalidArg.GetReason() != kakuro::slvExceptionInvlideBoardData_t::BOARD_SIZE)
			std::cout << " !!!! WARNING !!!! unexpected exception reason";
		bSucceeded = true;
	}
	std::cout << (bSucceeded ? " correctly an exception thrown" : " !!!! FAILED !!!! no exception")
		<< std::endl;

	std::cout << "cy < 2;";
	boardData.cx = 2;
	boardData.cy = 0;
	boardData.pData = &dummyCell;
	bSucceeded = false;
	try {
        std::unique_ptr<kakuro::solver_t>aSolver(kakuro::SolverFactory(boardData));
	}
	catch(kakuro::slvExceptionInvlideBoardData_t & eInvalidArg) {
		if(eInvalidArg.GetReason() != kakuro::slvExceptionInvlideBoardData_t::BOARD_SIZE)
			std::cout << " !!!! WARNING !!!! unexpected exception reason";
		bSucceeded = true;
	}
	std::cout << (bSucceeded ? " correctly an exception thrown" : " !!!! FAILED !!!! no exception")
		<< std::endl;

	return true;
}
