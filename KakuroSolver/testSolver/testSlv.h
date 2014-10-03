/***************************************************************************/
/* testSlv.h                                                               */
/* common header file for the test program of Kakuro Solving Engine        */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/13 Initial                                                      */
/***************************************************************************/

// utilities
extern int CountSol;
extern int CountDet;
extern int CountTmp;
extern int CountReset;
extern bool bVerbose;
extern size_t Size;
extern unsigned char * Expected;

void ResetCounts();
void testHandler(kakuro::slvEvent_t ev, void * user, void * data);

void MakeDataBoard(kakuro::board_t & testBoard, const unsigned * const data);

// test cases
bool test1();
bool test2();
bool test3();
bool test4();
bool test5();
bool test6();
bool test7();