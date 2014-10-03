/***************************************************************************/
/* solvimpl.cpp                                                            */
/* Kakuro solving engine implementation                                    */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/13 Initial                                                      */
/***************************************************************************/

#include <memory>
#include "kkrsolv_t.h"
#include "solvimpl.h"

namespace kakuro {

	static unsigned char BitToValue(unsigned long bit) {
		unsigned char value;
		switch(bit) {
			case 0x001:	value = 1;	break;
			case 0x002:	value = 2;	break;
			case 0x004:	value = 3;	break;
			case 0x008:	value = 4;	break;
			case 0x010:	value = 5;	break;
			case 0x020:	value = 6;	break;
			case 0x040:	value = 7;	break;
			case 0x080:	value = 8;	break;
			case 0x100:	value = 9;	break;
			default:
				throw slvEceptionSolverBug_t(__LINE__, __FILE__);
		}
		return value;
	}

	solverImpl_t::solverImpl_t(const board_t & boardData)
		: m_cx(boardData.cx), m_cy(boardData.cy),
		m_bSolved(false), m_eReq(REQEVENT_NONE), m_eh(nullptr), m_eventDataBoard(nullptr)
	{
		if(m_cx < 2 || m_cy < 2)
			throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::BOARD_SIZE);
		if(boardData.pData == nullptr)
			throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::NO_BOARD_DATA);

		std::unique_ptr<innerBoard_t> newBoard(new innerBoard_t(m_cx, m_cy));

		for(size_t idx = 0; idx < (m_cx + 1) * (m_cy + 1); idx++) {
			const size_t x = idx % (m_cx + 1);
			const size_t y = idx / (m_cx + 1);
			const cell_t * const currentCell = boardData.pData + idx;

			if(currentCell->isSum) {
				size_t cCell;	// number of cells in a sum
				size_t ofs;
				innerCellSet_t * newCellSet;
				size_t innerIdx;

				// look right
				for(cCell = 0; x + cCell < m_cx; cCell++)
					if((boardData.pData + idx + cCell + 1)->isSum)
						break;
				if(y == 0 && cCell != 0)
					throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::EDGE_CELL, x, y);
				if(cCell == 1 || cCell > 9)
					throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::NUM_CELL, x, y);
				if(cCell == 0 && currentCell->sumRight != 0)
					throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::SUM4EMPTY, x, y);
				if(cCell != 0) {
					newCellSet = new innerCellSet_t(x, y, cCell, currentCell->sumRight);
					newBoard->cellSetSet.push_back(newCellSet);
					innerIdx = (y - 1) * m_cx + x;
					for(ofs = 0; ofs < cCell; ofs++) {
						innerCell_t * pCell = &newBoard->pCells[innerIdx + ofs];
						newCellSet->AddACell(pCell);
						pCell->AddACellSet(newCellSet);
					}
				}

				// look down
				for(cCell = 0; y + cCell < m_cy; cCell++)
					if((boardData.pData + idx + (cCell + 1) * (m_cx + 1))->isSum)
						break;
				if(x == 0 && cCell != 0)
					throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::EDGE_CELL, x, y);
				if(cCell == 1 || cCell > 9)
					throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::NUM_CELL, x, y);
				if(cCell == 0 && currentCell->sumDown != 0)
					throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::SUM4EMPTY, x, y);
				if(cCell != 0) {
					newCellSet = new innerCellSet_t(x, y, cCell, currentCell->sumDown);
					newBoard->cellSetSet.push_back(newCellSet);
					innerIdx = y * m_cx + (x - 1);
					for(ofs = 0; ofs < cCell; ofs++) {
						innerCell_t * pCell = &newBoard->pCells[innerIdx + ofs * m_cx];
						newBoard->undetCellSet.push_back(pCell);
						newCellSet->AddACell(pCell);
						pCell->AddACellSet(newCellSet);
					}
				}
			} else {
				// current cell is not a sum
				// if a topmost/leftmost cell is not a sum cell, invalid
				if(x < 1 || y < 1)
					throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::EDGE_CELL, x, y);
			}
		}

		m_initialBoard = std::move(newBoard);
	}

	solverImpl_t::~solverImpl_t()
	{
		delete [] m_eventDataBoard;
		for(auto solution : m_solutions)
			delete [] solution;
	}

	void solverImpl_t::SetEventHandler(slvEventRequest_t req, eventHandler_t eh, void * pUser)
	{
		if(req != REQEVENT_NONE && eh == nullptr)
			throw slvExceptionInvalidArgument_t();

		m_eReq = req;
		if(m_eReq == REQEVENT_NONE) {
			m_eh = nullptr;
			m_eUser = nullptr;
		} else {
			m_eh = eh;
			m_eUser = pUser;
		}
	}

	void solverImpl_t::Solve()
	{
		Solve(m_initialBoard.get());
		m_bSolved = true;
	}

	size_t solverImpl_t::GetSolutionCount()
	{
		if(!m_bSolved)
			throw slvExceptionNotSolvedYet_t();
		return m_solutions.size();
	}

	void solverImpl_t::GetSolution(size_t idx, const unsigned char ** ppData)
	{
		if(!m_bSolved)
			throw slvExceptionNotSolvedYet_t();
		if(ppData == nullptr || idx >= m_solutions.size())
			throw slvExceptionInvalidArgument_t();
		*ppData = m_solutions[idx];
	}

	/*
	** private mathods
	*/
	void solverImpl_t::Solve(innerBoard_t * const pBoard)
	{
		bool changed;
		do {
			changed = false;
			for(auto ppCell = pBoard->undetCellSet.begin(); ppCell != pBoard->undetCellSet.end(); ) {
				if((*ppCell)->UpdateStatus() != STS_NONE) {
					changed = true;
					switch((*ppCell)->count()) {
					case 0:	// no solution
						return;

					case 1:	// determined
						SendEventCellUpdate(*ppCell, (m_initialBoard.get() == pBoard));
						ppCell = pBoard->undetCellSet.erase(ppCell);
						continue;
					}
				}
				ppCell++;
			}
			if(!changed) {
				for(auto pCellSet : pBoard->cellSetSet)
					pCellSet->ApplyRule();
				for(auto ppCell = pBoard->undetCellSet.begin(); ppCell != pBoard->undetCellSet.end(); ) {
					if((*ppCell)->UpdateStatus() != STS_NONE) {
						changed = true;
						switch((*ppCell)->count()) {
						case 0:	// no solution
							return;

						case 1:	// determined
							SendEventCellUpdate(*ppCell, (m_initialBoard.get() == pBoard));
							ppCell = pBoard->undetCellSet.erase(ppCell);
							continue;
						}
					}
					ppCell++;
				}
			}
			if(!changed) {
				for(auto pCellSet : pBoard->cellSetSet)
					pCellSet->ApplyRule2();
				for(auto ppCell = pBoard->undetCellSet.begin(); ppCell != pBoard->undetCellSet.end(); ) {
					if((*ppCell)->UpdateStatus() != STS_NONE) {
						changed = true;
						switch((*ppCell)->count()) {
						case 0:	// no solution
							return;

						case 1:	// determined
							SendEventCellUpdate(*ppCell, (m_initialBoard.get() == pBoard));
							ppCell = pBoard->undetCellSet.erase(ppCell);
							continue;
						}
					}
					ppCell++;
				}
			}
		} while(changed);

		if(pBoard->undetCellSet.empty()) {
			// a solution found
			m_solutions.push_back(MakeTransferableDataFromBoard(pBoard));
			SendEventBoardUpdate(pBoard, true);
		} else {
			// brute force!
			// pick up the first cell in undetermined cells
			const innerCell_t * pCell = pBoard->undetCellSet.front();
			size_t idx = pCell->GetIndex();

			for(size_t bitPos = 0; bitPos < MAX_CELL_VALUE; bitPos++) {
				if(!pCell->test(bitPos))
					continue;
				innerBoard_t * pNewBoard = new innerBoard_t(*pBoard);
				pNewBoard->pCells[idx].reset();
				pNewBoard->pCells[idx].set(bitPos);
				Solve(pNewBoard);
				delete pNewBoard;

				// send the 'reset the board' event
				SendEventBoardUpdate(pBoard, false);
			}
		}
	}

	unsigned char * solverImpl_t::MakeTransferableDataFromBoard(const innerBoard_t * pBoard)
	{
		unsigned char * pTransferable = new unsigned char[pBoard->cells];
		for(size_t i = 0; i < pBoard->cells; i++) {
			const innerCell_t & thisCell = pBoard->pCells[i];
			pTransferable[i /* == thisCell.GetIndex() */] =
				(thisCell.count() == 1 ? BitToValue(thisCell.to_ulong()) : 0);
		}
		return pTransferable;
	}

	void solverImpl_t::SendEventCellUpdate(const innerCell_t * pCell, bool bDetermined)
	{
		if(m_eh == nullptr)
			return;
		if(m_eReq < REQEVENT_DETERMINED || !bDetermined && m_eReq < REQEVENT_TEMPORARY)
			return;

		GetCoordFromIndex(pCell->GetIndex(), &m_eventDataCell.x, &m_eventDataCell.y);
		m_eventDataCell.value = BitToValue(pCell->to_ulong());
		m_eh((bDetermined ? EVENT_DETERMINED_UPDATE : EVENT_TEMPORARY_UPDATE), m_eUser, &m_eventDataCell);
	}

	void solverImpl_t::SendEventBoardUpdate(const innerBoard_t * pBoard, bool bSolved)
	{
		if(m_eh == nullptr)
			return;
		if(m_eReq < REQEVENT_SOLUTION || !bSolved && m_eReq < REQEVENT_TEMPORARY)
			return;

		delete [] m_eventDataBoard;
		m_eventDataBoard = MakeTransferableDataFromBoard(pBoard);
		m_eh((bSolved ? EVENT_SOLUTION : EVENT_RESET), m_eUser, m_eventDataBoard);
	}
}	// namespace kakuro {
