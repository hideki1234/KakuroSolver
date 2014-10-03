/***************************************************************************/
/* kkrtypes.h                                                              */
/* Kakuro solver data definitions                                          */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/09 Initial                                                      */
/***************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

	//
// board data defninitions
//

// +------------------+
// | board header     |
// |   version (== 1) |
// |   cx             |
// |   cy             |
// +------------------+  -+
// |                  |   |
// | cell information |   |
// |                  |   |
// +------------------+   |
// |                  |   |
//         ...            | (cx + 1) * (cy + 1)
// |                  |   |
// +------------------+   |
// |                  |   |
// | cell information |   |
// |                  |   |
// +------------------+  -+

// cell information
//
// +---------------------------+
// | sum cell info             |
// |   sum marker (== -1)      |
// |   sum right  (0, or 3~45) |
// |   sum down   (0, or 3~45) |
// +---------------------------+
//
//           or
//
// +-----------------------------+
// | answer cell info            |
// |   answer cell marker (== 0) |
// +-----------------------------+

typedef struct {
	unsigned int	version;	// must be 1
	unsigned int	cx;			// number of columns; leftmost sum cells excluded
	unsigned int	cy;			// number of rows; topmost sum cells excluded
}	boardHeader_t;

typedef char cellInfo_t;
const cellInfo_t SUM_CELL_MARKER = -1;
const cellInfo_t ANSWER_CELL_MARKER = 0;

typedef void * kkrHandle_t;

//
// callback function definitions
//

typedef unsigned int kkrSolveEvent_t;

const kkrSolveEvent_t KKREVENT_SOLUTION = 0;			// a solution found
const kkrSolveEvent_t KKREVENT_DETERMINED_UPDATE = 1;	// a cell value determined
const kkrSolveEvent_t KKREVENT_TEMPORARY_UPDATE = 2;	// a cell value temporarily put in brue force
const kkrSolveEvent_t KKREVENT_RESET = 3;				// temporary values are reset

typedef struct {
	size_t	x;
	size_t	y;
	unsigned char value;
}	kkrUpdate_t;

typedef void (*kkrSolveEventHandler_t)(kkrSolveEvent_t kkrEvent, void * user, void * data);
	// About data
	// data pointer is invalid once returned from event handler
	//
	// data format
	// KKREVENT_DETERMINED_UPDATE and KKREVENT_TEMPORARY_UPDATE
	//		kkrUpdate_t
	// KKREVENT_SOLUTION and KKREVENT_RESET
	//		sequence of (x,y) in unsinged char
	//			(0,0), (1,0) ... (cx-1,0), (0,1)...(cx-1,1)...(cx-1,cy-1)

typedef enum {
	KKRREQEVENT_NONE,		// no callback
	KKRREQEVENT_SOLUTION,	// when a solution found
	KKRREQEVENT_DETERMINED,	// CBEVENT_SOLUTION + when a cell value determined
	KKRREQEVENT_TEMPORARY,	// CBEVENT_DETERMINED + temp value in brute force
							//		and temp values are cleared in brute force
}	kkrSolveEventRequest_t;

//
// errors
//
typedef enum {
	KKRERR_NO_ERROR,
	KKRERR_INTERNAL_ERROR,		// possibly solver's bug
	KKRERR_SYSTEM_ERROR,		// such as insufficient memory
	KKRERR_INVALID_BOARD_DATA,	// error detail infomation is set
	KKRERR_INVALID_HANDLE,
	KKRERR_INVALID_ARGUMENT,
	KKRERR_NOT_SOLVED_YET,
	KKRERR_INVALID_INDEX,
	KKRERR_INVALID_DATA_STORAGE,
	KKRERR_NO_REASON,			// no error detail is set
} kkrError_t;

typedef enum {
	KKRRSN_UNKNOWN,
	KKRRSN_EDGE_CELL = 1,	// topmost/leftmost has non-sum cell(s)
	KKRRSN_SUM4EMPTY,		// sum is given for consecutive sums
	KKRRSN_NUM_CELL,		// # of cells in a sum is < 2 or > 9
	KKRRSN_SUM,				// impossible sum (ex. 45 for 2-cell sum)
	KKRRSN_BOARD_SIZE,		// board is smaller than 2x2
	KKRRSN_NO_BOARD_DATA,	// no board data is given
	KKRRSN_INVALID_FORMAT,	// invalid data version
	KKRRSN_INVALID_VERSION,	// invalid data version
} kkrErrorReason_t;

#if defined(__cplusplus)
}	// extern "C" {
#endif
