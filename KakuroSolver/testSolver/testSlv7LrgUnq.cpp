/***************************************************************************/
/* testSlv7LrgUnq.cpp                                                      */
/* test case 5 for Kakuro Solving Engine                                   */
/*    solves mid-sized/large problems with a unique solution               */
/*    tests events as well                                                 */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/22 Initial                                                      */
/***************************************************************************/
#include <iostream>
#include <memory>
#include "../KakuroSolver_t/kkrsolv_t.h"
#include "../KakuroSolver_t/solvimpl.h"
#include "testSlv.h"

using namespace kakuro;

/* Nikoli Vol.142 Kakuro No.4 */
static const unsigned DATA1[] = {
	15, 15,
/*	x,xx,xx, x,xx,xx, x,xx,xx, x,xx,xx, x,xx,xx, x,xx,xx, x,xx,xx, x,xx,xx, x,xx,xx, x,xx,xx, x,xx,xx, x,xx,xx, x,xx,xx, x,xx,xx, x,xx,xx, x,xx,xx,*/
	1,0,0,   1,0,0,   1,0,0,   1,0,24,  1,0,11,  1,0,8,   1,0,0,   1,0,22,  1,0,29,  1,0,0,   1,0,15,  1,0,5,   1,0,10,  1,0,0,   1,0,38,  1,0,14,
	1,0,0,   1,0,0,   1,10,28, 0,0,0,   0,0,0,   0,0,0,   1,10,10, 0,0,0,   0,0,0,   1,7,0,   0,0,0,   0,0,0,   0,0,0,   1,16,6,  0,0,0,   0,0,0,
	1,0,0,   1,42,23, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,21,7,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,
	1,28,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,16,11, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,6,16,  0,0,0,   0,0,0,   0,0,0,   1,0,23,
	1,12,0,  0,0,0,   0,0,0,   1,7,0,   0,0,0,   0,0,0,   0,0,0,   1,15,8,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,15,0,  0,0,0,   0,0,0,
	1,10,0,  0,0,0,   0,0,0,   1,0,31,  1,10,10, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,5,10,  0,0,0,   0,0,0,   1,0,15,  1,17,35, 0,0,0,   0,0,0,
	1,0,0,   1,11,16, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,22,6,  0,0,0,   0,0,0,   0,0,0,   1,35,24, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,
	1,30,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,37,34, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,29,  1,0,16,
	1,18,0,  0,0,0,   0,0,0,   0,0,0,   1,10,10, 0,0,0,   0,0,0,   1,0,4,   1,0,41,  1,12,7,  0,0,0,   0,0,0,   1,23,10, 0,0,0,   0,0,0,   0,0,0,
	1,0,0,   1,0,24,  1,43,22, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,29,6,  0,0,0,   0,0,0,   0,0,0,   0,0,0,
	1,34,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,7,35,  0,0,0,   0,0,0,   0,0,0,   1,11,11, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,24,
	1,10,0,  0,0,0,   0,0,0,   1,0,0,   1,17,29, 0,0,0,   0,0,0,   1,11,12, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,29,  1,13,0,  0,0,0,   0,0,0,
	1,13,0,  0,0,0,   0,0,0,   1,34,10, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,15,7,  0,0,0,   0,0,0,   0,0,0,   1,10,6,  0,0,0,   0,0,0,
	1,0,0,   1,10,7,  0,0,0,   0,0,0,   0,0,0,   1,15,16, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,20,8,  0,0,0,   0,0,0,   0,0,0,   0,0,0,
	1,39,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,28,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,0,
	1,3,0,   0,0,0,   0,0,0,   1,23,0,  0,0,0,   0,0,0,   0,0,0,   1,12,0,  0,0,0,   0,0,0,   1,10,0,  0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,0,0,
};
static const unsigned char SOL1[] = {
	0,0,7,2,1,0,7,3,0,1,4,2,0,7,9,
	0,5,8,3,7,4,9,6,0,2,1,3,4,6,5,
	8,6,9,5,0,1,6,2,4,3,0,1,2,3,0,
	9,3,0,1,4,2,0,1,3,5,2,4,0,9,6,
	6,4,0,0,2,3,1,4,0,4,1,0,0,8,9,
	0,1,3,2,5,0,5,8,9,0,6,9,7,5,8,
	9,7,6,8,0,3,2,5,1,7,4,6,9,0,0,
	7,2,9,0,8,2,0,0,0,9,3,0,6,8,9,
	0,0,5,6,7,1,3,9,4,8,0,9,8,5,7,
	9,7,8,4,6,0,1,4,2,0,2,1,5,3,0,
	7,3,0,0,9,8,0,5,1,2,3,0,0,6,7,
	8,5,0,9,4,7,8,6,0,5,1,9,0,2,8,
	0,1,2,7,0,5,4,2,1,3,0,8,2,1,9,
	6,4,8,5,7,9,0,7,2,1,6,5,3,4,0,
	1,2,0,8,9,6,0,8,4,0,2,7,1,0,0,
};

