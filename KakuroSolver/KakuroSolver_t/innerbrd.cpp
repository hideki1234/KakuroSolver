/***************************************************************************/
/* innerbrd.cpp                                                            */
/* inner kakuro board implementation                                       */
/* Copyright Hideki Ikeda, 2013                                            */
/*                                                                         */
/* 2013/10/15 Initial                                                      */
/***************************************************************************/

#include <memory>
#include "kkrsolv_t.h"
#include "innerbrd.h"

namespace kakuro {
	/*
	** helper functions
	*/
	static void SetCandidatesFromCellsAndSum(size_t cCell, size_t sum, candList_t & candSet)
	{
		const static valueSet_t	CAND2_3_12(0x3);
		const static valueSet_t	CAND2_4_13(0x5);
		const static valueSet_t	CAND2_5_14(0x9);
		const static valueSet_t	CAND2_5_23(0x6);
		const static valueSet_t	CAND2_6_15(0x11);
		const static valueSet_t	CAND2_6_24(0xa);
		const static valueSet_t	CAND2_7_16(0x21);
		const static valueSet_t	CAND2_7_25(0x12);
		const static valueSet_t	CAND2_7_34(0xc);
		const static valueSet_t	CAND2_8_17(0x41);
		const static valueSet_t	CAND2_8_26(0x22);
		const static valueSet_t	CAND2_8_35(0x14);
		const static valueSet_t	CAND2_9_18(0x81);
		const static valueSet_t	CAND2_9_27(0x42);
		const static valueSet_t	CAND2_9_36(0x24);
		const static valueSet_t	CAND2_9_45(0x18);
		const static valueSet_t	CAND2_10_19(0x101);
		const static valueSet_t	CAND2_10_28(0x82);
		const static valueSet_t	CAND2_10_37(0x44);
		const static valueSet_t	CAND2_10_46(0x28);
		const static valueSet_t	CAND2_11_29(0x102);
		const static valueSet_t	CAND2_11_38(0x84);
		const static valueSet_t	CAND2_11_47(0x48);
		const static valueSet_t	CAND2_11_56(0x30);
		const static valueSet_t	CAND2_12_39(0x104);
		const static valueSet_t	CAND2_12_48(0x88);
		const static valueSet_t	CAND2_12_57(0x50);
		const static valueSet_t	CAND2_13_49(0x108);
		const static valueSet_t	CAND2_13_58(0x90);
		const static valueSet_t	CAND2_13_67(0x60);
		const static valueSet_t	CAND2_14_59(0x110);
		const static valueSet_t	CAND2_14_68(0xa0);
		const static valueSet_t	CAND2_15_69(0x120);
		const static valueSet_t	CAND2_15_78(0xc0);
		const static valueSet_t	CAND2_16_79(0x140);
		const static valueSet_t	CAND2_17_89(0x180);
		const static valueSet_t	CAND3_6_123(0x7);
		const static valueSet_t	CAND3_7_124(0xb);
		const static valueSet_t	CAND3_8_125(0x13);
		const static valueSet_t	CAND3_8_134(0xd);
		const static valueSet_t	CAND3_9_126(0x23);
		const static valueSet_t	CAND3_9_135(0x15);
		const static valueSet_t	CAND3_9_234(0xe);
		const static valueSet_t	CAND3_10_127(0x43);
		const static valueSet_t	CAND3_10_136(0x25);
		const static valueSet_t	CAND3_10_145(0x19);
		const static valueSet_t	CAND3_10_235(0x16);
		const static valueSet_t	CAND3_11_128(0x83);
		const static valueSet_t	CAND3_11_137(0x45);
		const static valueSet_t	CAND3_11_146(0x29);
		const static valueSet_t	CAND3_11_236(0x26);
		const static valueSet_t	CAND3_11_245(0x1a);
		const static valueSet_t	CAND3_12_129(0x103);
		const static valueSet_t	CAND3_12_138(0x85);
		const static valueSet_t	CAND3_12_147(0x49);
		const static valueSet_t	CAND3_12_156(0x31);
		const static valueSet_t	CAND3_12_237(0x46);
		const static valueSet_t	CAND3_12_246(0x2a);
		const static valueSet_t	CAND3_12_345(0x1c);
		const static valueSet_t	CAND3_13_139(0x105);
		const static valueSet_t	CAND3_13_148(0x89);
		const static valueSet_t	CAND3_13_157(0x51);
		const static valueSet_t	CAND3_13_238(0x86);
		const static valueSet_t	CAND3_13_247(0x4a);
		const static valueSet_t	CAND3_13_256(0x32);
		const static valueSet_t	CAND3_13_346(0x2c);
		const static valueSet_t	CAND3_14_149(0x109);
		const static valueSet_t	CAND3_14_158(0x91);
		const static valueSet_t	CAND3_14_167(0x61);
		const static valueSet_t	CAND3_14_239(0x106);
		const static valueSet_t	CAND3_14_248(0x8a);
		const static valueSet_t	CAND3_14_257(0x52);
		const static valueSet_t	CAND3_14_347(0x4c);
		const static valueSet_t	CAND3_14_356(0x34);
		const static valueSet_t	CAND3_15_159(0x111);
		const static valueSet_t	CAND3_15_168(0xa1);
		const static valueSet_t	CAND3_15_249(0x10a);
		const static valueSet_t	CAND3_15_258(0x92);
		const static valueSet_t	CAND3_15_267(0x62);
		const static valueSet_t	CAND3_15_348(0x8c);
		const static valueSet_t	CAND3_15_357(0x54);
		const static valueSet_t	CAND3_15_456(0x38);
		const static valueSet_t	CAND3_16_169(0x121);
		const static valueSet_t	CAND3_16_178(0xc1);
		const static valueSet_t	CAND3_16_259(0x112);
		const static valueSet_t	CAND3_16_268(0xa2);
		const static valueSet_t	CAND3_16_349(0x10c);
		const static valueSet_t	CAND3_16_358(0x94);
		const static valueSet_t	CAND3_16_367(0x64);
		const static valueSet_t	CAND3_16_457(0x58);
		const static valueSet_t	CAND3_17_179(0x141);
		const static valueSet_t	CAND3_17_269(0x122);
		const static valueSet_t	CAND3_17_278(0xc2);
		const static valueSet_t	CAND3_17_359(0x114);
		const static valueSet_t	CAND3_17_368(0xa4);
		const static valueSet_t	CAND3_17_458(0x98);
		const static valueSet_t	CAND3_17_467(0x68);
		const static valueSet_t	CAND3_18_189(0x181);
		const static valueSet_t	CAND3_18_279(0x142);
		const static valueSet_t	CAND3_18_369(0x124);
		const static valueSet_t	CAND3_18_378(0xc4);
		const static valueSet_t	CAND3_18_459(0x118);
		const static valueSet_t	CAND3_18_468(0xa8);
		const static valueSet_t	CAND3_18_567(0x70);
		const static valueSet_t	CAND3_19_289(0x182);
		const static valueSet_t	CAND3_19_379(0x144);
		const static valueSet_t	CAND3_19_469(0x128);
		const static valueSet_t	CAND3_19_478(0xc8);
		const static valueSet_t	CAND3_19_568(0xb0);
		const static valueSet_t	CAND3_20_389(0x184);
		const static valueSet_t	CAND3_20_479(0x148);
		const static valueSet_t	CAND3_20_569(0x130);
		const static valueSet_t	CAND3_20_578(0xd0);
		const static valueSet_t	CAND3_21_489(0x188);
		const static valueSet_t	CAND3_21_579(0x150);
		const static valueSet_t	CAND3_21_678(0xe0);
		const static valueSet_t	CAND3_22_589(0x190);
		const static valueSet_t	CAND3_22_679(0x160);
		const static valueSet_t	CAND3_23_689(0x1a0);
		const static valueSet_t	CAND3_24_789(0x1c0);
		const static valueSet_t	CAND4_10_1234(0xf);
		const static valueSet_t	CAND4_11_1235(0x17);
		const static valueSet_t	CAND4_12_1236(0x27);
		const static valueSet_t	CAND4_12_1245(0x1b);
		const static valueSet_t	CAND4_13_1237(0x47);
		const static valueSet_t	CAND4_13_1246(0x2b);
		const static valueSet_t	CAND4_13_1345(0x1d);
		const static valueSet_t	CAND4_14_1238(0x87);
		const static valueSet_t	CAND4_14_1247(0x4b);
		const static valueSet_t	CAND4_14_1256(0x33);
		const static valueSet_t	CAND4_14_1346(0x2d);
		const static valueSet_t	CAND4_14_2345(0x1e);
		const static valueSet_t	CAND4_15_1239(0x107);
		const static valueSet_t	CAND4_15_1248(0x8b);
		const static valueSet_t	CAND4_15_1257(0x53);
		const static valueSet_t	CAND4_15_1347(0x4d);
		const static valueSet_t	CAND4_15_1356(0x35);
		const static valueSet_t	CAND4_15_2346(0x2e);
		const static valueSet_t	CAND4_16_1249(0x10b);
		const static valueSet_t	CAND4_16_1258(0x93);
		const static valueSet_t	CAND4_16_1267(0x63);
		const static valueSet_t	CAND4_16_1348(0x8d);
		const static valueSet_t	CAND4_16_1357(0x55);
		const static valueSet_t	CAND4_16_1456(0x39);
		const static valueSet_t	CAND4_16_2347(0x4e);
		const static valueSet_t	CAND4_16_2356(0x36);
		const static valueSet_t	CAND4_17_1259(0x113);
		const static valueSet_t	CAND4_17_1268(0xa3);
		const static valueSet_t	CAND4_17_1349(0x10d);
		const static valueSet_t	CAND4_17_1358(0x95);
		const static valueSet_t	CAND4_17_1367(0x65);
		const static valueSet_t	CAND4_17_1457(0x59);
		const static valueSet_t	CAND4_17_2348(0x8e);
		const static valueSet_t	CAND4_17_2357(0x56);
		const static valueSet_t	CAND4_17_2456(0x3a);
		const static valueSet_t	CAND4_18_1269(0x123);
		const static valueSet_t	CAND4_18_1278(0xc3);
		const static valueSet_t	CAND4_18_1359(0x115);
		const static valueSet_t	CAND4_18_1368(0xa5);
		const static valueSet_t	CAND4_18_1458(0x99);
		const static valueSet_t	CAND4_18_1467(0x69);
		const static valueSet_t	CAND4_18_2349(0x10e);
		const static valueSet_t	CAND4_18_2358(0x96);
		const static valueSet_t	CAND4_18_2367(0x66);
		const static valueSet_t	CAND4_18_2457(0x5a);
		const static valueSet_t	CAND4_18_3456(0x3c);
		const static valueSet_t	CAND4_19_1279(0x143);
		const static valueSet_t	CAND4_19_1369(0x125);
		const static valueSet_t	CAND4_19_1378(0xc5);
		const static valueSet_t	CAND4_19_1459(0x119);
		const static valueSet_t	CAND4_19_1468(0xa9);
		const static valueSet_t	CAND4_19_1567(0x71);
		const static valueSet_t	CAND4_19_2359(0x116);
		const static valueSet_t	CAND4_19_2368(0xa6);
		const static valueSet_t	CAND4_19_2458(0x9a);
		const static valueSet_t	CAND4_19_2467(0x6a);
		const static valueSet_t	CAND4_19_3457(0x5c);
		const static valueSet_t	CAND4_20_1289(0x183);
		const static valueSet_t	CAND4_20_1379(0x145);
		const static valueSet_t	CAND4_20_1469(0x129);
		const static valueSet_t	CAND4_20_1478(0xc9);
		const static valueSet_t	CAND4_20_1568(0xb1);
		const static valueSet_t	CAND4_20_2369(0x126);
		const static valueSet_t	CAND4_20_2378(0xc6);
		const static valueSet_t	CAND4_20_2459(0x11a);
		const static valueSet_t	CAND4_20_2468(0xaa);
		const static valueSet_t	CAND4_20_2567(0x72);
		const static valueSet_t	CAND4_20_3458(0x9c);
		const static valueSet_t	CAND4_20_3467(0x6c);
		const static valueSet_t	CAND4_21_1389(0x185);
		const static valueSet_t	CAND4_21_1479(0x149);
		const static valueSet_t	CAND4_21_1569(0x131);
		const static valueSet_t	CAND4_21_1578(0xd1);
		const static valueSet_t	CAND4_21_2379(0x146);
		const static valueSet_t	CAND4_21_2469(0x12a);
		const static valueSet_t	CAND4_21_2478(0xca);
		const static valueSet_t	CAND4_21_2568(0xb2);
		const static valueSet_t	CAND4_21_3459(0x11c);
		const static valueSet_t	CAND4_21_3468(0xac);
		const static valueSet_t	CAND4_21_3567(0x74);
		const static valueSet_t	CAND4_22_1489(0x189);
		const static valueSet_t	CAND4_22_1579(0x151);
		const static valueSet_t	CAND4_22_1678(0xe1);
		const static valueSet_t	CAND4_22_2389(0x186);
		const static valueSet_t	CAND4_22_2479(0x14a);
		const static valueSet_t	CAND4_22_2569(0x132);
		const static valueSet_t	CAND4_22_2578(0xd2);
		const static valueSet_t	CAND4_22_3469(0x12c);
		const static valueSet_t	CAND4_22_3478(0xcc);
		const static valueSet_t	CAND4_22_3568(0xb4);
		const static valueSet_t	CAND4_22_4567(0x78);
		const static valueSet_t	CAND4_23_1589(0x191);
		const static valueSet_t	CAND4_23_1679(0x161);
		const static valueSet_t	CAND4_23_2489(0x18a);
		const static valueSet_t	CAND4_23_2579(0x152);
		const static valueSet_t	CAND4_23_2678(0xe2);
		const static valueSet_t	CAND4_23_3479(0x14c);
		const static valueSet_t	CAND4_23_3569(0x134);
		const static valueSet_t	CAND4_23_3578(0xd4);
		const static valueSet_t	CAND4_23_4568(0xb8);
		const static valueSet_t	CAND4_24_1689(0x1a1);
		const static valueSet_t	CAND4_24_2589(0x192);
		const static valueSet_t	CAND4_24_2679(0x162);
		const static valueSet_t	CAND4_24_3489(0x18c);
		const static valueSet_t	CAND4_24_3579(0x154);
		const static valueSet_t	CAND4_24_3678(0xe4);
		const static valueSet_t	CAND4_24_4569(0x138);
		const static valueSet_t	CAND4_24_4578(0xd8);
		const static valueSet_t	CAND4_25_1789(0x1c1);
		const static valueSet_t	CAND4_25_2689(0x1a2);
		const static valueSet_t	CAND4_25_3589(0x194);
		const static valueSet_t	CAND4_25_3679(0x164);
		const static valueSet_t	CAND4_25_4579(0x158);
		const static valueSet_t	CAND4_25_4678(0xe8);
		const static valueSet_t	CAND4_26_2789(0x1c2);
		const static valueSet_t	CAND4_26_3689(0x1a4);
		const static valueSet_t	CAND4_26_4589(0x198);
		const static valueSet_t	CAND4_26_4679(0x168);
		const static valueSet_t	CAND4_26_5678(0xf0);
		const static valueSet_t	CAND4_27_3789(0x1c4);
		const static valueSet_t	CAND4_27_4689(0x1a8);
		const static valueSet_t	CAND4_27_5679(0x170);
		const static valueSet_t	CAND4_28_4789(0x1c8);
		const static valueSet_t	CAND4_28_5689(0x1b0);
		const static valueSet_t	CAND4_29_5789(0x1d0);
		const static valueSet_t	CAND4_30_6789(0x1e0);
		const static valueSet_t	CAND5_15_12345(0x1f);
		const static valueSet_t	CAND5_16_12346(0x2f);
		const static valueSet_t	CAND5_17_12347(0x4f);
		const static valueSet_t	CAND5_17_12356(0x37);
		const static valueSet_t	CAND5_18_12348(0x8f);
		const static valueSet_t	CAND5_18_12357(0x57);
		const static valueSet_t	CAND5_18_12456(0x3b);
		const static valueSet_t	CAND5_19_12349(0x10f);
		const static valueSet_t	CAND5_19_12358(0x97);
		const static valueSet_t	CAND5_19_12367(0x67);
		const static valueSet_t	CAND5_19_12457(0x5b);
		const static valueSet_t	CAND5_19_13456(0x3d);
		const static valueSet_t	CAND5_20_12359(0x117);
		const static valueSet_t	CAND5_20_12368(0xa7);
		const static valueSet_t	CAND5_20_12458(0x9b);
		const static valueSet_t	CAND5_20_12467(0x6b);
		const static valueSet_t	CAND5_20_13457(0x5d);
		const static valueSet_t	CAND5_20_23456(0x3e);
		const static valueSet_t	CAND5_21_12369(0x127);
		const static valueSet_t	CAND5_21_12378(0xc7);
		const static valueSet_t	CAND5_21_12459(0x11b);
		const static valueSet_t	CAND5_21_12468(0xab);
		const static valueSet_t	CAND5_21_12567(0x73);
		const static valueSet_t	CAND5_21_13458(0x9d);
		const static valueSet_t	CAND5_21_13467(0x6d);
		const static valueSet_t	CAND5_21_23457(0x5e);
		const static valueSet_t	CAND5_22_12379(0x147);
		const static valueSet_t	CAND5_22_12469(0x12b);
		const static valueSet_t	CAND5_22_12478(0xcb);
		const static valueSet_t	CAND5_22_12568(0xb3);
		const static valueSet_t	CAND5_22_13459(0x11d);
		const static valueSet_t	CAND5_22_13468(0xad);
		const static valueSet_t	CAND5_22_13567(0x75);
		const static valueSet_t	CAND5_22_23458(0x9e);
		const static valueSet_t	CAND5_22_23467(0x6e);
		const static valueSet_t	CAND5_23_12389(0x187);
		const static valueSet_t	CAND5_23_12479(0x14b);
		const static valueSet_t	CAND5_23_12569(0x133);
		const static valueSet_t	CAND5_23_12578(0xd3);
		const static valueSet_t	CAND5_23_13469(0x12d);
		const static valueSet_t	CAND5_23_13478(0xcd);
		const static valueSet_t	CAND5_23_13568(0xb5);
		const static valueSet_t	CAND5_23_14567(0x79);
		const static valueSet_t	CAND5_23_23459(0x11e);
		const static valueSet_t	CAND5_23_23468(0xae);
		const static valueSet_t	CAND5_23_23567(0x76);
		const static valueSet_t	CAND5_24_12489(0x18b);
		const static valueSet_t	CAND5_24_12579(0x153);
		const static valueSet_t	CAND5_24_12678(0xe3);
		const static valueSet_t	CAND5_24_13479(0x14d);
		const static valueSet_t	CAND5_24_13569(0x135);
		const static valueSet_t	CAND5_24_13578(0xd5);
		const static valueSet_t	CAND5_24_14568(0xb9);
		const static valueSet_t	CAND5_24_23469(0x12e);
		const static valueSet_t	CAND5_24_23478(0xce);
		const static valueSet_t	CAND5_24_23568(0xb6);
		const static valueSet_t	CAND5_24_24567(0x7a);
		const static valueSet_t	CAND5_25_12589(0x193);
		const static valueSet_t	CAND5_25_12679(0x163);
		const static valueSet_t	CAND5_25_13489(0x18d);
		const static valueSet_t	CAND5_25_13579(0x155);
		const static valueSet_t	CAND5_25_13678(0xe5);
		const static valueSet_t	CAND5_25_14569(0x139);
		const static valueSet_t	CAND5_25_14578(0xd9);
		const static valueSet_t	CAND5_25_23479(0x14e);
		const static valueSet_t	CAND5_25_23569(0x136);
		const static valueSet_t	CAND5_25_23578(0xd6);
		const static valueSet_t	CAND5_25_24568(0xba);
		const static valueSet_t	CAND5_25_34567(0x7c);
		const static valueSet_t	CAND5_26_12689(0x1a3);
		const static valueSet_t	CAND5_26_13589(0x195);
		const static valueSet_t	CAND5_26_13679(0x165);
		const static valueSet_t	CAND5_26_14579(0x159);
		const static valueSet_t	CAND5_26_14678(0xe9);
		const static valueSet_t	CAND5_26_23489(0x18e);
		const static valueSet_t	CAND5_26_23579(0x156);
		const static valueSet_t	CAND5_26_23678(0xe6);
		const static valueSet_t	CAND5_26_24569(0x13a);
		const static valueSet_t	CAND5_26_24578(0xda);
		const static valueSet_t	CAND5_26_34568(0xbc);
		const static valueSet_t	CAND5_27_12789(0x1c3);
		const static valueSet_t	CAND5_27_13689(0x1a5);
		const static valueSet_t	CAND5_27_14589(0x199);
		const static valueSet_t	CAND5_27_14679(0x169);
		const static valueSet_t	CAND5_27_15678(0xf1);
		const static valueSet_t	CAND5_27_23589(0x196);
		const static valueSet_t	CAND5_27_23679(0x166);
		const static valueSet_t	CAND5_27_24579(0x15a);
		const static valueSet_t	CAND5_27_24678(0xea);
		const static valueSet_t	CAND5_27_34569(0x13c);
		const static valueSet_t	CAND5_27_34578(0xdc);
		const static valueSet_t	CAND5_28_13789(0x1c5);
		const static valueSet_t	CAND5_28_14689(0x1a9);
		const static valueSet_t	CAND5_28_15679(0x171);
		const static valueSet_t	CAND5_28_23689(0x1a6);
		const static valueSet_t	CAND5_28_24589(0x19a);
		const static valueSet_t	CAND5_28_24679(0x16a);
		const static valueSet_t	CAND5_28_25678(0xf2);
		const static valueSet_t	CAND5_28_34579(0x15c);
		const static valueSet_t	CAND5_28_34678(0xec);
		const static valueSet_t	CAND5_29_14789(0x1c9);
		const static valueSet_t	CAND5_29_15689(0x1b1);
		const static valueSet_t	CAND5_29_23789(0x1c6);
		const static valueSet_t	CAND5_29_24689(0x1aa);
		const static valueSet_t	CAND5_29_25679(0x172);
		const static valueSet_t	CAND5_29_34589(0x19c);
		const static valueSet_t	CAND5_29_34679(0x16c);
		const static valueSet_t	CAND5_29_35678(0xf4);
		const static valueSet_t	CAND5_30_15789(0x1d1);
		const static valueSet_t	CAND5_30_24789(0x1ca);
		const static valueSet_t	CAND5_30_25689(0x1b2);
		const static valueSet_t	CAND5_30_34689(0x1ac);
		const static valueSet_t	CAND5_30_35679(0x174);
		const static valueSet_t	CAND5_30_45678(0xf8);
		const static valueSet_t	CAND5_31_16789(0x1e1);
		const static valueSet_t	CAND5_31_25789(0x1d2);
		const static valueSet_t	CAND5_31_34789(0x1cc);
		const static valueSet_t	CAND5_31_35689(0x1b4);
		const static valueSet_t	CAND5_31_45679(0x178);
		const static valueSet_t	CAND5_32_26789(0x1e2);
		const static valueSet_t	CAND5_32_35789(0x1d4);
		const static valueSet_t	CAND5_32_45689(0x1b8);
		const static valueSet_t	CAND5_33_36789(0x1e4);
		const static valueSet_t	CAND5_33_45789(0x1d8);
		const static valueSet_t	CAND5_34_46789(0x1e8);
		const static valueSet_t	CAND5_35_56789(0x1f0);
		const static valueSet_t	CAND6_21_123456(0x3f);
		const static valueSet_t	CAND6_22_123457(0x5f);
		const static valueSet_t	CAND6_23_123458(0x9f);
		const static valueSet_t	CAND6_23_123467(0x6f);
		const static valueSet_t	CAND6_24_123459(0x11f);
		const static valueSet_t	CAND6_24_123468(0xaf);
		const static valueSet_t	CAND6_24_123567(0x77);
		const static valueSet_t	CAND6_25_123469(0x12f);
		const static valueSet_t	CAND6_25_123478(0xcf);
		const static valueSet_t	CAND6_25_123568(0xb7);
		const static valueSet_t	CAND6_25_124567(0x7b);
		const static valueSet_t	CAND6_26_123479(0x14f);
		const static valueSet_t	CAND6_26_123569(0x137);
		const static valueSet_t	CAND6_26_123578(0xd7);
		const static valueSet_t	CAND6_26_124568(0xbb);
		const static valueSet_t	CAND6_26_134567(0x7d);
		const static valueSet_t	CAND6_27_123489(0x18f);
		const static valueSet_t	CAND6_27_123579(0x157);
		const static valueSet_t	CAND6_27_123678(0xe7);
		const static valueSet_t	CAND6_27_124569(0x13b);
		const static valueSet_t	CAND6_27_124578(0xdb);
		const static valueSet_t	CAND6_27_134568(0xbd);
		const static valueSet_t	CAND6_27_234567(0x7e);
		const static valueSet_t	CAND6_28_123589(0x197);
		const static valueSet_t	CAND6_28_123679(0x167);
		const static valueSet_t	CAND6_28_124579(0x15b);
		const static valueSet_t	CAND6_28_124678(0xeb);
		const static valueSet_t	CAND6_28_134569(0x13d);
		const static valueSet_t	CAND6_28_134578(0xdd);
		const static valueSet_t	CAND6_28_234568(0xbe);
		const static valueSet_t	CAND6_29_123689(0x1a7);
		const static valueSet_t	CAND6_29_124589(0x19b);
		const static valueSet_t	CAND6_29_124679(0x16b);
		const static valueSet_t	CAND6_29_125678(0xf3);
		const static valueSet_t	CAND6_29_134579(0x15d);
		const static valueSet_t	CAND6_29_134678(0xed);
		const static valueSet_t	CAND6_29_234569(0x13e);
		const static valueSet_t	CAND6_29_234578(0xde);
		const static valueSet_t	CAND6_30_123789(0x1c7);
		const static valueSet_t	CAND6_30_124689(0x1ab);
		const static valueSet_t	CAND6_30_125679(0x173);
		const static valueSet_t	CAND6_30_134589(0x19d);
		const static valueSet_t	CAND6_30_134679(0x16d);
		const static valueSet_t	CAND6_30_135678(0xf5);
		const static valueSet_t	CAND6_30_234579(0x15e);
		const static valueSet_t	CAND6_30_234678(0xee);
		const static valueSet_t	CAND6_31_124789(0x1cb);
		const static valueSet_t	CAND6_31_125689(0x1b3);
		const static valueSet_t	CAND6_31_134689(0x1ad);
		const static valueSet_t	CAND6_31_135679(0x175);
		const static valueSet_t	CAND6_31_145678(0xf9);
		const static valueSet_t	CAND6_31_234589(0x19e);
		const static valueSet_t	CAND6_31_234679(0x16e);
		const static valueSet_t	CAND6_31_235678(0xf6);
		const static valueSet_t	CAND6_32_125789(0x1d3);
		const static valueSet_t	CAND6_32_134789(0x1cd);
		const static valueSet_t	CAND6_32_135689(0x1b5);
		const static valueSet_t	CAND6_32_145679(0x179);
		const static valueSet_t	CAND6_32_234689(0x1ae);
		const static valueSet_t	CAND6_32_235679(0x176);
		const static valueSet_t	CAND6_32_245678(0xfa);
		const static valueSet_t	CAND6_33_126789(0x1e3);
		const static valueSet_t	CAND6_33_135789(0x1d5);
		const static valueSet_t	CAND6_33_145689(0x1b9);
		const static valueSet_t	CAND6_33_234789(0x1ce);
		const static valueSet_t	CAND6_33_235689(0x1b6);
		const static valueSet_t	CAND6_33_245679(0x17a);
		const static valueSet_t	CAND6_33_345678(0xfc);
		const static valueSet_t	CAND6_34_136789(0x1e5);
		const static valueSet_t	CAND6_34_145789(0x1d9);
		const static valueSet_t	CAND6_34_235789(0x1d6);
		const static valueSet_t	CAND6_34_245689(0x1ba);
		const static valueSet_t	CAND6_34_345679(0x17c);
		const static valueSet_t	CAND6_35_146789(0x1e9);
		const static valueSet_t	CAND6_35_236789(0x1e6);
		const static valueSet_t	CAND6_35_245789(0x1da);
		const static valueSet_t	CAND6_35_345689(0x1bc);
		const static valueSet_t	CAND6_36_156789(0x1f1);
		const static valueSet_t	CAND6_36_246789(0x1ea);
		const static valueSet_t	CAND6_36_345789(0x1dc);
		const static valueSet_t	CAND6_37_256789(0x1f2);
		const static valueSet_t	CAND6_37_346789(0x1ec);
		const static valueSet_t	CAND6_38_356789(0x1f4);
		const static valueSet_t	CAND6_39_456789(0x1f8);
		const static valueSet_t	CAND7_28_1234567(0x7f);
		const static valueSet_t	CAND7_29_1234568(0xbf);
		const static valueSet_t	CAND7_30_1234569(0x13f);
		const static valueSet_t	CAND7_30_1234578(0xdf);
		const static valueSet_t	CAND7_31_1234579(0x15f);
		const static valueSet_t	CAND7_31_1234678(0xef);
		const static valueSet_t	CAND7_32_1234589(0x19f);
		const static valueSet_t	CAND7_32_1234679(0x16f);
		const static valueSet_t	CAND7_32_1235678(0xf7);
		const static valueSet_t	CAND7_33_1234689(0x1af);
		const static valueSet_t	CAND7_33_1235679(0x177);
		const static valueSet_t	CAND7_33_1245678(0xfb);
		const static valueSet_t	CAND7_34_1234789(0x1cf);
		const static valueSet_t	CAND7_34_1235689(0x1b7);
		const static valueSet_t	CAND7_34_1245679(0x17b);
		const static valueSet_t	CAND7_34_1345678(0xfd);
		const static valueSet_t	CAND7_35_1235789(0x1d7);
		const static valueSet_t	CAND7_35_1245689(0x1bb);
		const static valueSet_t	CAND7_35_1345679(0x17d);
		const static valueSet_t	CAND7_35_2345678(0xfe);
		const static valueSet_t	CAND7_36_1236789(0x1e7);
		const static valueSet_t	CAND7_36_1245789(0x1db);
		const static valueSet_t	CAND7_36_1345689(0x1bd);
		const static valueSet_t	CAND7_36_2345679(0x17e);
		const static valueSet_t	CAND7_37_1246789(0x1eb);
		const static valueSet_t	CAND7_37_1345789(0x1dd);
		const static valueSet_t	CAND7_37_2345689(0x1be);
		const static valueSet_t	CAND7_38_1256789(0x1f3);
		const static valueSet_t	CAND7_38_1346789(0x1ed);
		const static valueSet_t	CAND7_38_2345789(0x1de);
		const static valueSet_t	CAND7_39_1356789(0x1f5);
		const static valueSet_t	CAND7_39_2346789(0x1ee);
		const static valueSet_t	CAND7_40_1456789(0x1f9);
		const static valueSet_t	CAND7_40_2356789(0x1f6);
		const static valueSet_t	CAND7_41_2456789(0x1fa);
		const static valueSet_t	CAND7_42_3456789(0x1fc);
		const static valueSet_t	CAND8_36_12345678(0xff);
		const static valueSet_t	CAND8_37_12345679(0x17f);
		const static valueSet_t	CAND8_38_12345689(0x1bf);
		const static valueSet_t	CAND8_39_12345789(0x1df);
		const static valueSet_t	CAND8_40_12346789(0x1ef);
		const static valueSet_t	CAND8_41_12356789(0x1f7);
		const static valueSet_t	CAND8_42_12456789(0x1fb);
		const static valueSet_t	CAND8_43_13456789(0x1fd);
		const static valueSet_t	CAND8_44_23456789(0x1fe);
		const static valueSet_t	CAND9_45_123456789(0x1ff);

		switch(cCell) {
		case 2:
			switch(sum) {
			case 3:
				candSet.push_back(&CAND2_3_12);
				break;
			case 4:
				candSet.push_back(&CAND2_4_13);
				break;
			case 5:
				candSet.push_back(&CAND2_5_14);
				candSet.push_back(&CAND2_5_23);
				break;
			case 6:
				candSet.push_back(&CAND2_6_15);
				candSet.push_back(&CAND2_6_24);
				break;
			case 7:
				candSet.push_back(&CAND2_7_16);
				candSet.push_back(&CAND2_7_25);
				candSet.push_back(&CAND2_7_34);
				break;
			case 8:
				candSet.push_back(&CAND2_8_17);
				candSet.push_back(&CAND2_8_26);
				candSet.push_back(&CAND2_8_35);
				break;
			case 9:
				candSet.push_back(&CAND2_9_18);
				candSet.push_back(&CAND2_9_27);
				candSet.push_back(&CAND2_9_36);
				candSet.push_back(&CAND2_9_45);
				break;
			case 10:
				candSet.push_back(&CAND2_10_19);
				candSet.push_back(&CAND2_10_28);
				candSet.push_back(&CAND2_10_37);
				candSet.push_back(&CAND2_10_46);
				break;
			case 11:
				candSet.push_back(&CAND2_11_29);
				candSet.push_back(&CAND2_11_38);
				candSet.push_back(&CAND2_11_47);
				candSet.push_back(&CAND2_11_56);
				break;
			case 12:
				candSet.push_back(&CAND2_12_39);
				candSet.push_back(&CAND2_12_48);
				candSet.push_back(&CAND2_12_57);
				break;
			case 13:
				candSet.push_back(&CAND2_13_49);
				candSet.push_back(&CAND2_13_58);
				candSet.push_back(&CAND2_13_67);
				break;
			case 14:
				candSet.push_back(&CAND2_14_59);
				candSet.push_back(&CAND2_14_68);
				break;
			case 15:
				candSet.push_back(&CAND2_15_69);
				candSet.push_back(&CAND2_15_78);
				break;
			case 16:
				candSet.push_back(&CAND2_16_79);
				break;
			case 17:
				candSet.push_back(&CAND2_17_89);
				break;
			default:
				throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::SUM);
			}
			break;
		case 3:
			switch(sum) {
			case 6:
				candSet.push_back(&CAND3_6_123);
				break;
			case 7:
				candSet.push_back(&CAND3_7_124);
				break;
			case 8:
				candSet.push_back(&CAND3_8_125);
				candSet.push_back(&CAND3_8_134);
				break;
			case 9:
				candSet.push_back(&CAND3_9_126);
				candSet.push_back(&CAND3_9_135);
				candSet.push_back(&CAND3_9_234);
				break;
			case 10:
				candSet.push_back(&CAND3_10_127);
				candSet.push_back(&CAND3_10_136);
				candSet.push_back(&CAND3_10_145);
				candSet.push_back(&CAND3_10_235);
				break;
			case 11:
				candSet.push_back(&CAND3_11_128);
				candSet.push_back(&CAND3_11_137);
				candSet.push_back(&CAND3_11_146);
				candSet.push_back(&CAND3_11_236);
				candSet.push_back(&CAND3_11_245);
				break;
			case 12:
				candSet.push_back(&CAND3_12_129);
				candSet.push_back(&CAND3_12_138);
				candSet.push_back(&CAND3_12_147);
				candSet.push_back(&CAND3_12_156);
				candSet.push_back(&CAND3_12_237);
				candSet.push_back(&CAND3_12_246);
				candSet.push_back(&CAND3_12_345);
				break;
			case 13:
				candSet.push_back(&CAND3_13_139);
				candSet.push_back(&CAND3_13_148);
				candSet.push_back(&CAND3_13_157);
				candSet.push_back(&CAND3_13_238);
				candSet.push_back(&CAND3_13_247);
				candSet.push_back(&CAND3_13_256);
				candSet.push_back(&CAND3_13_346);
				break;
			case 14:
				candSet.push_back(&CAND3_14_149);
				candSet.push_back(&CAND3_14_158);
				candSet.push_back(&CAND3_14_167);
				candSet.push_back(&CAND3_14_239);
				candSet.push_back(&CAND3_14_248);
				candSet.push_back(&CAND3_14_257);
				candSet.push_back(&CAND3_14_347);
				candSet.push_back(&CAND3_14_356);
				break;
			case 15:
				candSet.push_back(&CAND3_15_159);
				candSet.push_back(&CAND3_15_168);
				candSet.push_back(&CAND3_15_249);
				candSet.push_back(&CAND3_15_258);
				candSet.push_back(&CAND3_15_267);
				candSet.push_back(&CAND3_15_348);
				candSet.push_back(&CAND3_15_357);
				candSet.push_back(&CAND3_15_456);
				break;
			case 16:
				candSet.push_back(&CAND3_16_169);
				candSet.push_back(&CAND3_16_178);
				candSet.push_back(&CAND3_16_259);
				candSet.push_back(&CAND3_16_268);
				candSet.push_back(&CAND3_16_349);
				candSet.push_back(&CAND3_16_358);
				candSet.push_back(&CAND3_16_367);
				candSet.push_back(&CAND3_16_457);
				break;
			case 17:
				candSet.push_back(&CAND3_17_179);
				candSet.push_back(&CAND3_17_269);
				candSet.push_back(&CAND3_17_278);
				candSet.push_back(&CAND3_17_359);
				candSet.push_back(&CAND3_17_368);
				candSet.push_back(&CAND3_17_458);
				candSet.push_back(&CAND3_17_467);
				break;
			case 18:
				candSet.push_back(&CAND3_18_189);
				candSet.push_back(&CAND3_18_279);
				candSet.push_back(&CAND3_18_369);
				candSet.push_back(&CAND3_18_378);
				candSet.push_back(&CAND3_18_459);
				candSet.push_back(&CAND3_18_468);
				candSet.push_back(&CAND3_18_567);
				break;
			case 19:
				candSet.push_back(&CAND3_19_289);
				candSet.push_back(&CAND3_19_379);
				candSet.push_back(&CAND3_19_469);
				candSet.push_back(&CAND3_19_478);
				candSet.push_back(&CAND3_19_568);
				break;
			case 20:
				candSet.push_back(&CAND3_20_389);
				candSet.push_back(&CAND3_20_479);
				candSet.push_back(&CAND3_20_569);
				candSet.push_back(&CAND3_20_578);
				break;
			case 21:
				candSet.push_back(&CAND3_21_489);
				candSet.push_back(&CAND3_21_579);
				candSet.push_back(&CAND3_21_678);
				break;
			case 22:
				candSet.push_back(&CAND3_22_589);
				candSet.push_back(&CAND3_22_679);
				break;
			case 23:
				candSet.push_back(&CAND3_23_689);
				break;
			case 24:
				candSet.push_back(&CAND3_24_789);
				break;
			default:
				throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::SUM);
			}
			break;
		case 4:
			switch(sum) {
			case 10:
				candSet.push_back(&CAND4_10_1234);
				break;
			case 11:
				candSet.push_back(&CAND4_11_1235);
				break;
			case 12:
				candSet.push_back(&CAND4_12_1236);
				candSet.push_back(&CAND4_12_1245);
				break;
			case 13:
				candSet.push_back(&CAND4_13_1237);
				candSet.push_back(&CAND4_13_1246);
				candSet.push_back(&CAND4_13_1345);
				break;
			case 14:
				candSet.push_back(&CAND4_14_1238);
				candSet.push_back(&CAND4_14_1247);
				candSet.push_back(&CAND4_14_1256);
				candSet.push_back(&CAND4_14_1346);
				candSet.push_back(&CAND4_14_2345);
				break;
			case 15:
				candSet.push_back(&CAND4_15_1239);
				candSet.push_back(&CAND4_15_1248);
				candSet.push_back(&CAND4_15_1257);
				candSet.push_back(&CAND4_15_1347);
				candSet.push_back(&CAND4_15_1356);
				candSet.push_back(&CAND4_15_2346);
				break;
			case 16:
				candSet.push_back(&CAND4_16_1249);
				candSet.push_back(&CAND4_16_1258);
				candSet.push_back(&CAND4_16_1267);
				candSet.push_back(&CAND4_16_1348);
				candSet.push_back(&CAND4_16_1357);
				candSet.push_back(&CAND4_16_1456);
				candSet.push_back(&CAND4_16_2347);
				candSet.push_back(&CAND4_16_2356);
				break;
			case 17:
				candSet.push_back(&CAND4_17_1259);
				candSet.push_back(&CAND4_17_1268);
				candSet.push_back(&CAND4_17_1349);
				candSet.push_back(&CAND4_17_1358);
				candSet.push_back(&CAND4_17_1367);
				candSet.push_back(&CAND4_17_1457);
				candSet.push_back(&CAND4_17_2348);
				candSet.push_back(&CAND4_17_2357);
				candSet.push_back(&CAND4_17_2456);
				break;
			case 18:
				candSet.push_back(&CAND4_18_1269);
				candSet.push_back(&CAND4_18_1278);
				candSet.push_back(&CAND4_18_1359);
				candSet.push_back(&CAND4_18_1368);
				candSet.push_back(&CAND4_18_1458);
				candSet.push_back(&CAND4_18_1467);
				candSet.push_back(&CAND4_18_2349);
				candSet.push_back(&CAND4_18_2358);
				candSet.push_back(&CAND4_18_2367);
				candSet.push_back(&CAND4_18_2457);
				candSet.push_back(&CAND4_18_3456);
				break;
			case 19:
				candSet.push_back(&CAND4_19_1279);
				candSet.push_back(&CAND4_19_1369);
				candSet.push_back(&CAND4_19_1378);
				candSet.push_back(&CAND4_19_1459);
				candSet.push_back(&CAND4_19_1468);
				candSet.push_back(&CAND4_19_1567);
				candSet.push_back(&CAND4_19_2359);
				candSet.push_back(&CAND4_19_2368);
				candSet.push_back(&CAND4_19_2458);
				candSet.push_back(&CAND4_19_2467);
				candSet.push_back(&CAND4_19_3457);
				break;
			case 20:
				candSet.push_back(&CAND4_20_1289);
				candSet.push_back(&CAND4_20_1379);
				candSet.push_back(&CAND4_20_1469);
				candSet.push_back(&CAND4_20_1478);
				candSet.push_back(&CAND4_20_1568);
				candSet.push_back(&CAND4_20_2369);
				candSet.push_back(&CAND4_20_2378);
				candSet.push_back(&CAND4_20_2459);
				candSet.push_back(&CAND4_20_2468);
				candSet.push_back(&CAND4_20_2567);
				candSet.push_back(&CAND4_20_3458);
				candSet.push_back(&CAND4_20_3467);
				break;
			case 21:
				candSet.push_back(&CAND4_21_1389);
				candSet.push_back(&CAND4_21_1479);
				candSet.push_back(&CAND4_21_1569);
				candSet.push_back(&CAND4_21_1578);
				candSet.push_back(&CAND4_21_2379);
				candSet.push_back(&CAND4_21_2469);
				candSet.push_back(&CAND4_21_2478);
				candSet.push_back(&CAND4_21_2568);
				candSet.push_back(&CAND4_21_3459);
				candSet.push_back(&CAND4_21_3468);
				candSet.push_back(&CAND4_21_3567);
				break;
			case 22:
				candSet.push_back(&CAND4_22_1489);
				candSet.push_back(&CAND4_22_1579);
				candSet.push_back(&CAND4_22_1678);
				candSet.push_back(&CAND4_22_2389);
				candSet.push_back(&CAND4_22_2479);
				candSet.push_back(&CAND4_22_2569);
				candSet.push_back(&CAND4_22_2578);
				candSet.push_back(&CAND4_22_3469);
				candSet.push_back(&CAND4_22_3478);
				candSet.push_back(&CAND4_22_3568);
				candSet.push_back(&CAND4_22_4567);
				break;
			case 23:
				candSet.push_back(&CAND4_23_1589);
				candSet.push_back(&CAND4_23_1679);
				candSet.push_back(&CAND4_23_2489);
				candSet.push_back(&CAND4_23_2579);
				candSet.push_back(&CAND4_23_2678);
				candSet.push_back(&CAND4_23_3479);
				candSet.push_back(&CAND4_23_3569);
				candSet.push_back(&CAND4_23_3578);
				candSet.push_back(&CAND4_23_4568);
				break;
			case 24:
				candSet.push_back(&CAND4_24_1689);
				candSet.push_back(&CAND4_24_2589);
				candSet.push_back(&CAND4_24_2679);
				candSet.push_back(&CAND4_24_3489);
				candSet.push_back(&CAND4_24_3579);
				candSet.push_back(&CAND4_24_3678);
				candSet.push_back(&CAND4_24_4569);
				candSet.push_back(&CAND4_24_4578);
				break;
			case 25:
				candSet.push_back(&CAND4_25_1789);
				candSet.push_back(&CAND4_25_2689);
				candSet.push_back(&CAND4_25_3589);
				candSet.push_back(&CAND4_25_3679);
				candSet.push_back(&CAND4_25_4579);
				candSet.push_back(&CAND4_25_4678);
				break;
			case 26:
				candSet.push_back(&CAND4_26_2789);
				candSet.push_back(&CAND4_26_3689);
				candSet.push_back(&CAND4_26_4589);
				candSet.push_back(&CAND4_26_4679);
				candSet.push_back(&CAND4_26_5678);
				break;
			case 27:
				candSet.push_back(&CAND4_27_3789);
				candSet.push_back(&CAND4_27_4689);
				candSet.push_back(&CAND4_27_5679);
				break;
			case 28:
				candSet.push_back(&CAND4_28_4789);
				candSet.push_back(&CAND4_28_5689);
				break;
			case 29:
				candSet.push_back(&CAND4_29_5789);
				break;
			case 30:
				candSet.push_back(&CAND4_30_6789);
				break;
			default:
				throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::SUM);
			}
			break;
		case 5:
			switch(sum) {
			case 15:
				candSet.push_back(&CAND5_15_12345);
				break;
			case 16:
				candSet.push_back(&CAND5_16_12346);
				break;
			case 17:
				candSet.push_back(&CAND5_17_12347);
				candSet.push_back(&CAND5_17_12356);
				break;
			case 18:
				candSet.push_back(&CAND5_18_12348);
				candSet.push_back(&CAND5_18_12357);
				candSet.push_back(&CAND5_18_12456);
				break;
			case 19:
				candSet.push_back(&CAND5_19_12349);
				candSet.push_back(&CAND5_19_12358);
				candSet.push_back(&CAND5_19_12367);
				candSet.push_back(&CAND5_19_12457);
				candSet.push_back(&CAND5_19_13456);
				break;
			case 20:
				candSet.push_back(&CAND5_20_12359);
				candSet.push_back(&CAND5_20_12368);
				candSet.push_back(&CAND5_20_12458);
				candSet.push_back(&CAND5_20_12467);
				candSet.push_back(&CAND5_20_13457);
				candSet.push_back(&CAND5_20_23456);
				break;
			case 21:
				candSet.push_back(&CAND5_21_12369);
				candSet.push_back(&CAND5_21_12378);
				candSet.push_back(&CAND5_21_12459);
				candSet.push_back(&CAND5_21_12468);
				candSet.push_back(&CAND5_21_12567);
				candSet.push_back(&CAND5_21_13458);
				candSet.push_back(&CAND5_21_13467);
				candSet.push_back(&CAND5_21_23457);
				break;
			case 22:
				candSet.push_back(&CAND5_22_12379);
				candSet.push_back(&CAND5_22_12469);
				candSet.push_back(&CAND5_22_12478);
				candSet.push_back(&CAND5_22_12568);
				candSet.push_back(&CAND5_22_13459);
				candSet.push_back(&CAND5_22_13468);
				candSet.push_back(&CAND5_22_13567);
				candSet.push_back(&CAND5_22_23458);
				candSet.push_back(&CAND5_22_23467);
				break;
			case 23:
				candSet.push_back(&CAND5_23_12389);
				candSet.push_back(&CAND5_23_12479);
				candSet.push_back(&CAND5_23_12569);
				candSet.push_back(&CAND5_23_12578);
				candSet.push_back(&CAND5_23_13469);
				candSet.push_back(&CAND5_23_13478);
				candSet.push_back(&CAND5_23_13568);
				candSet.push_back(&CAND5_23_14567);
				candSet.push_back(&CAND5_23_23459);
				candSet.push_back(&CAND5_23_23468);
				candSet.push_back(&CAND5_23_23567);
				break;
			case 24:
				candSet.push_back(&CAND5_24_12489);
				candSet.push_back(&CAND5_24_12579);
				candSet.push_back(&CAND5_24_12678);
				candSet.push_back(&CAND5_24_13479);
				candSet.push_back(&CAND5_24_13569);
				candSet.push_back(&CAND5_24_13578);
				candSet.push_back(&CAND5_24_14568);
				candSet.push_back(&CAND5_24_23469);
				candSet.push_back(&CAND5_24_23478);
				candSet.push_back(&CAND5_24_23568);
				candSet.push_back(&CAND5_24_24567);
				break;
			case 25:
				candSet.push_back(&CAND5_25_12589);
				candSet.push_back(&CAND5_25_12679);
				candSet.push_back(&CAND5_25_13489);
				candSet.push_back(&CAND5_25_13579);
				candSet.push_back(&CAND5_25_13678);
				candSet.push_back(&CAND5_25_14569);
				candSet.push_back(&CAND5_25_14578);
				candSet.push_back(&CAND5_25_23479);
				candSet.push_back(&CAND5_25_23569);
				candSet.push_back(&CAND5_25_23578);
				candSet.push_back(&CAND5_25_24568);
				candSet.push_back(&CAND5_25_34567);
				break;
			case 26:
				candSet.push_back(&CAND5_26_12689);
				candSet.push_back(&CAND5_26_13589);
				candSet.push_back(&CAND5_26_13679);
				candSet.push_back(&CAND5_26_14579);
				candSet.push_back(&CAND5_26_14678);
				candSet.push_back(&CAND5_26_23489);
				candSet.push_back(&CAND5_26_23579);
				candSet.push_back(&CAND5_26_23678);
				candSet.push_back(&CAND5_26_24569);
				candSet.push_back(&CAND5_26_24578);
				candSet.push_back(&CAND5_26_34568);
				break;
			case 27:
				candSet.push_back(&CAND5_27_12789);
				candSet.push_back(&CAND5_27_13689);
				candSet.push_back(&CAND5_27_14589);
				candSet.push_back(&CAND5_27_14679);
				candSet.push_back(&CAND5_27_15678);
				candSet.push_back(&CAND5_27_23589);
				candSet.push_back(&CAND5_27_23679);
				candSet.push_back(&CAND5_27_24579);
				candSet.push_back(&CAND5_27_24678);
				candSet.push_back(&CAND5_27_34569);
				candSet.push_back(&CAND5_27_34578);
				break;
			case 28:
				candSet.push_back(&CAND5_28_13789);
				candSet.push_back(&CAND5_28_14689);
				candSet.push_back(&CAND5_28_15679);
				candSet.push_back(&CAND5_28_23689);
				candSet.push_back(&CAND5_28_24589);
				candSet.push_back(&CAND5_28_24679);
				candSet.push_back(&CAND5_28_25678);
				candSet.push_back(&CAND5_28_34579);
				candSet.push_back(&CAND5_28_34678);
				break;
			case 29:
				candSet.push_back(&CAND5_29_14789);
				candSet.push_back(&CAND5_29_15689);
				candSet.push_back(&CAND5_29_23789);
				candSet.push_back(&CAND5_29_24689);
				candSet.push_back(&CAND5_29_25679);
				candSet.push_back(&CAND5_29_34589);
				candSet.push_back(&CAND5_29_34679);
				candSet.push_back(&CAND5_29_35678);
				break;
			case 30:
				candSet.push_back(&CAND5_30_15789);
				candSet.push_back(&CAND5_30_24789);
				candSet.push_back(&CAND5_30_25689);
				candSet.push_back(&CAND5_30_34689);
				candSet.push_back(&CAND5_30_35679);
				candSet.push_back(&CAND5_30_45678);
				break;
			case 31:
				candSet.push_back(&CAND5_31_16789);
				candSet.push_back(&CAND5_31_25789);
				candSet.push_back(&CAND5_31_34789);
				candSet.push_back(&CAND5_31_35689);
				candSet.push_back(&CAND5_31_45679);
				break;
			case 32:
				candSet.push_back(&CAND5_32_26789);
				candSet.push_back(&CAND5_32_35789);
				candSet.push_back(&CAND5_32_45689);
				break;
			case 33:
				candSet.push_back(&CAND5_33_36789);
				candSet.push_back(&CAND5_33_45789);
				break;
			case 34:
				candSet.push_back(&CAND5_34_46789);
				break;
			case 35:
				candSet.push_back(&CAND5_35_56789);
				break;
			default:
				throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::SUM);
			}
			break;
		case 6:
			switch(sum) {
			case 21:
				candSet.push_back(&CAND6_21_123456);
				break;
			case 22:
				candSet.push_back(&CAND6_22_123457);
				break;
			case 23:
				candSet.push_back(&CAND6_23_123458);
				candSet.push_back(&CAND6_23_123467);
				break;
			case 24:
				candSet.push_back(&CAND6_24_123459);
				candSet.push_back(&CAND6_24_123468);
				candSet.push_back(&CAND6_24_123567);
				break;
			case 25:
				candSet.push_back(&CAND6_25_123469);
				candSet.push_back(&CAND6_25_123478);
				candSet.push_back(&CAND6_25_123568);
				candSet.push_back(&CAND6_25_124567);
				break;
			case 26:
				candSet.push_back(&CAND6_26_123479);
				candSet.push_back(&CAND6_26_123569);
				candSet.push_back(&CAND6_26_123578);
				candSet.push_back(&CAND6_26_124568);
				candSet.push_back(&CAND6_26_134567);
				break;
			case 27:
				candSet.push_back(&CAND6_27_123489);
				candSet.push_back(&CAND6_27_123579);
				candSet.push_back(&CAND6_27_123678);
				candSet.push_back(&CAND6_27_124569);
				candSet.push_back(&CAND6_27_124578);
				candSet.push_back(&CAND6_27_134568);
				candSet.push_back(&CAND6_27_234567);
				break;
			case 28:
				candSet.push_back(&CAND6_28_123589);
				candSet.push_back(&CAND6_28_123679);
				candSet.push_back(&CAND6_28_124579);
				candSet.push_back(&CAND6_28_124678);
				candSet.push_back(&CAND6_28_134569);
				candSet.push_back(&CAND6_28_134578);
				candSet.push_back(&CAND6_28_234568);
				break;
			case 29:
				candSet.push_back(&CAND6_29_123689);
				candSet.push_back(&CAND6_29_124589);
				candSet.push_back(&CAND6_29_124679);
				candSet.push_back(&CAND6_29_125678);
				candSet.push_back(&CAND6_29_134579);
				candSet.push_back(&CAND6_29_134678);
				candSet.push_back(&CAND6_29_234569);
				candSet.push_back(&CAND6_29_234578);
				break;
			case 30:
				candSet.push_back(&CAND6_30_123789);
				candSet.push_back(&CAND6_30_124689);
				candSet.push_back(&CAND6_30_125679);
				candSet.push_back(&CAND6_30_134589);
				candSet.push_back(&CAND6_30_134679);
				candSet.push_back(&CAND6_30_135678);
				candSet.push_back(&CAND6_30_234579);
				candSet.push_back(&CAND6_30_234678);
				break;
			case 31:
				candSet.push_back(&CAND6_31_124789);
				candSet.push_back(&CAND6_31_125689);
				candSet.push_back(&CAND6_31_134689);
				candSet.push_back(&CAND6_31_135679);
				candSet.push_back(&CAND6_31_145678);
				candSet.push_back(&CAND6_31_234589);
				candSet.push_back(&CAND6_31_234679);
				candSet.push_back(&CAND6_31_235678);
				break;
			case 32:
				candSet.push_back(&CAND6_32_125789);
				candSet.push_back(&CAND6_32_134789);
				candSet.push_back(&CAND6_32_135689);
				candSet.push_back(&CAND6_32_145679);
				candSet.push_back(&CAND6_32_234689);
				candSet.push_back(&CAND6_32_235679);
				candSet.push_back(&CAND6_32_245678);
				break;
			case 33:
				candSet.push_back(&CAND6_33_126789);
				candSet.push_back(&CAND6_33_135789);
				candSet.push_back(&CAND6_33_145689);
				candSet.push_back(&CAND6_33_234789);
				candSet.push_back(&CAND6_33_235689);
				candSet.push_back(&CAND6_33_245679);
				candSet.push_back(&CAND6_33_345678);
				break;
			case 34:
				candSet.push_back(&CAND6_34_136789);
				candSet.push_back(&CAND6_34_145789);
				candSet.push_back(&CAND6_34_235789);
				candSet.push_back(&CAND6_34_245689);
				candSet.push_back(&CAND6_34_345679);
				break;
			case 35:
				candSet.push_back(&CAND6_35_146789);
				candSet.push_back(&CAND6_35_236789);
				candSet.push_back(&CAND6_35_245789);
				candSet.push_back(&CAND6_35_345689);
				break;
			case 36:
				candSet.push_back(&CAND6_36_156789);
				candSet.push_back(&CAND6_36_246789);
				candSet.push_back(&CAND6_36_345789);
				break;
			case 37:
				candSet.push_back(&CAND6_37_256789);
				candSet.push_back(&CAND6_37_346789);
				break;
			case 38:
				candSet.push_back(&CAND6_38_356789);
				break;
			case 39:
				candSet.push_back(&CAND6_39_456789);
				break;
			default:
				throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::SUM);
			}
			break;
		case 7:
			switch(sum) {
			case 28:
				candSet.push_back(&CAND7_28_1234567);
				break;
			case 29:
				candSet.push_back(&CAND7_29_1234568);
				break;
			case 30:
				candSet.push_back(&CAND7_30_1234569);
				candSet.push_back(&CAND7_30_1234578);
				break;
			case 31:
				candSet.push_back(&CAND7_31_1234579);
				candSet.push_back(&CAND7_31_1234678);
				break;
			case 32:
				candSet.push_back(&CAND7_32_1234589);
				candSet.push_back(&CAND7_32_1234679);
				candSet.push_back(&CAND7_32_1235678);
				break;
			case 33:
				candSet.push_back(&CAND7_33_1234689);
				candSet.push_back(&CAND7_33_1235679);
				candSet.push_back(&CAND7_33_1245678);
				break;
			case 34:
				candSet.push_back(&CAND7_34_1234789);
				candSet.push_back(&CAND7_34_1235689);
				candSet.push_back(&CAND7_34_1245679);
				candSet.push_back(&CAND7_34_1345678);
				break;
			case 35:
				candSet.push_back(&CAND7_35_1235789);
				candSet.push_back(&CAND7_35_1245689);
				candSet.push_back(&CAND7_35_1345679);
				candSet.push_back(&CAND7_35_2345678);
				break;
			case 36:
				candSet.push_back(&CAND7_36_1236789);
				candSet.push_back(&CAND7_36_1245789);
				candSet.push_back(&CAND7_36_1345689);
				candSet.push_back(&CAND7_36_2345679);
				break;
			case 37:
				candSet.push_back(&CAND7_37_1246789);
				candSet.push_back(&CAND7_37_1345789);
				candSet.push_back(&CAND7_37_2345689);
				break;
			case 38:
				candSet.push_back(&CAND7_38_1256789);
				candSet.push_back(&CAND7_38_1346789);
				candSet.push_back(&CAND7_38_2345789);
				break;
			case 39:
				candSet.push_back(&CAND7_39_1356789);
				candSet.push_back(&CAND7_39_2346789);
				break;
			case 40:
				candSet.push_back(&CAND7_40_1456789);
				candSet.push_back(&CAND7_40_2356789);
				break;
			case 41:
				candSet.push_back(&CAND7_41_2456789);
				break;
			case 42:
				candSet.push_back(&CAND7_42_3456789);
				break;
			default:
				throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::SUM);
			}
			break;
		case 8:
			switch(sum) {
			case 36:
				candSet.push_back(&CAND8_36_12345678);
				break;
			case 37:
				candSet.push_back(&CAND8_37_12345679);
				break;
			case 38:
				candSet.push_back(&CAND8_38_12345689);
				break;
			case 39:
				candSet.push_back(&CAND8_39_12345789);
				break;
			case 40:
				candSet.push_back(&CAND8_40_12346789);
				break;
			case 41:
				candSet.push_back(&CAND8_41_12356789);
				break;
			case 42:
				candSet.push_back(&CAND8_42_12456789);
				break;
			case 43:
				candSet.push_back(&CAND8_43_13456789);
				break;
			case 44:
				candSet.push_back(&CAND8_44_23456789);
				break;
			default:
				throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::SUM);
			}
			break;
		case 9:
			switch(sum) {
			case 45:
				candSet.push_back(&CAND9_45_123456789);
                break;
			default:
				throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::SUM);
			}
			break;
		default:
            throw slvExceptionInvlideBoardData_t(slvExceptionInvlideBoardData_t::NUM_CELL);
		}
	}

	const size_t BitCounts[] = {
		0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,    // 0 - 15
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,    // 16 - 31
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,    // 32 - 47
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,    // 48 - 63
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,    // 64 - 79
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,    // 80 - 95
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,    // 96 - 111
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,    // 112 - 127
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,    // 128 - 143
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,    // 144 - 159
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,    // 160 - 175
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,    // 176 - 191
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,    // 192 - 207
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,    // 208 - 223
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,    // 224 - 239
		4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,    // 240 - 255
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,    // 256 - 271
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,    // 272 - 287
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,    // 288 - 303
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,    // 304 - 319
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,    // 320 - 335
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,    // 336 - 351
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,    // 352 - 367
		4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,    // 368 - 383
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,    // 384 - 399
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,    // 400 - 415
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,    // 416 - 431
		4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,    // 432 - 447
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,    // 448 - 463
		4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,    // 464 - 479
		4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,    // 480 - 495
		5, 6, 6, 7, 6, 7, 7, 8, 6, 7, 7, 8, 7, 8, 8, 9,    // 496 - 511
	};

	inline size_t getBitCount(int bit) {
#if defined(_DEBUG)
		if(bit < 0 || 511 < bit)
			throw slvEceptionSolverBug_t(__LINE__, __FILE__);
#endif
		return BitCounts[bit];
	}

	size_t bit2pos(int bit)
	{
		size_t pos;
		switch(bit) {
		case 1:   pos = 0; break;
		case 2:   pos = 1; break;
		case 4:   pos = 2; break;
		case 8:   pos = 3; break;
		case 16:  pos = 4; break;
		case 32:  pos = 5; break;
		case 64:  pos = 6; break;
		case 128: pos = 7; break;
		case 256: pos = 8; break;
		default:
#if defined(_DEBUG)
			throw slvEceptionSolverBug_t(__LINE__, __FILE__);
#else
			pos = 9;
			break;
#endif
		}

		return pos;
	}

	/*
	** innerCell_t implementaion
	*/
	// innerCell_t ctor - defined in header

	// innerCell_t dtor
	innerCell_t::~innerCell_t() {
		// should not delete m_sets here; innerBoard_t's property
	}

	void innerCell_t::AddACellSet(innerCellSet_t * newSet)
	{
		size_t c;
		for(c = 0; c < (sizeof(m_sets)/sizeof(m_sets[0])); c++) {
			if(m_sets[c] == nullptr) {
				m_sets[c] = newSet;
				break;
			}
		}
	}

	statusChange_t innerCell_t::UpdateStatus()
	{
		valueSet_t orgValue = *this;
		*this &= *m_sets[0] & *m_sets[1];

		if(this->none())
			return STS_NO_SOLUTION;

		bool unique = (this->count() == 1);
		if(!unique && *this == orgValue)
			return STS_NONE;

		for(auto aSet : m_sets)
			aSet->UpdateStatus(this);

		return (unique ? STS_DETERMINED : STS_CHANGED);
	}

	/*
	** innerCellSet_t implementaion
	*/
	// innerCellSet_t ctor
	innerCellSet_t::innerCellSet_t(size_t x, size_t y, size_t numOfCells, size_t sum)
	{
        try {
		    SetCandidatesFromCellsAndSum(numOfCells, sum, m_cands);
        } catch(slvExceptionInvlideBoardData_t & e) {
            throw slvExceptionInvlideBoardData_t(e.GetReason(), x, y);
        }
		for(auto aCand : m_cands)
			*this |= *aCand;
	}

	// innerCellSet_t copy ctor
	innerCellSet_t::innerCellSet_t(const innerCellSet_t & org) :
        valueSet_t(org), m_detValues(org.m_detValues), m_cands(org.m_cands)
	{
	}

	// innerCellSet_t dtor
	innerCellSet_t::~innerCellSet_t()
	{
		// m_undetCells - should not delete the contents here
		// they belong to innterboard_t
	}

	void innerCellSet_t::UpdateStatus(innerCell_t * pCell)
	{
#if defined(_DEBUG)
		if(m_undetCells.find(pCell) == m_undetCells.end())
			throw slvEceptionSolverBug_t(__LINE__, __FILE__);
#endif
		auto itor = m_cands.begin();
		while(itor != m_cands.end()) {
			if((*pCell & **itor).none())
				itor = m_cands.erase(itor);
			else
				itor++;
		}

		if(pCell->count() == 1) {
			// aCell determined
			m_undetCells.erase(pCell);
			m_detValues |= *pCell;
		}

		valueSet_t possibleValues = m_detValues;
		for(auto undet : m_undetCells)
			possibleValues |= *undet;
		auto itor2 = m_cands.begin();
		while(itor2 != m_cands.end()) {
			if((possibleValues & **itor2) == **itor2)
				itor2++;
			else
				itor2 = m_cands.erase(itor2);
		}

		this->reset();
		for(auto cand : m_cands)
			*this |= *cand;
		*this &= ~m_detValues;
	}

	void innerCellSet_t::ApplyRule()
	{
		if(m_undetCells.empty())
			return;

		valueSet_t cand;
		cand.set();
		for(auto aCand : m_cands)
			cand &= *aCand;
		if(cand.none())
			return;
		cand &= ~m_detValues;

		// this commented code worked but it was slow
		//
		//size_t bcCand = 0;
		//std::vector<valueSet_t> cbits;
		//unsigned long tmp = cand.to_ulong();
		//while(tmp > 0) {
		//	++bcCand;
		//	unsigned long tmp2 = tmp & (tmp - 1);
		//	cbits.push_back(tmp ^ tmp2);
		//	tmp = tmp2;
		//}

		//for(size_t comb = 1; comb < (1 << bcCand) - 1; ++comb) {
		//	valueSet_t sigBits;
		//	size_t cSigBits = 0;
		//	tmp = comb;
		//	while(tmp > 0) {
		//		++cSigBits;
		//		unsigned long tmp2 = tmp & (tmp - 1);
		//		sigBits |= cbits[bit2pos(tmp ^ tmp2)];
		//		tmp = tmp2;
		//	}

		//	std::vector<innerCell_t *>pCells;
		//	pCells.reserve(9);
		//	for(auto pCurCell : m_undetCells) {
		//		if((*pCurCell & sigBits) != 0) {
		//			pCells.push_back(pCurCell);
		//		}
		//	}
		//	if(pCells.size() == cSigBits) {
		//		for(auto pCurCell : pCells)
		//			*pCurCell &= sigBits;
		//	}
		//}

		for(size_t bitpos = 0; bitpos < 9; bitpos++) {
			if(cand.test(bitpos)) {
				size_t count = 0;
				innerCell_t * pCell;
				for(auto pCurCell : m_undetCells) {
					if(pCurCell->test(bitpos)) {
						count++;
						pCell = pCurCell;
					}
				}
				if(count == 1) {
					pCell->reset();
					pCell->set(bitpos);
				}
			}
		}
	}

	void innerCellSet_t::ApplyRule2()
	{
		if(m_undetCells.empty())
			return;

		size_t numUndets = m_undetCells.size();
		std::vector<innerCell_t *> undetCellsRandom;
		undetCellsRandom.reserve(numUndets);
		for(auto pCell : m_undetCells)
			undetCellsRandom.push_back(pCell);

		for(int cmb = 3; cmb < (1 << numUndets) - 1; ++cmb) {
			size_t bc = getBitCount(cmb);
			if(bc < 2)
				continue;

			valueSet_t orSet;
			int tmp = cmb;
			int tmp2;
			while(tmp > 0) {
				tmp2 = tmp & (tmp - 1);
				orSet |= *undetCellsRandom[ bit2pos(tmp ^ tmp2) ];
				tmp = tmp2;
			}
			if(orSet.count() != bc)
				continue;

			for(size_t i = 0; i < numUndets; ++i) {
				if(((1 << i) | cmb) != 0)
					continue;
				*undetCellsRandom[i] &= ~orSet;
			}
		}
	}

	innerCellSet_t * innerCellSet_t::MakeSelfCopyForNewBoard(innerBoard_t * newBoard)
	{
		innerCellSet_t * newCopy = new innerCellSet_t(*this);

		for(auto pMyCell : m_undetCells) {
			const size_t idx = pMyCell->GetIndex();
			innerCell_t * pNewCell = &newBoard->pCells[idx];
			pNewCell->AddACellSet(newCopy);
			newCopy->AddACell(pNewCell);
		}

		return newCopy;
	}

	/*
	** innerBoard_t implementaion
	*/
	// innerBoard_t ctor
	innerBoard_t::innerBoard_t(size_t cx, size_t cy) : cells(cx*cy), pCells(cells)
	{
		for(size_t idx = 0; idx < cells; idx++)
			pCells[idx].SetIndex(idx);
	}

	innerBoard_t::innerBoard_t(const innerBoard_t & base) : cells(base.cells), pCells(cells)
	{
		for(size_t idx = 0; idx < cells; idx++) {
			pCells[idx] &= base.pCells[idx];
			pCells[idx].SetIndex(idx);
		}

		for(auto baseCell : base.undetCellSet) {
			undetCellSet.push_back(&pCells[baseCell->GetIndex()]);
		}

		cellSetSet.reserve(base.cellSetSet.size());
		for(auto baseCellSet : base.cellSetSet) {
			innerCellSet_t * newSet = baseCellSet->MakeSelfCopyForNewBoard(this);
			cellSetSet.push_back(newSet);
		}
	}

	innerBoard_t::~innerBoard_t()
	{
		for(auto aSet : cellSetSet)
			delete aSet;
	}
}