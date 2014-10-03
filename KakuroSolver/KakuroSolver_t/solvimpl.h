/***************************************************************************/
/* solvimpl.h                                                              */
/* Kakuro solving engine data definitions                                  */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/13 Initial                                                      */
/***************************************************************************/

#include "innerbrd.h"

namespace kakuro {
	//
	// kakuro solver implementation class
	//
	class solverImpl_t : public solver_t {
	protected:
		const size_t	m_cx;
		const size_t	m_cy;

        std::unique_ptr<innerBoard_t> m_initialBoard;

		// solution
		bool m_bSolved;	// Solve() ever called?
		std::vector<unsigned char *> m_solutions;

		// event handler
		slvEventRequest_t m_eReq;
		eventHandler_t m_eh;
		void * m_eUser;			// user data
		slvEventDataUpdate_t m_eventDataCell;
		slvEventDataBoard_t m_eventDataBoard;

	public:
		explicit solverImpl_t(const board_t & boardData);
		~solverImpl_t();

		void SetEventHandler(slvEventRequest_t req, eventHandler_t eh, void * pUser) override;
		void Solve() override;
		size_t GetSolutionCount() override;
		void GetSolution(size_t idx, const unsigned char ** ppData) override;

	private:
		void GetCoordFromIndex(const size_t index, size_t *px, size_t *py) const {
			*px = index % m_cx;
			*py = index / m_cx;
		}
		void Solve(innerBoard_t * pBoard);	// called by Solve(void) intenally
		unsigned char * MakeTransferableDataFromBoard(const innerBoard_t * pBoard);
		void SendEventCellUpdate(const innerCell_t * pCell, bool bDetermined);
		void SendEventBoardUpdate(const innerBoard_t * pBoard, bool bSolved);
	};
}