/* Nikoli Vol.137 Kakuro No.5 */
static const unsigned DATA2[] = {
	21, 31,
/*		x,xx,00, x,xx,01, x,xx,02, x,xx,03, x,xx,04, x,xx,05, x,xx,06, x,xx,07, x,xx,08, x,xx,09, x,xx,10, x,xx,11, x,xx,12, x,xx,13, x,xx,14, x,xx,15, x,xx,16, x,xx,17, x,xx,18, x,xx,19, x,xx,20, x,xx,21, */
/* 0*/	1,0,0,   1,0,0,   1,0,0,   1,0,4,   1,0,6,   1,0,16,  1,0,0,   1,0,0,   1,0,34,  1,0,11,  1,0,14,  1,0,26,  1,0,0,   1,0,0,   1,0,10,  1,0,7,   1,0,0,   1,0,0,   1,0,0,   1,0,20,  1,0,39,  1,0,0,
/* 1*/	1,0,0,   1,0,0,   1,7,34,  0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,29,32, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,4,44,  0,0,0,   0,0,0,   1,0,11,  1,0,0,   1,15,18, 0,0,0,   0,0,0,   1,0,12,
/* 2*/	1,0,0,   1,10,7,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,34,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,10,21, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,10,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,
/* 3*/	1,12,0,  0,0,0,   0,0,0,   1,4,0,   0,0,0,   0,0,0,   1,17,30, 0,0,0,   0,0,0,   1,0,6,   1,39,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,29,7,  0,0,0,   0,0,0,   0,0,0,   0,0,0,
/* 4*/	1,11,0,  0,0,0,   0,0,0,   1,0,0,   1,16,15, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,6,7,   0,0,0,   0,0,0,   0,0,0,   1,0,14,  1,10,0,  0,0,0,   0,0,0,   0,0,0,   1,12,0,  0,0,0,   0,0,0,
/* 5*/	1,7,0,   0,0,0,   0,0,0,   1,41,12, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,24,3,  0,0,0,   0,0,0,   0,0,0,   1,6,4,   0,0,0,   0,0,0,   1,0,7,   1,11,11, 0,0,0,   0,0,0,
/* 6*/	1,0,0,	 1,12,0,  0,0,0,   0,0,0,   0,0,0,   1,15,15, 0,0,0,   0,0,0,   1,0,0,   1,22,39, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,11,10, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,0,
/* 7*/	1,0,0,   1,0,0,   1,14,22, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,15,8,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,3,15,  0,0,0,   0,0,0,   1,8,12,  0,0,0,   0,0,0,   1,0,26,  1,0,0,
/* 8*/	1,0,0,   1,30,11, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,6,   1,13,37, 0,0,0,   0,0,0,   1,0,24,  1,0,0,   1,3,17,  0,0,0,   0,0,0,   1,15,14, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,15,
/* 9*/	1,7,0,   0,0,0,   0,0,0,   0,0,0,   1,22,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,39,14, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,17,41, 0,0,0,   0,0,0,
/*10*/	1,8,0,   0,0,0,   0,0,0,   1,0,7,   1,0,0,   1,10,4,  0,0,0,   0,0,0,   1,29,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,7,10,  0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,23,13, 0,0,0,   0,0,0,   0,0,0,
/*11*/	1,6,0,   0,0,0,   0,0,0,   0,0,0,   1,10,33, 0,0,0,   0,0,0,   0,0,0,   1,23,23, 0,0,0,   0,0,0,   0,0,0,   1,6,14,  0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,14,11, 0,0,0,   0,0,0,   0,0,0,   1,0,0,
/*12*/	1,16,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,24,0,  0,0,0,   0,0,0,   0,0,0,   1,11,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,7,17,  0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,0,0,
/*13*/	1,0,0,   1,9,0,   0,0,0,   0,0,0,   0,0,0,   1,0,8,   1,12,24, 0,0,0,   0,0,0,   1,0,6,   1,0,0,   1,5,16,  0,0,0,   0,0,0,   1,0,8,   1,30,23, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,13,  1,0,0,
/*14*/	1,0,0,   1,0,23,  1,0,29,  1,37,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,39,3,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,6,0,   0,0,0,   0,0,0,   0,0,0,   1,0,23,
/*15*/	1,16,0,  0,0,0,   0,0,0,   1,20,42, 0,0,0,   0,0,0,   0,0,0,   1,0,7,   1,7,0,   0,0,0,   0,0,0,   0,0,0,   1,0,4,   1,12,0,  0,0,0,   0,0,0,   1,0,21,  1,0,0,   1,15,16, 0,0,0,   0,0,0,   0,0,0,
/*16*/	1,30,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,10,0,  0,0,0,   0,0,0,   1,0,4,   1,7,0,   0,0,0,   0,0,0,   0,0,0,   1,0,7,   1,15,0,  0,0,0,   0,0,0,   1,28,12, 0,0,0,   0,0,0,   0,0,0,   0,0,0,
/*17*/	1,24,0,  0,0,0,   0,0,0,   0,0,0,   1,0,20,  1,0,0,   1,5,3,   0,0,0,   0,0,0,   1,0,30,  1,6,16,  0,0,0,   0,0,0,   0,0,0,   1,0,24,  1,8,37,  0,0,0,   0,0,0,   0,0,0,   1,12,0,  0,0,0,   0,0,0,
/*18*/	1,0,0,   1,22,0,  0,0,0,   0,0,0,   0,0,0,   1,22,13, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,37,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,20,  1,0,21,  1,0,0,
/*19*/	1,0,0,   1,0,0,   1,10,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,12,15, 0,0,0,   0,0,0,   1,0,20,  1,0,0,   1,17,39, 0,0,0,   0,0,0,   1,0,0,   1,14,4,  0,0,0,   0,0,0,   0,0,0,   1,0,29,
/*20*/	1,0,0,   1,0,0,   1,23,12, 0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,29,24, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,23,11, 0,0,0,   0,0,0,   0,0,0,   1,15,11, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,
/*21*/	1,0,0,   1,7,3,   0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,19,28, 0,0,0,   0,0,0,   0,0,0,   1,23,7,  0,0,0,   0,0,0,   0,0,0,   1,10,0,  0,0,0,   0,0,0,   0,0,0,   1,23,0,  0,0,0,   0,0,0,   0,0,0,
/*22*/	1,8,0,   0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,18,8,  0,0,0,   0,0,0,   0,0,0,   1,11,44, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,17,8,  0,0,0,   0,0,0,   1,0,6,   1,0,0,   1,14,17, 0,0,0,   0,0,0,
/*23*/	1,7,0,   0,0,0,   0,0,0,   1,0,23,  1,38,10, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,21,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,10,10, 0,0,0,   0,0,0,   0,0,0,
/*24*/	1,0,0,   1,16,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,3,16,  0,0,0,   0,0,0,   1,0,17,  1,0,6,   1,12,7,  0,0,0,   0,0,0,   1,0,0,   1,11,29, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,0,
/*25*/	1,0,0,   1,0,0,   1,10,21, 0,0,0,   0,0,0,   1,15,23, 0,0,0,   0,0,0,   1,16,10, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,24,34, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,33,  1,0,0,
/*26*/	1,0,0,   1,29,13, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,39,10, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,7,   1,17,16, 0,0,0,   0,0,0,   1,6,17,  0,0,0,   0,0,0,   0,0,0,   1,0,19,
/*27*/	1,4,0,   0,0,0,   0,0,0,   1,0,0,   1,10,14, 0,0,0,   0,0,0,   1,10,0,  0,0,0,   0,0,0,   0,0,0,   1,28,11, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,17,0,  0,0,0,   0,0,0,
/*28*/	1,5,0,   0,0,0,   0,0,0,   1,14,20, 0,0,0,   0,0,0,   0,0,0,   1,0,6,   1,7,7,   0,0,0,   0,0,0,   0,0,0,   1,15,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,7,   1,14,0,  0,0,0,   0,0,0,
/*29*/	1,30,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,22,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,3,   1,11,7,  0,0,0,   0,0,0,   1,8,0,   0,0,0,   0,0,0,   1,12,6,  0,0,0,   0,0,0,
/*30*/	1,11,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,10,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,19,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,12,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,0,
/*31*/	1,0,0,   1,8,0,   0,0,0,   0,0,0,   1,0,0,   1,0,0,   1,6,0,   0,0,0,   0,0,0,   1,0,0,   1,12,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,11,0,  0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,0,0,
};
static const unsigned char SOL2[] = {
/* 1*/	0,0,1,2,4,0,0,8,7,5,9,0,0,3,1,0,0,0,9,6,0,
/* 2*/  0,4,3,1,2,0,7,6,4,9,8,0,4,1,2,3,0,2,3,4,1,
/* 3*/	4,8,0,3,1,0,8,9,0,0,7,8,9,6,4,5,0,9,8,7,5,
/* 4*/	2,9,0,0,3,6,2,4,1,0,2,1,3,0,0,1,2,7,0,8,4,
/* 5*/	1,6,0,4,6,8,9,7,5,2,0,7,8,9,0,2,4,0,0,9,2,
/* 6*/	0,7,3,2,0,9,6,0,0,4,1,2,7,5,3,0,1,2,3,5,0,
/* 7*/	0,0,2,1,4,7,0,0,4,1,2,3,5,0,1,2,0,1,7,0,0,
/* 8*/	0,7,6,8,9,0,0,5,8,0,0,0,2,1,0,3,5,4,1,2,0,
/* 9*/	2,4,1,0,2,1,4,3,5,7,0,8,6,5,9,4,7,0,0,8,9,
/*10*/	5,3,0,0,0,2,8,0,7,8,5,9,0,4,2,1,0,0,8,9,6,
/*11*/	3,1,2,0,1,3,6,0,6,9,8,0,1,2,3,0,0,2,5,7,0,
/*12*/	1,2,4,6,3,0,7,8,9,0,1,5,2,3,0,0,2,1,4,0,0,
/*13*/	0,5,1,3,0,0,3,9,0,0,0,2,3,0,0,8,9,7,6,0,0,
/*14*/	0,0,0,8,5,7,9,6,2,0,6,7,4,5,8,9,0,3,2,1,0,
/*15*/	9,7,0,9,3,8,0,0,4,1,2,0,0,3,9,0,0,0,7,2,6,
/*16*/	6,8,9,7,0,9,1,0,0,2,4,1,0,0,6,9,0,4,9,7,8,
/*17*/	8,9,7,0,0,0,2,3,0,0,1,3,2,0,0,4,3,1,0,3,9,
/*18*/	0,5,8,9,0,2,4,1,7,5,3,0,5,7,2,8,9,6,0,0,0,
/*19*/	0,0,3,2,4,1,0,0,8,4,0,0,0,9,8,0,0,3,7,4,0,
/*20*/	0,0,6,8,9,0,0,5,9,7,8,0,9,8,6,0,3,2,4,1,5,
/*21*/	0,2,4,1,0,0,9,4,6,0,9,6,8,0,7,2,1,0,9,6,8,
/*22*/	2,1,5,0,0,9,7,2,0,2,3,1,5,0,9,8,0,0,0,5,9,
/*23*/	1,6,0,0,6,7,8,3,9,5,0,4,6,3,5,1,2,0,1,2,7,
/*24*/	0,3,6,1,2,4,0,1,2,0,0,0,7,5,0,0,1,2,5,3,0,
/*25*/	0,0,8,2,0,8,7,0,6,3,1,2,4,0,0,8,3,4,9,0,0,
/*26*/	0,5,9,7,8,0,9,6,8,7,5,4,0,0,8,9,0,1,2,3,0,
/*27*/	3,1,0,0,9,1,0,4,5,1,0,1,5,2,6,7,4,3,0,9,8,
/*28*/	1,4,0,5,6,3,0,0,4,2,1,0,2,3,4,5,1,0,0,8,6,
/*29*/	7,6,9,8,0,2,3,1,7,4,5,0,0,4,7,0,7,1,0,7,5,
/*30*/	2,3,5,1,0,4,1,2,3,0,3,2,4,1,9,0,3,2,1,6,0,
/*31*/	0,2,6,0,0,0,2,4,0,0,2,1,3,6,0,0,2,4,5,0,0,
};

