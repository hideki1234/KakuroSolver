/***************************************************************************/
/* testSlvMain.cpp                                                         */
/* unit test for Kakuro Solving Engine                                     */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/13 Initial                                                      */
/***************************************************************************/

#include <iostream>
#include <list>
#include <utility>
#include "..\KakuroSolver_t\kkrsolv_t.h"
#include "testSlv.h"

using namespace kakuro;

int main()
{
	std::list<std::pair<const char *, bool>> results;

	results.push_back(std::make_pair("test1", test1()));
	std::cout << std::endl;
	results.push_back(std::make_pair("test2", test2()));
	std::cout << std::endl;
	results.push_back(std::make_pair("test3", test3()));
	std::cout << std::endl;
	results.push_back(std::make_pair("test4", test4()));
	std::cout << std::endl;
	results.push_back(std::make_pair("test5", test5()));
	std::cout << std::endl;
	results.push_back(std::make_pair("test6", test6()));
	std::cout << std::endl;
	results.push_back(std::make_pair("test7", test7()));

	std::cout << std::endl
		<< "******** SUMMARY ********" << std::endl;
	for(auto aResult : results) {
		std::cout << "* " << aResult.first
			<< (aResult.second ? " succeeded" : " !!!! FAILED !!!!")
			<< std::endl;
	}
	return 0;
}

// utilities
int CountSol;
int CountDet;
int CountTmp;
int CountReset;
bool bVerbose;
size_t Size;
unsigned char * Expected;

void ResetCounts()
{
	CountSol = CountDet = CountTmp = CountReset = 0;
	Expected = nullptr;
}

void testHandler(slvEvent_t ev, void * user, void * data)
{
	switch(ev) {
	case EVENT_SOLUTION:
		CountSol++;
		if(bVerbose) {
			const slvEventDataBoard_t pSolData = reinterpret_cast<slvEventDataBoard_t>(data);
			std::cout << "\t\tEVENT_SOLUTION: ";
			for(size_t i = 0; i < Size; i++)
				std::cout << static_cast<unsigned>(pSolData[i]) << ", ";
			std::cout << std::endl << "\t\t";
		}
		break;
	case EVENT_DETERMINED_UPDATE:
		CountDet++;
		if(bVerbose) {
			const slvEventDataUpdate_t * pSolData = reinterpret_cast<slvEventDataUpdate_t*>(data);
			std::cout <<"\t\tEVENT_DETERMINED_UPDATE: x=" << pSolData->x
				<< ", y=" << pSolData->y << ", value=" << static_cast<unsigned>(pSolData->value)
				<< std::endl << "\t\t";
		}
		break;
	case EVENT_TEMPORARY_UPDATE:
		CountTmp++;
		if(bVerbose) {
			const slvEventDataUpdate_t * pSolData = reinterpret_cast<slvEventDataUpdate_t*>(data);
			std::cout <<"\t\tEVENT_TEMPORARY_UPDATE: x=" << pSolData->x
				<< ", y=" << pSolData->y << ", value=" << static_cast<unsigned>(pSolData->value)
				<< std::endl << "\t\t";
		}
		break;
	case EVENT_RESET:
		CountReset++;
		if(bVerbose) {
			const slvEventDataBoard_t pSolData = reinterpret_cast<slvEventDataBoard_t>(data);
			std::cout << "\t\tEVENT_RESET: ";
			for(size_t i = 0; i < Size; i++)
				std::cout << static_cast<unsigned>(pSolData[i]) << ", ";
			std::cout << std::endl << "\t\t";
		}
		break;
	default:
		std::cout << "!!!! Unknown event: " << ev;
	}
}

void MakeDataBoard(kakuro::board_t & testBoard, const unsigned * const data)
{
	size_t c;

	if(data == nullptr) {
		testBoard.cx = testBoard.cy = 2;
		testBoard.pData = nullptr;
		return;
	}

	testBoard.cx = data[0];
	testBoard.cy = data[1];
	c = (testBoard.cx + 1) * (testBoard.cy + 1);
	testBoard.pData = new kakuro::cell_t[c];

	const unsigned * val = data + 2;
	for(size_t i = 0; i < c; i++) {
		testBoard.pData[i].isSum = (*val++ != 0);
		testBoard.pData[i].sumRight = *val++;
		testBoard.pData[i].sumDown = *val++;
	}
}
