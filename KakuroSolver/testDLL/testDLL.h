/***************************************************************************/
/* testDLL.h                                                               */
/* common header file for the test program of KakuroSolver DLL             */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/10 Initial                                                      */
/***************************************************************************/

/*
** common data for test cases
*/

bool test1();
/* test1()
**  returns true when ok to continue
**
**  Calls each API with intentional invalid argument
**  to make sure APIs return an error properly
*/

bool test2();
/* test1()
**  returns true when ok to continue
**
**  Go through the whole solving process with simple problem
*/

bool test3();