/* nikoli.com sample No.10 */
static const unsigned DATA3[] = {
	21, 13,
/*		x,xx,00, x,xx,01, x,xx,02, x,xx,03, x,xx,04, x,xx,05, x,xx,06, x,xx,07, x,xx,08, x,xx,09, x,xx,10, x,xx,11, x,xx,12, x,xx,13, x,xx,14, x,xx,15, x,xx,16, x,xx,17, x,xx,18, x,xx,19, x,xx,20, x,xx,21, */
/* 0*/	1,0,0,   1,0,10,  1,0,24,  1,0,7,   1,0,0,   1,0,0,   1,0,0,   1,0,8,   1,0,17,  1,0,13,  1,0,0,   1,0,0,   1,0,0,   1,0,22,  1,0,12,  1,0,24,  1,0,0,   1,0,0,   1,0,0,   1,0,19,  1,0,28,  1,0,14,
/* 1*/	1,7,0,   0,0,0,   0,0,0,   0,0,0,   1,0,16,  1,0,0,   1,12,17, 0,0,0,   0,0,0,   0,0,0,   1,0,32,  1,0,0,   1,23,34, 0,0,0,   0,0,0,   0,0,0,   1,0,35,  1,0,0,   1,23,31, 0,0,0,   0,0,0,   0,0,0,
/* 2*/	1,11,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,19,9,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,33,7,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,28,7,  0,0,0,   0,0,0,   0,0,0,   0,0,0,
/* 3*/	1,28,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,34,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,29,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,
/* 4*/	1,12,0,  0,0,0,   0,0,0,   1,7,27,  0,0,0,   0,0,0,   0,0,0,   1,0,16,  1,0,0,   1,16,20, 0,0,0,   0,0,0,   0,0,0,   1,0,30,  1,0,0,   1,10,30, 0,0,0,   0,0,0,   0,0,0,   1,10,26, 0,0,0,   0,0,0,
/* 5*/	1,0,0,   1,0,12,  1,18,20, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,20,22, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,29,9,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,10,  1,0,20,
/* 6*/	1,24,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,35,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,23,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,26,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,
/* 7*/	1,23,0,  0,0,0,   0,0,0,   0,0,0,   1,0,31,  1,0,0,   1,9,16,  0,0,0,   0,0,0,   0,0,0,   1,0,17,  1,0,0,   1,10,32, 0,0,0,   0,0,0,   0,0,0,   1,0,19,  1,0,0,   1,23,34, 0,0,0,   0,0,0,   0,0,0,
/* 8*/	1,15,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,19,8,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,33,24, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,10,24, 0,0,0,   0,0,0,   0,0,0,   0,0,0,
/* 9*/	1,0,0,   1,0,10,  1,19,15, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,35,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,31,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,0,30,  1,0,12,
/*10*/	1,4,0,   0,0,0,   0,0,0,   1,10,20, 0,0,0,   0,0,0,   0,0,0,   1,0,6,   1,0,0,   1,18,9,  0,0,0,   0,0,0,   0,0,0,   1,0,6,   1,0,0,   1,24,7,  0,0,0,   0,0,0,   0,0,0,   1,12,24,  0,0,0,   0,0,0,
/*11*/	1,41,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,23,8,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,36,7,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,
/*12*/	1,14,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,17,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,15,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,26,0,  0,0,0,   0,0,0,   0,0,0,   0,0,0,
/*13*/	1,7,0,   0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,0,0,   1,7,0,   0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,0,0,   1,6,0,   0,0,0,   0,0,0,   0,0,0,   1,0,0,   1,0,0,   1,23,0,  0,0,0,   0,0,0,   0,0,0,
};

