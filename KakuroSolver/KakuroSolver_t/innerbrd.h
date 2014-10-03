/***************************************************************************/
/* innerbrd.h                                                              */
/* definitions for inner kakuro board                                      */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/15 Initial                                                      */
/***************************************************************************/

#include <bitset>
#include <vector>
#include <list>
#include <set>

namespace kakuro {
	const unsigned long long FULLCANDIDATE = 0x1FF;

	typedef std::bitset<9> valueSet_t;
	typedef std::list<const valueSet_t *> candList_t;

	class innerCellSet_t;	// define later
	struct innerBoard_t;	// define later

	/*
	**
	*/
	enum statusChange_t {
		STS_NONE,			// inner status hasn't changed
		STS_CHANGED,		// inner status has change; not determined yet
		STS_DETERMINED,		// cell's value uniquely determined
		STS_NO_SOLUTION,	// no possible solution
	};

	/*
	** innerCell_t definition
	** innerCell_t holds numbers which the undetermined cells can take.
	** This means numbers in the determined cells are excluded.
	*/
	class innerCell_t : public valueSet_t {
	private:
		size_t	m_idx;
		innerCellSet_t * m_sets[2];


	public:
		explicit innerCell_t() : m_idx(0), valueSet_t(FULLCANDIDATE) {
			m_sets[0] = m_sets[1] = nullptr;
		}
		~innerCell_t();

		// for initialization
		void AddACellSet(innerCellSet_t * newSet);

		void SetIndex(size_t idx) {m_idx = idx;}
		size_t GetIndex() const {return m_idx;}
		statusChange_t UpdateStatus();
	};

	/*
	** innerCellSet_t definition
	*/
	class innerCellSet_t : public valueSet_t {
	private:
		std::set<innerCell_t *> m_undetCells;	// undetermined cells
		valueSet_t	m_detValues;				// determined values
		candList_t	m_cands;					// set of candidates

        innerCellSet_t(const innerCellSet_t & org); // for MakeSelfCopyForNewBoard()

	public:
		innerCellSet_t(size_t x, size_t y, size_t numOfCells, size_t sum);
		~innerCellSet_t();

		// methods for initialization
		void AddACell(innerCell_t * newCell) { m_undetCells.insert(newCell); }

		// methods for innerCell_t; innerCell_t calls this when its status
		// is updated
		void UpdateStatus(innerCell_t * pCell);
		void ApplyRule();
		void ApplyRule2();

		innerCellSet_t * MakeSelfCopyForNewBoard(innerBoard_t * newBoard);
	};

	/*
	** innerBoard_t definition
	*/
	struct innerBoard_t {
		size_t cells;
		std::vector<innerCellSet_t *> cellSetSet;
		std::list<innerCell_t *> undetCellSet;	// pointers to a pCells item
		std::vector<innerCell_t> pCells;

		innerBoard_t(size_t cx, size_t cy);
		innerBoard_t(const innerBoard_t & base);
		~innerBoard_t();
	};
}	// namespace kakuro {

