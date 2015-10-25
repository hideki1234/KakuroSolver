/***************************************************************************/
/* kkrsolv_t.h                                                             */
/* Kakuro solving engine data definitions                                  */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/12 Initial                                                      */
/***************************************************************************/

#include <memory>

namespace kakuro {
	/*********************************************
	** constant values
	*********************************************/
	const size_t MAX_CELL_VALUE = 9;

	/*********************************************
	** Exception definitions which solver_t throws
	*********************************************/

	/*
	** base class for all solver_t exceptions
	*/
	class slvException_t {
		int m_type;
	protected:
		slvException_t(int type) : m_type(type) {}
	public:
		int GetType() const {return m_type;}
	};

	// board data is invalid
	class slvExceptionInvlideBoardData_t : public slvException_t {
	public:
		enum reason_t {
			EDGE_CELL = 1,	// topmost/leftmost has non-sum cell(s)
			SUM4EMPTY,		// sum is given for consecutive sums
			NUM_CELL,		// # of cells in a sum is < 2 or > 9
			SUM,			// impossible sum (ex. 45 for 2-cell sum)
			BOARD_SIZE,		// board is smaller than 2x2
			NO_BOARD_DATA,	// no board data is given
		};
	
	private:
		size_t m_x, m_y;	// position including leftmost/topmost sum cells
		reason_t m_reason;

	public:
		slvExceptionInvlideBoardData_t(reason_t r)
			: slvException_t(1), m_reason(r), m_x(0), m_y(0) {}
		slvExceptionInvlideBoardData_t(reason_t r, size_t x, size_t y)
			: slvException_t(1), m_reason(r), m_x(x), m_y(y) {}
		reason_t GetReason() const {return m_reason;}
		void GetPosition(size_t * x, size_t * y) const {*x = m_x; *y = m_y;}
	};

	// argument passed to method is invalid
	class slvExceptionInvalidArgument_t : public slvException_t {
	public:
		slvExceptionInvalidArgument_t() : slvException_t(2) {}
	};

	// haven't solved yet; no solution ready
	class slvExceptionNotSolvedYet_t : public slvException_t {
	public:
		slvExceptionNotSolvedYet_t() : slvException_t(3) {}
	};

	// probably a bug in solving engine
	class slvEceptionSolverBug_t : public slvException_t {
	private:
		const size_t m_line;
		const char * const m_file;
	public:
		slvEceptionSolverBug_t(size_t line, const char * file)
			: slvException_t(4), m_line(line), m_file(file) {}
		size_t GetLine() const {return m_line;}
		const char * GetFileName() const {return m_file;}
	};

	/************************************
	** data definitions for event handler
	************************************/

	// event types solver_t fires
	typedef unsigned int slvEvent_t;
	const slvEvent_t EVENT_SOLUTION = 0;			// a solution found
	const slvEvent_t EVENT_DETERMINED_UPDATE = 1;	// a cell value determined
	const slvEvent_t EVENT_TEMPORARY_UPDATE = 2;	// a cell value temporarily put in brue force
	const slvEvent_t EVENT_RESET = 3;				// temporary values are reset

	// event types an event handler requests
	typedef enum {
		REQEVENT_NONE,			// no callback
		REQEVENT_SOLUTION,		/* when a solution found, EVENT_SOLUTION fired
								*/
		REQEVENT_DETERMINED,	/* REQEVENT_SOLUTION +
								** when a cell value determined,
								** EVENT_DETERMINED_UPDATE fired
								*/
		REQEVENT_TEMPORARY,		/* CBEVENT_DETERMINED +
								** EVENT_TEMPORARY_UPDATE and EVENT_RESET
								*/
	}	slvEventRequest_t;

	/*
	** event handler prototype
	*/
	typedef void (*eventHandler_t)(slvEvent_t kkrEvent, void * user, void * data);

	/*
	** data which event handler returns
	*/
	// EVENT_DETERMINED_UPDATE and EVENT_TEMPORARY_UPDATE
	typedef struct {
		size_t x;
		size_t y;
		unsigned char value;
	} slvEventDataUpdate_t;

	// EVENT_SOLUTION and EVENT_RESET
	typedef unsigned char * slvEventDataBoard_t;
	// unsigned char[cx * cy]
	// sequence of (x,y)
	// (0,0) (1,0) ... (cx-1,0) (0,1) ... (cx-1,1) ... (cx-1,cy-1)
	// a sum cell is set to 0.
	// for EVENT_RESET, an undetermined cell is set to 0

	/*************************************
	** definition of kakuro solver engine
	*************************************/

	class solver_t {
	protected:
		solver_t();

	public:
		virtual ~solver_t();

		virtual void SetEventHandler(slvEventRequest_t req, eventHandler_t eh, void * pUser) = 0;
		virtual void Solve() = 0;
		virtual size_t GetSolutionCount() = 0;
		virtual void GetSolution(size_t idx, const unsigned char ** ppData) = 0;
	};


	/*******************************
	** definitions of solver factory
	*******************************/

	typedef struct {
		bool isSum;
		unsigned char sumRight;
		unsigned char sumDown;
	} cell_t;

	typedef struct {
		size_t cx;	// excludes leftmost sum
		size_t cy;	// excludes topmost sum
		cell_t * pData;	// (cx + 1) * (cy * 1)
	} board_t;

	solver_t * SolverFactory(const board_t & boardData);

	/********************************
	** kakuro solving engine version
	********************************/

	void GetVersionNumber(int * major, int * minor, int * release);

}	// namespace kakuro