static const unsigned char SOL3[] = {
/* 1*/	1,4,2,0,0,0,1,8,3,0,0,0,6,8,9,0,0,0,9,8,6,
/* 2*/  3,5,1,2,0,4,2,9,1,3,0,8,9,4,7,5,0,8,7,9,4,
/* 3*/	2,7,4,6,3,1,5,0,9,8,4,6,7,0,8,6,2,5,3,4,1,
/* 4*/	4,8,0,4,1,2,0,0,0,5,2,9,0,0,0,7,1,2,0,7,3,
/* 5*/	0,0,7,1,5,3,2,0,2,7,1,4,6,0,9,8,4,7,1,0,0,
/* 6*/	5,7,9,3,0,7,6,8,5,9,0,7,2,1,4,9,0,9,6,3,8,
/* 7*/	6,9,8,0,0,0,1,5,3,0,0,0,5,3,2,0,0,0,8,6,9,
/* 8*/	1,4,2,8,0,2,4,9,1,3,0,8,9,5,7,4,0,4,2,1,3,
/* 9*/	0,0,1,9,2,4,3,0,9,6,7,5,8,0,8,1,7,6,9,0,0,
/*10*/	3,1,0,6,1,3,0,0,0,2,9,7,0,0,0,9,8,7,0,9,3,
/*11*/	4,8,9,7,5,6,2,0,3,1,8,9,2,0,2,3,9,8,7,6,1,
/*12*/	2,4,7,1,0,1,3,6,2,5,0,3,1,5,4,2,0,9,8,7,2,
/*13*/	1,2,4,0,0,0,1,2,4,0,0,0,3,2,1,0,0,0,9,8,6,
};

