/***************************************************************************/
/* apiutil.h                                                               */
/* definitions of utilities for kakuro solver APIs                         */
/* requires kkrtypes.h                                                     */
/*                                                                         */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/11 Initial                                                      */
/***************************************************************************/

// errors inside the API project
typedef enum {
	APIERR_NO_ERROR,
	APIERR_SYSTEM_ERROR,		// such as insufficient memory
	APIERR_HANDLE_NOT_EXISTS,
}	APIError_t;

// handle store
APIError_t HandleInsert(kkrHandle_t newHandle);
bool HandleExists(kkrHandle_t aHandle);
	// returns true if aHandle exists
APIError_t HandleErase(kkrHandle_t aHandle);

// solver error detail
void ClearErrorDetail();
void SetErrorDetail(kkrErrorReason_t reason, size_t x, size_t y);
void SetErrorDetail(const kakuro::slvExceptionInvlideBoardData_t & e);