/***************************************************************************/
/* kkrsolv.h                                                               */
/* Kakuro solver interfaces                                                */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/09 Initial                                                      */
/***************************************************************************/
#include "kkrtypes.h"

#if defined(__cplusplus)
extern "C" {
#endif

//
// API declarations
//
__declspec(dllimport) kkrError_t KkrCreateSolver(void * pBoardData, kkrHandle_t * pHBoard);
__declspec(dllimport) kkrError_t KkrSetEventHandler(kkrHandle_t hBoard, kkrSolveEventRequest_t req, kkrSolveEventHandler_t cb, void * pUser);
__declspec(dllimport) kkrError_t KkrSolve(kkrHandle_t hBoard);
__declspec(dllimport) kkrError_t KkrGetSolutionCount(kkrHandle_t hBoard, size_t * pCount);
__declspec(dllimport) kkrError_t KkrGetSolution(kkrHandle_t hBoard, size_t idx, const unsigned char ** ppData);
	// ppData is valid until any other Kkr APIs are called.
__declspec(dllimport) kkrError_t KkrDeleteSolver(kkrHandle_t hBoard);
__declspec(dllimport) kkrError_t KkrGetLastErrorDetail(kkrErrorReason_t * pReason, size_t * px, size_t * py);
__declspec(dllimport) void KkrGetVersion(int * pMajor, int * pMinor, int * pRelease);

#if defined(__cplusplus)
}	// extern "C" {
#endif