static const unsigned * TestData[] = {
	DATA1,
	DATA2,
	DATA3,
};
static const unsigned char * TestSol[] = {
	SOL1,
	SOL2,
	SOL3,
};
static const size_t CTestData = sizeof(TestData)/sizeof(TestData[0]);

typedef bool (*evalEvents)();

static bool testCommon(slvEventRequest_t req, eventHandler_t eh, evalEvents fEval)
{
	bool bPassed = true;
	board_t testBoard;

	for(auto i = 0; i < CTestData; i++) {
		bool bPassedThis = true;
		const size_t boardSize = TestData[i][0] * TestData[i][1];
		std::cout << "\ttest " << i << ": ";

		ResetCounts();
		MakeDataBoard(testBoard, TestData[i]);

		try {
			solverImpl_t solver(testBoard);
			if(req != REQEVENT_NONE) {
				Size = boardSize;
				solver.SetEventHandler(req, eh, nullptr);
			}
			solver.Solve();
			if(fEval != nullptr)
				bPassedThis &= fEval();
			const size_t numSol = solver.GetSolutionCount();
			if(numSol == 1) {
				const unsigned char * solution;
				solver.GetSolution(0, &solution);
				size_t cell;
				for(cell = 0; cell < boardSize; cell++)
					if(solution[cell] != TestSol[i][cell])
						break;
				if(cell != boardSize) {
					bPassedThis = false;
					std::cout << "Solution not match" << std::endl;
					std::cout << "\t\texpected:\t";
					for(cell = 0; cell < boardSize; cell++)
						std::cout << static_cast<unsigned>(TestSol[i][cell]) << ", ";
					std::cout << std::endl << "\t\tacutal:\t";
					for(cell = 0; cell < boardSize; cell++)
						std::cout << static_cast<unsigned>(solution[cell]) << ", ";
				}
			} else {
				// doesn't have a unique solution
				bPassedThis = false;
				std::cout << "# of Solutions: " << numSol << std::endl;
				for(size_t solId = 0; solId < numSol; solId++) {
					const unsigned char * solution;
					solver.GetSolution(solId, &solution);
					std::cout << "\t\t";
					for(size_t cell = 0; cell < boardSize; cell++)
						std::cout << solution[cell] << ", ";
				}
			}
		}
		catch(slvEceptionSolverBug_t & eBug) {
			bPassedThis = false;
			std::cout << "Bug exception caught: line=" << eBug.GetLine()
				<< ", filename=" <<eBug.GetFileName();
		}
		catch(slvException_t & e) {
			bPassedThis = false;
			std::cout << "exception caught: " << e.GetType();
		}

		delete [] testBoard.pData;

		std::cout << (bPassedThis ? "succeeded" : "!!!! FAILED !!!!")
			<< std::endl;
		bPassed &= bPassedThis;
	}

	return bPassed;
}

