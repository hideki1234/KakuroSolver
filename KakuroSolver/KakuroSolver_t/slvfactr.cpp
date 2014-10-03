/***************************************************************************/
/* slvfactr.cpp                                                            */
/* implementation of SolverFactory                                         */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/13 Initial                                                      */
/***************************************************************************/

#include <memory>
#include "kkrsolv_t.h"
#include "solvimpl.h"

namespace kakuro {

	solver_t * SolverFactory(const board_t & boardData)
	{
		solver_t * pNewSolver;
		pNewSolver = new solverImpl_t(boardData);

		return pNewSolver;
	}
}	// namespace kakuro {
