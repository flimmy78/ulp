/**************************************************************************************
* Copyright (c) 2005 , ICDEV.COM
* All rights reserved.
*
* miaofng@2010
*
* ascii font lib 8x16, horizontal direction
*
* versions:
*   1.0   2006.10.7  lcf  basic function
*   1.1    2010.08.9  remove chinese notes
**************************************************************************************/

const char ascii_8x16[] = {
	/*   */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* ! */
	0x00,0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x18,0x18,0x00,0x00,
	/* " */
	0x00,0x00,0x00,0x00,0x00,0x00,0x14,0x28,0x28,0x50,0x00,0x00,0x00,0x00,0x00,0x00,
	/* # */
	0x00,0x00,0x00,0x14,0x14,0x7E,0x14,0x14,0x28,0x7E,0x28,0x28,0x28,0x00,0x00,0x00,
	/* $ */
	0x00,0x00,0x10,0x38,0x54,0x54,0x50,0x30,0x18,0x14,0x54,0x54,0x38,0x10,0x10,0x00,
	/* % */
	0x00,0x00,0x00,0x44,0xA4,0xA8,0xA8,0x50,0x14,0x2A,0x2A,0x4A,0x44,0x00,0x00,0x00,
	/* & */
	0x00,0x00,0x00,0x20,0x50,0x50,0x50,0x7C,0xA8,0xA8,0x98,0x88,0x76,0x00,0x00,0x00,
	/* ' */
	0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x08,0x30,0x00,0x00,0x00,0x00,0x00,0x00,
	/* ( */
	0x00,0x00,0x04,0x08,0x10,0x10,0x20,0x20,0x20,0x20,0x20,0x10,0x10,0x08,0x04,0x00,
	/* ) */
	0x00,0x00,0x20,0x10,0x08,0x08,0x04,0x04,0x04,0x04,0x04,0x08,0x08,0x10,0x20,0x00,
	/* * */
	0x00,0x00,0x00,0x00,0x10,0x10,0xD6,0x38,0x38,0xD6,0x10,0x10,0x00,0x00,0x00,0x00,
	/* + */
	0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x10,0xFE,0x10,0x10,0x10,0x00,0x00,0x00,0x00,
	/* , */
	0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x08,0x30,0x00,0x00,0x00,0x00,0x00,0x00,
	/* - */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* . */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* / */
	0x00,0x00,0x02,0x04,0x04,0x04,0x08,0x08,0x10,0x10,0x10,0x20,0x20,0x40,0x40,0x00,
	/* 0 */
	0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00,0x00,
	/* 1 */
	0x00,0x00,0x00,0x10,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,0x00,
	/* 2 */
	0x00,0x00,0x00,0x3C,0x42,0x42,0x02,0x04,0x08,0x10,0x20,0x42,0x7E,0x00,0x00,0x00,
	/* 3 */
	0x00,0x00,0x00,0x3C,0x42,0x42,0x04,0x18,0x04,0x02,0x42,0x42,0x3C,0x00,0x00,0x00,
	/* 4 */
	0x00,0x00,0x00,0x08,0x08,0x18,0x28,0x48,0x48,0x7E,0x08,0x08,0x1E,0x00,0x00,0x00,
	/* 5 */
	0x00,0x00,0x00,0x7E,0x40,0x40,0x5C,0x62,0x02,0x02,0x42,0x42,0x3C,0x00,0x00,0x00,
	/* 6 */
	0x00,0x00,0x00,0x1C,0x24,0x40,0x40,0x5C,0x62,0x42,0x42,0x42,0x3C,0x00,0x00,0x00,
	/* 7 */
	0x00,0x00,0x00,0x7E,0x44,0x44,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x00,
	/* 8 */
	0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x3C,0x24,0x42,0x42,0x42,0x3C,0x00,0x00,0x00,
	/* 9 */
	0x00,0x00,0x00,0x38,0x44,0x42,0x42,0x46,0x3A,0x02,0x02,0x24,0x38,0x00,0x00,0x00,
	/* : */
	0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,
	/* ; */
	0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x10,0x00,0x00,0x00,
	/* < */
	0x00,0x00,0x00,0x02,0x04,0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x04,0x02,0x00,0x00,
	/* = */
	0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,
	/* > */
	0x00,0x00,0x00,0x40,0x20,0x10,0x08,0x04,0x02,0x04,0x08,0x10,0x20,0x40,0x00,0x00,
	/* ? */
	0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x02,0x04,0x08,0x08,0x00,0x18,0x18,0x00,0x00,
	/* @ */
	0x00,0x00,0x00,0x38,0x44,0x9A,0xAA,0xAA,0xAA,0xAA,0xB4,0x42,0x3C,0x00,0x00,0x00,
	/* A */
	0x00,0x00,0x00,0x10,0x10,0x28,0x28,0x28,0x28,0x7C,0x44,0x44,0xEE,0x00,0x00,0x00,
	/* B */
	0x00,0x00,0x00,0xFC,0x42,0x42,0x44,0x78,0x44,0x42,0x42,0x42,0xFC,0x00,0x00,0x00,
	/* C */
	0x00,0x00,0x00,0x3E,0x42,0x82,0x80,0x80,0x80,0x80,0x82,0x44,0x38,0x00,0x00,0x00,
	/* D */
	0x00,0x00,0x00,0xF8,0x44,0x42,0x42,0x42,0x42,0x42,0x42,0x44,0xF8,0x00,0x00,0x00,
	/* E */
	0x00,0x00,0x00,0xFC,0x42,0x48,0x48,0x78,0x48,0x48,0x40,0x42,0xFC,0x00,0x00,0x00,
	/* F */
	0x00,0x00,0x00,0xFC,0x42,0x48,0x48,0x78,0x48,0x48,0x40,0x40,0xE0,0x00,0x00,0x00,
	/* G */
	0x00,0x00,0x00,0x3C,0x44,0x84,0x80,0x80,0x80,0x8E,0x84,0x44,0x38,0x00,0x00,0x00,
	/* H */
	0x00,0x00,0x00,0xEE,0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x44,0xEE,0x00,0x00,0x00,
	/* I */
	0x00,0x00,0x00,0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,0x00,
	/* J */
	0x00,0x00,0x3E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x88,0xF0,0x00,0x00,
	/* K */
	0x00,0x00,0x00,0xEE,0x44,0x48,0x50,0x70,0x50,0x48,0x48,0x44,0xEE,0x00,0x00,0x00,
	/* L */
	0x00,0x00,0x00,0xE0,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x42,0xFE,0x00,0x00,0x00,
	/* M */
	0x00,0x00,0x00,0xEE,0x6C,0x6C,0x6C,0x54,0x54,0x54,0x54,0x54,0xD6,0x00,0x00,0x00,
	/* N */
	0x00,0x00,0x00,0xEE,0x64,0x64,0x54,0x54,0x54,0x4C,0x4C,0x4C,0xE4,0x00,0x00,0x00,
	/* O */
	0x00,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x00,0x00,0x00,
	/* P */
	0x00,0x00,0x00,0xFC,0x42,0x42,0x42,0x7C,0x40,0x40,0x40,0x40,0xE0,0x00,0x00,0x00,
	/* Q */
	0x00,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0xB2,0x4C,0x38,0x06,0x00,0x00,
	/* R */
	0x00,0x00,0x00,0xF8,0x44,0x44,0x44,0x78,0x50,0x48,0x48,0x44,0xE6,0x00,0x00,0x00,
	/* S */
	0x00,0x00,0x00,0x3E,0x42,0x42,0x40,0x30,0x0C,0x02,0x42,0x42,0x7C,0x00,0x00,0x00,
	/* T */
	0x00,0x00,0x00,0xFE,0x92,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00,0x00,
	/* U */
	0x00,0x00,0x00,0xEE,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x38,0x00,0x00,0x00,
	/* V */
	0x00,0x00,0x00,0xEE,0x44,0x44,0x44,0x28,0x28,0x28,0x28,0x10,0x10,0x00,0x00,0x00,
	/* W */
	0x00,0x00,0x00,0xD6,0x54,0x54,0x54,0x54,0x6C,0x28,0x28,0x28,0x28,0x00,0x00,0x00,
	/* X */
	0x00,0x00,0x00,0xEE,0x44,0x28,0x28,0x10,0x10,0x28,0x28,0x44,0xEE,0x00,0x00,0x00,
	/* Y */
	0x00,0x00,0x00,0xEE,0x44,0x44,0x28,0x28,0x10,0x10,0x10,0x10,0x38,0x00,0x00,0x00,
	/* Z */
	0x00,0x00,0x00,0x3E,0x44,0x04,0x08,0x08,0x10,0x10,0x20,0x22,0x7E,0x00,0x00,0x00,
	/* [ */
	0x00,0x00,0x3C,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3C,0x00,
	/* \ */
	0x00,0x00,0x40,0x40,0x20,0x20,0x10,0x10,0x10,0x08,0x08,0x04,0x04,0x04,0x02,0x00,
	/* ] */
	0x00,0x00,0x3C,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x3C,0x00,
	/* ^ */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* _ */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* ` */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* a */
	0x00,0x00,0x00,0x00,0x00,0x38,0x44,0x1C,0x24,0x44,0x44,0x3E,0x00,0x00,0x00,0x00,
	/* b */
	0x00,0x00,0x00,0xC0,0x40,0x40,0x5C,0x62,0x42,0x42,0x42,0x42,0x7C,0x00,0x00,0x00,
	/* c */
	0x00,0x00,0x00,0x00,0x00,0x1C,0x22,0x40,0x40,0x40,0x22,0x1C,0x00,0x00,0x00,0x00,
	/* d */
	0x00,0x00,0x00,0x0C,0x04,0x04,0x7C,0x84,0x84,0x84,0x84,0x8C,0x76,0x00,0x00,0x00,
	/* e */
	0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x7E,0x40,0x40,0x42,0x3C,0x00,0x00,0x00,0x00,
	/* f */
	0x00,0x00,0x00,0x0E,0x12,0x10,0x7C,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,0x00,
	/* g */
	0x00,0x00,0x00,0x00,0x3E,0x44,0x44,0x38,0x40,0x3C,0x42,0x42,0x3C,0x00,0x00,0x00,
	/* h */
	0x00,0x00,0x00,0xC0,0x40,0x40,0x5C,0x62,0x42,0x42,0x42,0x42,0xE7,0x00,0x00,0x00,
	/* i */
	0x00,0x00,0x00,0x30,0x30,0x00,0x70,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,0x00,
	/* j */
	0x00,0x00,0x0C,0x0C,0x00,0x1C,0x04,0x04,0x04,0x04,0x04,0x04,0x44,0x78,0x00,0x00,
	/* k */
	0x00,0x00,0x00,0xC0,0x40,0x40,0x4E,0x48,0x50,0x68,0x48,0x44,0xEE,0x00,0x00,0x00,
	/* l */
	0x00,0x00,0x00,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,0x00,
	/* m */
	0x00,0x00,0x00,0x00,0x00,0xF8,0x54,0x54,0x54,0x54,0x54,0xD6,0x00,0x00,0x00,0x00,
	/* n */
	0x00,0x00,0x00,0x00,0x00,0xDC,0x62,0x42,0x42,0x42,0x42,0xE7,0x00,0x00,0x00,0x00,
	/* o */
	0x00,0x00,0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x24,0x18,0x00,0x00,0x00,0x00,
	/* p */
	0x00,0x00,0x00,0x00,0xDC,0x62,0x42,0x42,0x42,0x42,0x7C,0x40,0xE0,0x00,0x00,0x00,
	/* q */
	0x00,0x00,0x00,0x00,0x7C,0x84,0x84,0x84,0x84,0x8C,0x74,0x04,0x0E,0x00,0x00,0x00,
	/* r */
	0x00,0x00,0x00,0x00,0x00,0xEE,0x32,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,0x00,0x00,
	/* s */
	0x00,0x00,0x00,0x00,0x00,0x3C,0x44,0x40,0x38,0x04,0x44,0x78,0x00,0x00,0x00,0x00,
	/* t */
	0x00,0x00,0x00,0x00,0x10,0x10,0x7C,0x10,0x10,0x10,0x10,0x10,0x0C,0x00,0x00,0x00,
	/* u */
	0x00,0x00,0x00,0x00,0x00,0xC6,0x42,0x42,0x42,0x42,0x46,0x3B,0x00,0x00,0x00,0x00,
	/* v */
	0x00,0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x28,0x10,0x10,0x00,0x00,0x00,0x00,
	/* w */
	0x00,0x00,0x00,0x00,0x00,0xD6,0x54,0x54,0x54,0x28,0x28,0x28,0x00,0x00,0x00,0x00,
	/* x */
	0x00,0x00,0x00,0x00,0x00,0x6E,0x24,0x18,0x18,0x18,0x24,0x76,0x00,0x00,0x00,0x00,
	/* y */
	0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x28,0x18,0x10,0x10,0xE0,0x00,0x00,0x00,
	/* z */
	0x00,0x00,0x00,0x00,0x00,0x7E,0x44,0x08,0x10,0x10,0x22,0x7E,0x00,0x00,0x00,0x00,
	/* { */
	0x00,0x00,0x18,0x10,0x10,0x10,0x10,0x10,0x20,0x10,0x10,0x10,0x10,0x10,0x18,0x00,
	/* | */
	0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
	/* } */
	0x00,0x00,0x30,0x10,0x10,0x10,0x10,0x10,0x08,0x10,0x10,0x10,0x10,0x10,0x30,0x00,
	/* ~ */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x62,0x92,0x8C,0x00,0x00,0x00,0x00,0x00,0x00,
};