static bool testNoEvent()
{
	std::cout << "*     Solve without events" << std::endl;
	return testCommon(REQEVENT_NONE, nullptr, nullptr);
}

static bool testSolEval()
{
	bool bPassed = true;

	if(CountSol != 1) {
		std::cout << "\t\tEVENT_SOLUTION fired " << CountSol << " times; once is expected" << std::endl;
		bPassed = false;
	}
	if(CountDet != 0) {
		std::cout << "\t\tEVENT_DETERMINED_UPDATE falsely fired" << std::endl;
		bPassed = false;
	}
	if(CountTmp != 0) {
		std::cout << "\t\tEVENT_TEMPORARY_UPDATE falsely fired" << std::endl;
		bPassed = false;
	}
	if(CountReset != 0) {
		std::cout << "\t\tEVENT_RESET falsely fired" << std::endl;
		bPassed = false;
	}

	return bPassed;
}

static bool testSol()
{
	std::cout << "*     Solve REQEVENT_SOLUTION" << std::endl;
	return testCommon(REQEVENT_SOLUTION, testHandler, testSolEval);
}

static bool testDetEval()
{
	bool bPassed = true;

	if(CountSol != 1) {
		std::cout << "\t\tEVENT_SOLUTION fired " << CountSol << " times; once is expected" << std::endl;
		bPassed = false;
	}
	std::cout << "\t\tEVENT_DETERMINED_UPDATE fired " << CountDet <<" times" << std::endl;
	if(CountTmp != 0) {
		std::cout << "\t\tEVENT_TEMPORARY_UPDATE falsely fired" << std::endl;
		bPassed = false;
	}
	if(CountReset != 0) {
		std::cout << "\t\tEVENT_RESET falsely fired" << std::endl;
		bPassed = false;
	}

	return bPassed;
}

static bool testDet()
{
	std::cout << "*     Solve REQEVENT_DETERMINED" << std::endl;
	return testCommon(REQEVENT_DETERMINED, testHandler, testDetEval);
}

static bool testTmpEval()
{
	bool bPassed = true;

	if(CountSol != 1) {
		std::cout << "\t\tEVENT_SOLUTION fired " << CountSol << " times; once is expected" << std::endl;
		bPassed = false;
	}
	std::cout << "\t\tEVENT_DETERMINED_UPDATE fired " << CountDet << " times" << std::endl;
	std::cout << "\t\tEVENT_TEMPORARY_UPDATE fired " << CountTmp << " times" << std::endl;
	std::cout << "\t\tEVENT_RESET fired " << CountReset << " times" << std::endl;

	return bPassed;
}

static bool testTmp()
{
	std::cout << "*     Solve REQEVENT_TEMPORARY" << std::endl;
	return testCommon(REQEVENT_TEMPORARY, testHandler, testTmpEval);
}

bool test7()
{
	bool bPassed = true;
	std::cout << "***** test case 7: solve large unique problems; Events" << std::endl;
	bVerbose = false;

	bPassed &= testNoEvent();
	bPassed &= testSol();
	bPassed &= testDet();
	bPassed &= testTmp();

	return bPassed;
}
