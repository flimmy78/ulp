/*
 * david@2012 initial version
 * dac:12bits, sample points:16/32, sample points:52/104
 */

//sample points 32 per period, dac 12bits
//math function = (2048 + 2048*sinx)*0.7575, x<0,2pi>,
//  REF = 3.3V, VOUTp-p = 2.5V, VOUTp-p = 0.7575 * REF
// 2.5/3.3 = 0.7575
static const unsigned short gear32[32] = {	\
	0x060f, 0x073d, 0x0860, 0x096c, 0x0a57, 0x0b18, 0x0ba8, 0x0c00, \
	0x0c1e, 0x0c01, 0x0ba9, 0x0b1a, 0x0a59, 0x096e, 0x0863, 0x0740, \
	0x0611, 0x04e3, 0x03c0, 0x02b3, 0x01c8, 0x0107, 0x0077, 0x001e, \
	0x0000, 0x001d, 0x0074, 0x0103, 0x01c3, 0x02ad, 0x03b9, 0x04dc, \
};
static const unsigned short gear32_1[32] = {	\
	0x0070, 0x0083, 0x0096, 0x00a6, 0x00b5, 0x00c1, 0x0ca, 0x00d0, \
	0x00d1, 0x00d0, 0x00ca, 0x00c1, 0x00b5, 0x00a6, 0x0096, 0x0084, \
	0x0071, 0x005e, 0x004c, 0x003b, 0x002c, 0x0020, 0x0017, 0x0011, \
	0x0010, 0x0011, 0x0017, 0x0020, 0x002c, 0x003a, 0x004b, 0x005d, \
};

//sample points 16 per period, dac 12bits
//math function = (2048 + 2048*sinx)*0.7575, x<0,2pi>
//  REF = 3.3V, VOUTp-p = 2.5V, VOUTp-p = 0.7575 * REF
// 2.5/3.3 = 0.7575
static const unsigned short gear16[16] = {	\
	0x060f, 0x0860, 0x0a57, 0x0ba8, 0x0c1e, 0x0ba9, 0x0a59, 0x0863, \
	0x0611, 0x03c0, 0x01c8, 0x0077, 0x0000, 0x0074, 0x01c3, 0x03b9, \
};

//sample points 52 per period, dac 12bits
//math function = (2048 + 2048*sinx)*0.7575, x<0,2pi>
// 2.5/3.3 = 0.7575
static const unsigned short gear52[52] = {	\
	0x060f, 0x06ca, 0x0782, 0x0835, 0x08df, 0x0980, 0x0a13, 0x0a98, \
	0x0b0b, 0x0b6c, 0x0bb9, 0x0bf1, 0x0c13, 0x0c1e, 0x0c13, 0x0bf1, \
	0x0bba, 0x0b6d, 0x0b0d, 0x0a99, 0x0a15, 0x0982, 0x08e2, 0x0837, \
	0x0784, 0x06cc, 0x0611, 0x0556, 0x049e, 0x03eb, 0x0340, 0x02a0, \
	0x020c, 0x0188, 0x0114, 0x00b3, 0x0066, 0x002d, 0x000b, 0x0000, \
	0x000a, 0x002c, 0x0063, 0x00af, 0x0110, 0x0183, 0x0207, 0x029a, \
	0x033a, 0x03e4, 0x0497, 0x054f, \
};

//sample points 104 per period, dac 12bits
//math function = (2048 + 2048*sinx)*0.7575, x<0,2pi>
// 2.5/3.3 = 0.7575
static const unsigned short gear104[104] = {	\
	0x60f, 0x66c, 0x6ca, 0x726, 0x782, 0x7dc, 0x835, 0x88b, \
	0x8df, 0x931, 0x980, 0x9cb, 0xa13, 0xa57, 0xa98, 0xad4, \
	0xb0b, 0xb3e, 0xb6c, 0xb95, 0xbb9, 0xbd8, 0xbf1, 0xc05, \
	0xc13, 0xc1b, 0xc1e, 0xc1b, 0xc13, 0xc05, 0xbf1, 0xbd8, \
	0xbba, 0xb96, 0xb6d, 0xb3f, 0xb0d, 0xad5, 0xa99, 0xa59, \
	0xa15, 0x9cd, 0x982, 0x933, 0x8e2, 0x88e, 0x837, 0x7df, \
	0x784, 0x729, 0x6cc, 0x66f, 0x611, 0x5b4, 0x556, 0x4fa, \
	0x49e, 0x444, 0x3eb, 0x395, 0x340, 0x2ef, 0x2a0, 0x254, \
	0x20c, 0x1c8, 0x188, 0x14c, 0x114, 0x0e1, 0x0b3, 0x08a, \
	0x066, 0x047, 0x02d, 0x01a, 0x00b, 0x003, 0x000, 0x002, \
	0x00a, 0x018, 0x02c, 0x045, 0x063, 0x087, 0x0af, 0x0dd, \
	0x110, 0x147, 0x183, 0x1c3, 0x207, 0x24f, 0x29a, 0x2e8, \
	0x33a, 0x38e, 0x3e4, 0x43d, 0x497, 0x4f3, 0x54f, 0x5ac, \
};
static const unsigned short gear104_1[104] = {	\
	0x070, 0x076, 0x07c, 0x082, 0x088, 0x08d, 0x093, 0x098, \
	0x09d, 0xa3, 0xa8, 0xac, 0xb1, 0xb5, 0xb9, 0xbd, \
	0xc0, 0xc3, 0xc6, 0xc9, 0xcb, 0xcd, 0xcf, 0xd0, \
	0xd1, 0xd1, 0xd1, 0xd1, 0xd1, 0xd0, 0xcf, 0xcd, \
	0xcb, 0xc9, 0xc6, 0xc3, 0xc0, 0xbd, 0xb9, 0xb5, \
	0xb1, 0xac, 0xa8, 0xa3, 0x9e, 0x98, 0x93, 0x8d, \
	0x88, 0x82, 0x7c, 0x76, 0x71, 0x6b, 0x65, 0x5f, \
	0x59, 0x54, 0x4e, 0x49, 0x44, 0x3e, 0x3a, 0x35, \
	0x30, 0x2c, 0x28, 0x24, 0x21, 0x1e, 0x1b, 0x18, \
	0x16, 0x14, 0x12, 0x11, 0x10, 0x10, 0x10, 0x10, \
	0x10, 0x11, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1d, \
	0x21, 0x24, 0x28, 0x2c, 0x30, 0x34, 0x39, 0x3e, \
	0x43, 0x48, 0x4e, 0x53, 0x59, 0x5f, 0x64, 0x6a, \
};
static  unsigned short gear104_3840[3840] = {
0x71,0x77,0x7D,0x83,0x88,0x8E,0x93,0x99,0x9E,0xA3,0xA8,0xAD,0xB1,
0xB6,0xBA,0xBD,0xC1,0xC4,0xC7,0xCA,0xCC,0xCE,0xCF,0xD0,0xD1,0xD2,
0xD2,0xD2,0xD1,0xD0,0xCF,0xCD,0xCB,0xC9,0xC7,0xC4,0xC1,0xBD,0xB9,
0xB5,0xB1,0xAC,0xA8,0xA3,0x9E,0x98,0x93,0x8D,0x88,0x82,0x7C,0x76,
0x70,0x6A,0x65,0x5F,0x59,0x53,0x4E,0x48,0x43,0x3E,0x39,0x35,0x30,
0x2C,0x28,0x24,0x21,0x1E,0x1B,0x18,0x16,0x14,0x13,0x11,0x11,0x10,
0x10,0x10,0x11,0x12,0x13,0x15,0x17,0x19,0x1C,0x1F,0x22,0x25,0x29,
0x2D,0x32,0x36,0x3B,0x40,0x45,0x4A,0x50,0x55,0x5B,0x61,0x67,0x6D,
0x72,0x78,0x7E,0x84,0x8A,0x8F,0x95,0x9A,0x9F,0xA4,0xA9,0xAE,0xB2,
0xB7,0xBB,0xBE,0xC2,0xC5,0xC8,0xCA,0xCC,0xCE,0xD0,0xD1,0xD1,0xD2,
0xD2,0xD2,0xD1,0xD0,0xCF,0xCD,0xCB,0xC9,0xC6,0xC3,0xC0,0xBC,0xB8,
0xB4,0xB0,0xAB,0xA6,0xA1,0x9C,0x97,0x91,0x8C,0x86,0x80,0x7B,0x75,
0x6F,0x69,0x63,0x5D,0x58,0x52,0x4D,0x47,0x42,0x3D,0x38,0x33,0x2F,
0x2B,0x27,0x23,0x20,0x1D,0x1A,0x18,0x16,0x14,0x12,0x11,0x10,0x10,
0x10,0x10,0x11,0x12,0x13,0x15,0x17,0x1A,0x1C,0x1F,0x23,0x26,0x2A,
0x2E,0x33,0x37,0x3C,0x41,0x46,0x4C,0x51,0x57,0x5D,0x62,0x68,0x6E,
0x74,0x7A,0x80,0x85,0x8B,0x91,0x96,0x9B,0xA1,0xA6,0xAB,0xAF,0xB4,
0xB8,0xBC,0xBF,0xC2,0xC6,0xC8,0xCB,0xCD,0xCE,0xD0,0xD1,0xD2,0xD2,
0xD2,0xD2,0xD1,0xD0,0xCE,0xCD,0xCA,0xC8,0xC5,0xC2,0xBF,0xBB,0xB7,
0xB3,0xAF,0xAA,0xA5,0xA0,0x9B,0x96,0x90,0x8A,0x85,0x7F,0x79,0x73,
0x6D,0x68,0x62,0x5C,0x56,0x51,0x4B,0x46,0x41,0x3C,0x37,0x32,0x2E,
0x2A,0x26,0x22,0x1F,0x1C,0x19,0x17,0x15,0x13,0x12,0x11,0x10,0x10,
0x10,0x11,0x11,0x12,0x14,0x16,0x18,0x1A,0x1D,0x20,0x24,0x27,0x2B,
0x2F,0x34,0x39,0x3D,0x42,0x48,0x4D,0x53,0x58,0x5E,0x64,0x6A,0x6F,
0x75,0x7B,0x81,0x87,0x8C,0x92,0x97,0x9D,0xA2,0xA7,0xAC,0xB0,0xB5,
0xB9,0xBC,0xC0,0xC3,0xC6,0xC9,0xCB,0xCD,0xCF,0xD0,0xD1,0xD2,0xD2,
0xD2,0xD1,0xD1,0xCF,0xCE,0xCC,0xCA,0xC7,0xC5,0xC1,0xBE,0xBA,0xB6,
0xB2,0xAE,0xA9,0xA4,0x9F,0x9A,0x94,0x8F,0x89,0x83,0x7E,0x78,0x72,
0x6C,0x66,0x60,0x5B,0x55,0x4F,0x4A,0x45,0x3F,0x3B,0x36,0x31,0x2D,
0x29,0x25,0x22,0x1E,0x1B,0x19,0x17,0x15,0x13,0x12,0x11,0x10,0x10,
0x10,0x11,0x12,0x13,0x14,0x16,0x18,0x1B,0x1E,0x21,0x24,0x28,0x2C,
0x30,0x35,0x3A,0x3F,0x44,0x49,0x4E,0x54,0x5A,0x5F,0x65,0x6B,0x71,
0x77,0x7D,0x82,0x88,0x8E,0x93,0x99,0x9E,0xA3,0xA8,0xAD,0xB1,0xB6,
0xBA,0xBD,0xC1,0xC4,0xC7,0xC9,0xCC,0xCE,0xCF,0xD0,0xD1,0xD2,0xD2,
0xD2,0xD1,0xD0,0xCF,0xCD,0xCC,0xC9,0xC7,0xC4,0xC1,0xBD,0xB9,0xB5,
0xB1,0xAC,0xA8,0xA3,0x9E,0x98,0x93,0x8D,0x88,0x82,0x7C,0x76,0x70,
0x6B,0x65,0x5F,0x59,0x54,0x4E,0x49,0x43,0x3E,0x39,0x35,0x30,0x2C,
0x28,0x24,0x21,0x1E,0x1B,0x18,0x16,0x14,0x13,0x11,0x11,0x10,0x10,
0x10,0x11,0x12,0x13,0x15,0x17,0x19,0x1C,0x1F,0x22,0x25,0x29,0x2D,
0x32,0x36,0x3B,0x40,0x45,0x4A,0x50,0x55,0x5B,0x61,0x67,0x6C,0x72,
0x78,0x7E,0x84,0x89,0x8F,0x95,0x9A,0x9F,0xA4,0xA9,0xAE,0xB2,0xB7,
0xBB,0xBE,0xC2,0xC5,0xC8,0xCA,0xCC,0xCE,0xCF,0xD1,0xD1,0xD2,0xD2,
0xD2,0xD1,0xD0,0xCF,0xCD,0xCB,0xC9,0xC6,0xC3,0xC0,0xBC,0xB8,0xB4,
0xB0,0xAB,0xA7,0xA2,0x9C,0x97,0x92,0x8C,0x86,0x81,0x7B,0x75,0x6F,
0x69,0x63,0x5E,0x58,0x52,0x4D,0x47,0x42,0x3D,0x38,0x34,0x2F,0x2B,
0x27,0x23,0x20,0x1D,0x1A,0x18,0x16,0x14,0x12,0x11,0x10,0x10,0x10,
0x10,0x11,0x12,0x13,0x15,0x17,0x1A,0x1C,0x1F,0x23,0x26,0x2A,0x2E,
0x33,0x37,0x3C,0x41,0x46,0x4C,0x51,0x57,0x5C,0x62,0x68,0x6E,0x74,
0x7A,0x7F,0x85,0x8B,0x90,0x96,0x9B,0xA1,0xA6,0xAA,0xAF,0xB3,0xB8,
0xBB,0xBF,0xC2,0xC5,0xC8,0xCB,0xCD,0xCE,0xD0,0xD1,0xD2,0xD2,0xD2,
0xD2,0xD1,0xD0,0xCE,0xCD,0xCA,0xC8,0xC5,0xC2,0xBF,0xBB,0xB7,0xB3,
0xAF,0xAA,0xA5,0xA0,0x9B,0x96,0x90,0x8B,0x85,0x7F,0x79,0x73,0x6E,
0x68,0x62,0x5C,0x56,0x51,0x4B,0x46,0x41,0x3C,0x37,0x32,0x2E,0x2A,
0x26,0x22,0x1F,0x1C,0x19,0x17,0x15,0x13,0x12,0x11,0x10,0x10,0x10,
0x11,0x11,0x12,0x14,0x16,0x18,0x1A,0x1D,0x20,0x23,0x27,0x2B,0x2F,
0x34,0x38,0x3D,0x42,0x48,0x4D,0x52,0x58,0x5E,0x64,0x69,0x6F,0x75,
0x7B,0x81,0x87,0x8C,0x92,0x97,0x9D,0xA2,0xA7,0xAC,0xB0,0xB4,0xB9,
0xBC,0xC0,0xC3,0xC6,0xC9,0xCB,0xCD,0xCF,0xD0,0xD1,0xD2,0xD2,0xD2,
0xD1,0xD1,0xCF,0xCE,0xCC,0xCA,0xC7,0xC5,0xC1,0xBE,0xBA,0xB6,0xB2,
0xAE,0xA9,0xA4,0x9F,0x9A,0x94,0x8F,0x89,0x84,0x7E,0x78,0x72,0x6C,
0x66,0x60,0x5B,0x55,0x4F,0x4A,0x45,0x40,0x3B,0x36,0x31,0x2D,0x29,
0x25,0x22,0x1E,0x1C,0x19,0x17,0x15,0x13,0x12,0x11,0x10,0x10,0x10,
0x11,0x12,0x13,0x14,0x16,0x18,0x1B,0x1E,0x21,0x24,0x28,0x2C,0x30,
0x35,0x3A,0x3E,0x44,0x49,0x4E,0x54,0x59,0x5F,0x65,0x6B,0x71,0x77,
0x7C,0x82,0x88,0x8E,0x93,0x99,0x9E,0xA3,0xA8,0xAD,0xB1,0xB5,0xB9,
0xBD,0xC1,0xC4,0xC7,0xC9,0xCC,0xCE,0xCF,0xD0,0xD1,0xD2,0xD2,0xD2,
0xD1,0xD0,0xCF,0xCE,0xCC,0xC9,0xC7,0xC4,0xC1,0xBD,0xB9,0xB5,0xB1,
0xAD,0xA8,0xA3,0x9E,0x99,0x93,0x8E,0x88,0x82,0x7C,0x76,0x71,0x6B,
0x65,0x5F,0x59,0x54,0x4E,0x49,0x43,0x3E,0x39,0x35,0x30,0x2C,0x28,
0x24,0x21,0x1E,0x1B,0x18,0x16,0x14,0x13,0x11,0x11,0x10,0x10,0x10,
0x11,0x12,0x13,0x15,0x17,0x19,0x1C,0x1E,0x22,0x25,0x29,0x2D,0x31,
0x36,0x3B,0x40,0x45,0x4A,0x50,0x55,0x5B,0x61,0x66,0x6C,0x72,0x78,
0x7E,0x84,0x89,0x8F,0x94,0x9A,0x9F,0xA4,0xA9,0xAE,0xB2,0xB6,0xBA,
0xBE,0xC2,0xC5,0xC7,0xCA,0xCC,0xCE,0xCF,0xD1,0xD1,0xD2,0xD2,0xD2,
0xD1,0xD0,0xCF,0xCD,0xCB,0xC9,0xC6,0xC3,0xC0,0xBC,0xB8,0xB4,0xB0,
0xAB,0xA7,0xA2,0x9D,0x97,0x92,0x8C,0x86,0x81,0x7B,0x75,0x6F,0x69,
0x63,0x5E,0x58,0x52,0x4D,0x47,0x42,0x3D,0x38,0x34,0x2F,0x2B,0x27,
0x23,0x20,0x1D,0x1A,0x18,0x16,0x14,0x12,0x11,0x11,0x10,0x10,0x10,
0x11,0x12,0x13,0x15,0x17,0x1A,0x1C,0x1F,0x23,0x26,0x2A,0x2E,0x32,
0x37,0x3C,0x41,0x46,0x4B,0x51,0x57,0x5C,0x62,0x68,0x6E,0x74,0x79,
0x7F,0x85,0x8B,0x90,0x96,0x9B,0xA0,0xA5,0xAA,0xAF,0xB3,0xB7,0xBB,
0xBF,0xC2,0xC5,0xC8,0xCB,0xCD,0xCE,0xD0,0xD1,0xD2,0xD2,0xD2,0xD2,
0xD1,0xD0,0xCE,0xCD,0xCB,0xC8,0xC5,0xC2,0xBF,0xBB,0xB8,0xB3,0xAF,
0xAA,0xA5,0xA0,0x9B,0x96,0x90,0x8B,0x85,0x7F,0x79,0x74,0x6E,0x68,
0x62,0x5C,0x57,0x51,0x4C,0x46,0x41,0x3C,0x37,0x33,0x2E,0x2A,0x26,
0x23,0x1F,0x1C,0x1A,0x17,0x15,0x13,0x12,0x11,0x10,0x10,0x10,0x10,
0x11,0x12,0x14,0x16,0x18,0x1A,0x1D,0x20,0x23,0x27,0x2B,0x2F,0x34,
0x38,0x3D,0x42,0x47,0x4D,0x52,0x58,0x5E,0x63,0x69,0x6F,0x75,0x7B,
0x81,0x86,0x8C,0x92,0x97,0x9C,0xA2,0xA7,0xAB,0xB0,0xB4,0xB8,0xBC,
0xC0,0xC3,0xC6,0xC9,0xCB,0xCD,0xCF,0xD0,0xD1,0xD2,0xD2,0xD2,0xD1,
0xD1,0xCF,0xCE,0xCC,0xCA,0xC8,0xC5,0xC2,0xBE,0xBA,0xB7,0xB2,0xAE,
0xA9,0xA4,0x9F,0x9A,0x95,0x8F,0x89,0x84,0x7E,0x78,0x72,0x6C,0x66,
0x61,0x5B,0x55,0x50,0x4A,0x45,0x40,0x3B,0x36,0x31,0x2D,0x29,0x25,
0x22,0x1F,0x1C,0x19,0x17,0x15,0x13,0x12,0x11,0x10,0x10,0x10,0x11,
0x11,0x13,0x14,0x16,0x18,0x1B,0x1E,0x21,0x24,0x28,0x2C,0x30,0x35,
0x39,0x3E,0x43,0x49,0x4E,0x54,0x59,0x5F,0x65,0x6B,0x71,0x76,0x7C,
0x82,0x88,0x8D,0x93,0x98,0x9E,0xA3,0xA8,0xAD,0xB1,0xB5,0xB9,0xBD,
0xC1,0xC4,0xC7,0xC9,0xCC,0xCE,0xCF,0xD0,0xD1,0xD2,0xD2,0xD2,0xD1,
0xD0,0xCF,0xCE,0xCC,0xC9,0xC7,0xC4,0xC1,0xBD,0xBA,0xB6,0xB1,0xAD,
0xA8,0xA3,0x9E,0x99,0x93,0x8E,0x88,0x82,0x7C,0x77,0x71,0x6B,0x65,
0x5F,0x5A,0x54,0x4E,0x49,0x44,0x3F,0x3A,0x35,0x30,0x2C,0x28,0x24,
0x21,0x1E,0x1B,0x18,0x16,0x14,0x13,0x12,0x11,0x10,0x10,0x10,0x11,
0x12,0x13,0x15,0x17,0x19,0x1B,0x1E,0x22,0x25,0x29,0x2D,0x31,0x36,
0x3B,0x40,0x45,0x4A,0x4F,0x55,0x5B,0x60,0x66,0x6C,0x72,0x78,0x7E,
0x83,0x89,0x8F,0x94,0x9A,0x9F,0xA4,0xA9,0xAE,0xB2,0xB6,0xBA,0xBE,
0xC1,0xC5,0xC7,0xCA,0xCC,0xCE,0xCF,0xD1,0xD1,0xD2,0xD2,0xD2,0xD1,
0xD0,0xCF,0xCD,0xCB,0xC9,0xC6,0xC3,0xC0,0xBC,0xB9,0xB4,0xB0,0xAC,
0xA7,0xA2,0x9D,0x97,0x92,0x8C,0x87,0x81,0x7B,0x75,0x6F,0x69,0x64,
0x5E,0x58,0x52,0x4D,0x48,0x42,0x3D,0x38,0x34,0x2F,0x2B,0x27,0x24,
0x20,0x1D,0x1A,0x18,0x16,0x14,0x12,0x11,0x11,0x10,0x10,0x10,0x11,
0x12,0x13,0x15,0x17,0x19,0x1C,0x1F,0x22,0x26,0x2A,0x2E,0x32,0x37,
0x3C,0x41,0x46,0x4B,0x51,0x56,0x5C,0x62,0x68,0x6E,0x73,0x79,0x7F,
0x85,0x8B,0x90,0x96,0x9B,0xA0,0xA5,0xAA,0xAF,0xB3,0xB7,0xBB,0xBF,
0xC2,0xC5,0xC8,0xCA,0xCD,0xCE,0xD0,0xD1,0xD2,0xD2,0xD2,0xD2,0xD1,
0xD0,0xCE,0xCD,0xCB,0xC8,0xC5,0xC2,0xBF,0xBC,0xB8,0xB3,0xAF,0xAA,
0xA6,0xA1,0x9B,0x96,0x91,0x8B,0x85,0x7F,0x7A,0x74,0x6E,0x68,0x62,
0x5C,0x57,0x51,0x4C,0x46,0x41,0x3C,0x37,0x33,0x2E,0x2A,0x26,0x23,
0x1F,0x1C,0x1A,0x17,0x15,0x13,0x12,0x11,0x10,0x10,0x10,0x10,0x11,
0x12,0x14,0x16,0x18,0x1A,0x1D,0x20,0x23,0x27,0x2B,0x2F,0x33,0x38,
0x3D,0x42,0x47,0x4D,0x52,0x58,0x5D,0x63,0x69,0x6F,0x75,0x7B,0x80,
0x86,0x8C,0x92,0x97,0x9C,0xA1,0xA6,0xAB,0xB0,0xB4,0xB8,0xBC,0xC0,
0xC3,0xC6,0xC9,0xCB,0xCD,0xCF,0xD0,0xD1,0xD2,0xD2,0xD2,0xD1,0xD1,
0xD0,0xCE,0xCC,0xCA,0xC8,0xC5,0xC2,0xBE,0xBB,0xB7,0xB2,0xAE,0xA9,
0xA4,0x9F,0x9A,0x95,0x8F,0x8A,0x84,0x7E,0x78,0x72,0x6C,0x67,0x61,
0x5B,0x55,0x50,0x4A,0x45,0x40,0x3B,0x36,0x32,0x2D,0x29,0x25,0x22,
0x1F,0x1C,0x19,0x17,0x15,0x13,0x12,0x11,0x10,0x10,0x10,0x11,0x11,
0x13,0x14,0x16,0x18,0x1B,0x1E,0x21,0x24,0x28,0x2C,0x30,0x35,0x39,
0x3E,0x43,0x49,0x4E,0x53,0x59,0x5F,0x65,0x6A,0x70,0x76,0x7C,0x82,
0x88,0x8D,0x93,0x98,0x9E,0xA3,0xA8,0xAC,0xB1,0xB5,0xB9,0xBD,0xC1,
0xC4,0xC7,0xC9,0xCC,0xCD,0xCF,0xD0,0xD1,0xD2,0xD2,0xD2,0xD1,0xD0,
0xCF,0xCE,0xCC,0xC9,0xC7,0xC4,0xC1,0xBD,0xBA,0xB6,0xB1,0xAD,0xA8,
0xA3,0x9E,0x99,0x93,0x8E,0x88,0x82,0x7D,0x77,0x71,0x6B,0x65,0x5F,
0x5A,0x54,0x4E,0x49,0x44,0x3F,0x3A,0x35,0x31,0x2C,0x28,0x24,0x21,
0x1E,0x1B,0x18,0x16,0x14,0x13,0x12,0x11,0x10,0x10,0x10,0x11,0x12,
0x13,0x15,0x17,0x19,0x1B,0x1E,0x22,0x25,0x29,0x2D,0x31,0x36,0x3A,
0x3F,0x45,0x4A,0x4F,0x55,0x5B,0x60,0x66,0x6C,0x72,0x78,0x7D,0x83,
0x89,0x8F,0x94,0x9A,0x9F,0xA4,0xA9,0xAE,0xB2,0xB6,0xBA,0xBE,0xC1,
0xC4,0xC7,0xCA,0xCC,0xCE,0xCF,0xD1,0xD1,0xD2,0xD2,0xD2,0xD1,0xD0,
0xCF,0xCD,0xCB,0xC9,0xC6,0xC3,0xC0,0xBD,0xB9,0xB5,0xB0,0xAC,0xA7,
0xA2,0x9D,0x97,0x92,0x8C,0x87,0x81,0x7B,0x75,0x6F,0x6A,0x64,0x5E,
0x58,0x53,0x4D,0x48,0x43,0x3D,0x39,0x34,0x2F,0x2B,0x27,0x24,0x20,
0x1D,0x1A,0x18,0x16,0x14,0x12,0x11,0x11,0x10,0x10,0x10,0x11,0x12,
0x13,0x15,0x17,0x19,0x1C,0x1F,0x22,0x26,0x2A,0x2E,0x32,0x37,0x3C,
0x41,0x46,0x4B,0x51,0x56,0x5C,0x62,0x67,0x6D,0x73,0x79,0x7F,0x85,
0x8A,0x90,0x96,0x9B,0xA0,0xA5,0xAA,0xAF,0xB3,0xB7,0xBB,0xBF,0xC2,
0xC5,0xC8,0xCA,0xCD,0xCE,0xD0,0xD1,0xD2,0xD2,0xD2,0xD2,0xD1,0xD0,
0xCE,0xCD,0xCB,0xC8,0xC6,0xC3,0xBF,0xBC,0xB8,0xB4,0xAF,0xAB,0xA6,
0xA1,0x9C,0x96,0x91,0x8B,0x85,0x80,0x7A,0x74,0x6E,0x68,0x62,0x5D,
0x57,0x51,0x4C,0x46,0x41,0x3C,0x37,0x33,0x2E,0x2A,0x26,0x23,0x1F,
0x1C,0x1A,0x17,0x15,0x14,0x12,0x11,0x10,0x10,0x10,0x10,0x11,0x12,
0x14,0x15,0x18,0x1A,0x1D,0x20,0x23,0x27,0x2B,0x2F,0x33,0x38,0x3D,
0x42,0x47,0x4C,0x52,0x58,0x5D,0x63,0x69,0x6F,0x75,0x7A,0x80,0x86,
0x8C,0x91,0x97,0x9C,0xA1,0xA6,0xAB,0xB0,0xB4,0xB8,0xBC,0xC0,0xC3,
0xC6,0xC9,0xCB,0xCD,0xCF,0xD0,0xD1,0xD2,0xD2,0xD2,0xD1,0xD1,0xD0,0xCE,0xCC,0xCA,0xC8,0xC5,0xC2,0xBE,0xBB,0xB7,0xB3,0xAE,0xA9,0xA5,0x9F,0x9A,0x95,0x8F,0x8A,0x84,0x7E,0x78,0x72,0x6D,0x67,0x61,0x5B,0x56,0x50,0x4A,0x45,0x40,0x3B,0x36,0x32,0x2D,0x29,0x25,0x22,0x1F,0x1C,0x19,0x17,0x15,0x13,0x12,0x11,0x10,0x10,0x10,0x11,0x11,0x13,0x14,0x16,0x18,0x1B,0x1D,0x21,0x24,0x28,0x2C,0x30,0x34,0x39,0x3E,0x43,0x48,0x4E,0x53,0x59,0x5F,0x64,0x6A,0x70,0x76,0x7C,0x82,0x87,0x8D,0x93,0x98,0x9D,0xA3,0xA8,0xAC,0xB1,0xB5,0xB9,0xBD,0xC0,0xC4,0xC7,0xC9,0xCB,0xCD,0xCF,0xD0,0xD1,0xD2,0xD2,0xD2,0xD1,0xD0,0xCF,0xCE,0xCC,0xCA,0xC7,0xC4,0xC1,0xBD,0xBA,0xB6,0xB1,0xAD,0xA8,0xA3,0x9E,0x99,0x94,0x8E,0x88,0x83,0x7D,0x77,0x71,0x6B,0x65,0x60,0x5A,0x54,0x4F,0x49,0x44,0x3F,0x3A,0x35,0x31,0x2C,0x28,0x25,0x21,0x1E,0x1B,0x18,0x16,0x14,0x13,0x12,0x11,0x10,0x10,0x10,0x11,0x12,0x13,0x15,0x16,0x19,0x1B,0x1E,0x21,0x25,0x29,0x2D,0x31,0x36,0x3A,0x3F,0x44,0x4A,0x4F,0x55,0x5A,0x60,0x66,0x6C,0x72,0x77,0x7D,0x83,0x89,0x8F,0x94,0x99,0x9F,0xA4,0xA9,0xAD,0xB2,0xB6,0xBA,0xBE,0xC1,0xC4,0xC7,0xCA,0xCC,0xCE,0xCF,0xD1,0xD1,0xD2,0xD2,0xD2,0xD1,0xD0,0xCF,0xCD,0xCB,0xC9,0xC6,0xC3,0xC0,0xBD,0xB9,0xB5,0xB0,0xAC,0xA7,0xA2,0x9D,0x98,0x92,0x8D,0x87,0x81,0x7B,0x76,0x70,0x6A,0x64,0x5E,0x58,0x53,0x4D,0x48,0x43,0x3E,0x39,0x34,0x30,0x2B,0x27,0x24,0x20,0x1D,0x1A,0x18,0x16,0x14,0x12,0x11,0x11,0x10,0x10,0x10,0x11,0x12,0x13,0x15,0x17,0x19,0x1C,0x1F,0x22,0x26,0x2A,0x2E,0x32,0x37,0x3C,0x40,0x46,0x4B,0x50,0x56,0x5C,0x62,0x67,0x6D,0x73,0x79,0x7F,0x85,0x8A,0x90,0x95,0x9B,0xA0,0xA5,0xAA,0xAF,0xB3,0xB7,0xBB,0xBF,0xC2,0xC5,0xC8,0xCA,0xCC,0xCE,0xD0,0xD1,0xD2,0xD2,0xD2,0xD2,0xD1,0xD0,0xCF,0xCD,0xCB,0xC8,0xC6,0xC3,0xBF,0xBC,0xB8,0xB4,0xAF,0xAB,0xA6,0xA1,0x9C,0x96,0x91,0x8B,0x86,0x80,0x7A,0x74,0x6E,0x68,0x63,0x5D,0x57,0x51,0x4C,0x47,0x41,0x3C,0x38,0x33,0x2F,0x2A,0x26,0x23,0x20,0x1C,0x1A,0x17,0x15,0x14,0x12,0x11,0x10,0x10,0x10,0x10,0x11,0x12,0x14,0x15,0x18,0x1A,0x1D,0x20,0x23,0x27,0x2B,0x2F,0x33,0x38,0x3D,0x42,0x47,0x4C,0x52,0x57,0x5D,0x63,0x69,0x6F,0x74,0x7A,0x80,0x86,0x8C,0x91,0x97,0x9C,0xA1,0xA6,0xAB,0xB0,0xB4,0xB8,0xBC,0xC0,0xC3,0xC6,0xC9,0xCB,0xCD,0xCF,0xD0,0xD1,0xD2,0xD2,0xD2,0xD1,0xD1,0xD0,0xCE,0xCC,0xCA,0xC8,0xC5,0xC2,0xBE,0xBB,0xB7,0xB3,0xAE,0xAA,0xA5,0xA0,0x9A,0x95,0x8F,0x8A,0x84,0x7E,0x79,0x73,0x6D,0x67,0x61,0x5B,0x56,0x50,0x4B,0x45,0x40,0x3B,0x36,0x32,0x2D,0x29,0x26,0x22,0x1F,0x1C,0x19,0x17,0x15,0x13,0x12,0x11,0x10,0x10,0x10,0x11,0x11,0x13,0x14,0x16,0x18,0x1B,0x1D,0x21,0x24,0x28,0x2C,0x30,0x34,0x39,0x3E,0x43,0x48,0x4E,0x53,0x59,0x5F,0x64,0x6A,0x70,0x76,0x7C,0x82,0x87,0x8D,0x93,0x98,0x9D,0xA2,0xA7,0xAC,0xB1,0xB5,0xB9,0xBD,0xC0,0xC4,0xC7,0xC9,0xCB,0xCD,0xCF,0xD0,0xD1,0xD2,0xD2,0xD2,0xD1,0xD0,0xCF,0xCE,0xCC,0xCA,0xC7,0xC4,0xC1,0xBE,0xBA,0xB6,0xB2,0xAD,0xA8,0xA3,0x9E,0x99,0x94,0x8E,0x88,0x83,0x7D,0x77,0x71,0x6B,0x66,0x60,0x5A,0x54,0x4F,0x49,0x44,0x3F,0x3A,0x35,0x31,0x2C,0x28,0x25,0x21,0x1E,0x1B,0x19,0x16,0x14,0x13,0x12,0x11,0x10,0x10,0x10,0x11,0x12,0x13,0x14,0x16,0x19,0x1B,0x1E,0x21,0x25,0x29,0x2D,0x31,0x35,0x3A,0x3F,0x44,0x4A,0x4F,0x55,0x5A,0x60,0x66,0x6C,0x71,0x77,0x7D,0x83,0x89,0x8E,0x94,0x99,0x9F,0xA4,0xA9,0xAD,0xB2,0xB6,0xBA,0xBE,0xC1,0xC4,0xC7,0xCA,0xCC,0xCE,0xCF,0xD1,0xD1,0xD2,0xD2,0xD2,0xD1,0xD0,0xCF,0xCD,0xCB,0xC9,0xC6,0xC3,0xC0,0xBD,0xB9,0xB5,0xB1,0xAC,0xA7,0xA2,0x9D,0x98,0x92,0x8D,0x87,0x81,0x7C,0x76,0x70,0x6A,0x64,0x5E,0x59,0x53,0x4D,0x48,0x43,0x3E,0x39,0x34,0x30,0x2B,0x28,0x24,0x20,0x1D,0x1A,0x18,0x16,0x14,0x13,0x11,0x11,0x10,0x10,0x10,0x11,0x12,0x13,0x15,0x17,0x19,0x1C,0x1F,0x22,0x26,0x2A,0x2E,0x32,0x37,0x3B,0x40,0x46,0x4B,0x50,0x56,0x5C,0x61,0x67,0x6D,0x73,0x79,0x7F,0x84,0x8A,0x90,0x95,0x9B,0xA0,0xA5,0xAA,0xAE,0xB3,0xB7,0xBB,0xBF,0xC2,0xC5,0xC8,0xCA,0xCC,0xCE,0xD0,0xD1,0xD1,0xD2,0xD2,0xD2,0xD1,0xD0,0xCF,0xCD,0xCB,0xC8,0xC6,0xC3,0xBF,0xBC,0xB8,0xB4,0xAF,0xAB,0xA6,0xA1,0x9C,0x96,0x91,0x8B,0x86,0x80,0x7A,0x74,0x6E,0x69,0x63,0x5D,0x57,0x52,0x4C,0x47,0x42,0x3D,0x38,0x33,0x2F,0x2A,0x27,0x23,0x20,0x1D,0x1A,0x17,0x15,0x14,0x12,0x11,0x10,0x10,0x10,0x10,0x11,0x12,0x14,0x15,0x17,0x1A,0x1D,0x20,0x23,0x27,0x2B,0x2F,0x33,0x38,0x3D,0x42,0x47,0x4C,0x52,0x57,0x5D,0x63,0x69,0x6E,0x74,0x7A,0x80,0x86,0x8B,0x91,0x97,0x9C,0xA1,0xA6,0xAB,0xAF,0xB4,0xB8,0xBC,0xBF,0xC3,0xC6,0xC8,0xCB,0xCD,0xCF,0xD0,0xD1,0xD2,0xD2,0xD2,0xD1,0xD1,0xD0,0xCE,0xCC,0xCA,0xC8,0xC5,0xC2,0xBF,0xBB,0xB7,0xB3,0xAE,0xAA,0xA5,0xA0,0x9B,0x95,0x90,0x8A,0x84,0x7F,0x79,0x73,0x6D,0x67,0x61,0x5C,0x56,0x50,0x4B,0x45,0x40,0x3B,0x37,0x32,0x2E,0x2A,0x26,0x22,0x1F,0x1C,0x19,0x17,0x15,0x13,0x12,0x11,0x10,0x10,0x10,0x11,0x11,0x13,0x14,0x16,0x18,0x1B,0x1D,0x20,0x24,0x28,0x2C,0x30,0x34,0x39,0x3E,0x43,0x48,0x4D,0x53,0x59,0x5E,0x64,0x6A,0x70,0x76,0x7C,0x81,0x87,0x8D,0x92,0x98,0x9D,0xA2,0xA7,0xAC,0xB1,0xB5,0xB9,0xBD,0xC0,0xC4,0xC6,0xC9,0xCB,0xCD,0xCF,0xD0,0xD1,0xD2,0xD2,0xD2,0xD1,0xD0,0xCF,0xCE,0xCC,0xCA,0xC7,0xC4,0xC1,0xBE,0xBA,0xB6,0xB2,0xAD,0xA9,0xA4,0x9E,0x99,0x94,0x8E,0x89,0x83,0x7D,0x77,0x71,0x6C,0x66,0x60,0x5A,0x54,0x4F,0x49,0x44,0x3F,0x3A,0x35,0x31,0x2D,0x29,0x25,0x21,0x1E,0x1B,0x19,0x16,0x14,0x13,0x12,0x11,0x10,0x10,0x10,0x11,0x12,0x13,0x14,0x16,0x19,0x1B,0x1E,0x21,0x25,0x28,0x2D,0x31,0x35,0x3A,0x3F,0x44,0x49,0x4F,0x54,0x5A,0x60,0x66,0x6B,0x71,0x77,0x7D,0x83,0x89,0x8E,0x94,0x99,0x9E,0xA4,0xA8,0xAD,0xB2,0xB6,0xBA,0xBE,0xC1,0xC4,0xC7,0xCA,0xCC,0xCE,0xCF,0xD0,0xD1,0xD2,0xD2,0xD2,0xD1,0xD0,0xCF,0xCD,0xCB,0xC9,0xC6,0xC4,0xC0,0xBD,0xB9,0xB5,0xB1,0xAC,0xA7,0xA2,0x9D,0x98,0x92,0x8D,0x87,0x81,0x7C,0x76,0x70,0x6A,0x64,0x5E,0x59,0x53,0x4E,0x48,0x43,0x3E,0x39,0x34,0x30,0x2C,0x28,0x24,0x20,0x1D,0x1B,0x18,0x16,0x14,0x13,0x11,0x11,0x10,0x10,0x10,0x11,0x12,0x13,0x15,0x17,0x19,0x1C,0x1F,0x22,0x26,0x29,0x2E,0x32,0x36,0x3B,0x40,0x45,0x4B,0x50,0x56,0x5B,0x61,0x67,0x6D,0x73,0x79,0x7E,0x84,0x8A,0x90,0x95,0x9A,0xA0,0xA5,0xAA,0xAE,0xB3,0xB7,0xBB,0xBF,0xC2,0xC5,0xC8,0xCA,0xCC,0xCE,0xD0,0xD1,0xD1,0xD2,0xD2,0xD2,0xD1,0xD0,0xCF,0xCD,0xCB,0xC8,0xC6,0xC3,0xBF,0xBC,0xB8,0xB4,0xB0,0xAB,0xA6,0xA1,0x9C,0x97,0x91,0x8C,0x86,0x80,0x7A,0x74,0x6F,0x69,0x63,0x5D,0x57,0x52,0x4C,0x47,0x42,0x3D,0x38,0x33,0x2F,0x2B,0x27,0x23,0x20,0x1D,0x1A,0x17,0x15,0x14,0x12,0x11,0x10,0x10,0x10,0x10,0x11,0x12,0x14,0x15,0x17,0x1A,0x1D,0x20,0x23,0x27,0x2A,0x2F,0x33,0x38,0x3C,0x41,0x47,0x4C,0x52,0x57,0x5D,0x63,0x68,0x6E,0x74,0x7A,0x80,0x86,0x8B,0x91,0x96,0x9C,0xA1,0xA6,0xAB,0xAF,0xB4,0xB8,0xBC,0xBF,0xC3,0xC6,0xC8,0xCB,0xCD,0xCF,0xD0,0xD1,0xD2,0xD2,0xD2,0xD1,0xD1,0xD0,0xCE,0xCC,0xCA,0xC8,0xC5,0xC2,0xBF,0xBB,0xB7,0xB3,0xAE,0xAA,0xA5,0xA0,0x9B,0x95,0x90,0x8A,0x84,0x7F,0x79,0x73,0x6D,0x67,0x61,0x5C,0x56,0x50,0x4B,0x46,0x40,0x3B,0x37,0x32,0x2E,0x2A,0x26,0x22,0x1F,0x1C,0x19,0x17,0x15,0x13,0x12,0x11,0x10,0x10,0x10,0x11,0x11,0x12,0x14,0x16,0x18,0x1A,0x1D,0x20,0x24,0x27,0x2B,0x30,0x34,0x39,0x3E,0x43,0x48,0x4D,0x53,0x59,0x5E,0x64,0x6A,0x70,0x76,0x7B,0x81,0x87,0x8D,0x92,0x98,0x9D,0xA2,0xA7,0xAC,0xB0,0xB5,0xB9,0xBD,0xC0,0xC3,0xC6,0xC9,0xCB,0xCD,0xCF,0xD0,0xD1,0xD2,0xD2,0xD2,0xD1,0xD1,0xCF,0xCE,0xCC,0xCA,0xC7,0xC4,0xC1,0xBE,0xBA,0xB6,0xB2,0xAD,0xA9,0xA4,0x9F,0x99,0x94,0x8E,0x89,0x83,0x7D,0x77,0x72,0x6C,0x66,0x60,0x5A,0x55,0x4F,0x4A,0x44,0x3F,0x3A,0x36,0x31,0x2D,0x29,0x25,0x21,0x1E,0x1B,0x19,0x16,0x15,0x13,0x12,0x11,0x10,0x10,0x10,0x11,0x12,0x13,0x14,0x16,0x19,0x1B,0x1E,0x21,0x25,0x28,0x2C,0x31,0x35,0x3A,0x3F,0x44,0x49,0x4F,0x54,0x5A,0x60,0x65,0x6B,0x71,0x77,0x7D,0x83,0x88,0x8E,0x94,0x99,0x9E,0xA3,0xA8,0xAD,0xB2,0xB6,0xBA,0xBE,0xC1,0xC4,0xC7,0xCA,0xCC,0xCE,0xCF,0xD0,0xD1,0xD2,0xD2,0xD2,0xD1,0xD0,0xCF,0xCD,0xCB,0xC9,0xC7,0xC4,0xC0,0xBD,0xB9,0xB5,0xB1,0xAC,0xA7,0xA3,0x9D,0x98,0x93,0x8D,0x87,0x82,0x7C,0x76,0x70,0x6A,0x64,0x5F,0x59,0x53,0x4E,0x48,0x43,0x3E,0x39,0x34,0x30,0x2C,0x28,0x24,0x21,0x1D,0x1B,0x18,0x16,0x14,0x13,0x11,0x11,0x10,0x10,0x10,0x11,0x12,0x13,0x15,0x17,0x19,0x1C,0x1F,0x22,0x26,0x29,0x2D,0x32,0x36,0x3B,0x40,0x45,0x4B,0x50,0x56,0x5B,0x61,0x67,0x6D,0x73,0x78,0x7E,0x84,0x8A,0x8F,0x95,0x9A,0xA0,0xA5,0xA9,0xAE,0xB3,0xB7,0xBB,0xBE,0xC2,0xC5,0xC8,0xCA,0xCC,0xCE,0xD0,0xD1,0xD1,0xD2,0xD2,0xD2,0xD1,0xD0,0xCF,0xCD,0xCB,0xC9,0xC6,0xC3,0xC0,0xBC,0xB8,0xB4,0xB0,0xAB,0xA6,0xA1,0x9C,0x97,0x91,0x8C,0x86,0x80,0x7A,0x75,0x6F,0x69,0x63,0x5D,0x58,0x52,0x4C,0x47,0x42,0x3D,0x38,0x33,0x2F,0x2B,0x27,0x23,0x20,0x1D,0x1A,0x18,0x15,0x14,0x12,0x11,0x10,0x10,0x10,0x10,0x11,0x12,0x14,0x15,0x17,0x1A,0x1C,0x1F,0x23,0x26,0x2A,0x2E,0x33,0x37,0x3C,0x41,0x47,0x4C,0x51,0x57,0x5D,0x62,0x68,0x6E,0x74,0x7A,0x80,0x85,0x8B,0x91,0x96,0x9C,0xA1,0xA6,0xAB,0xAF,0xB4,0xB8,0xBC,0xBF,0xC3,0xC6,0xC8,0xCB,0xCD,0xCE,0xD0,0xD1,0xD2,0xD2,0xD2,0xD2,0xD1,0xD0,0xCE,0xCC,0xCA,0xC8,0xC5,0xC2,0xBF,0xBB,0xB7,0xB3,0xAF,0xAA,0xA5,0xA0,0x9B,0x95,0x90,0x8A,0x85,0x7F,0x79,0x73,0x6D,0x67,0x62,0x5C,0x56,0x51,0x4B,0x46,0x41,0x3C,0x37,0x32,0x2E,0x2A,0x26,0x22,0x1F,0x1C,0x19,0x17,0x15,0x13,0x12,0x11,0x10,0x10,0x10,0x11,0x11,0x12,0x14,0x16,0x18,0x1A,0x1D,0x20,0x24,0x27,0x2B,0x30,0x34,0x39,0x3E,0x43,0x48,0x4D,0x53,0x58,0x5E,0x64,0x6A,0x70,0x75,0x7B,0x81,0x87,0x8D,0x92,0x98,0x9D,0xA2,0xA7,0xAC,0xB0,0xB5,0xB9,0xBD,0xC0,0xC3,0xC6,0xC9,0xCB,0xCD,0xCF,0xD0,0xD1,0xD2,0xD2,0xD2,0xD1,0xD1,0xCF,0xCE,0xCC,0xCA,0xC7,0xC4,0xC1,0xBE,0xBA,0xB6,0xB2,0xAD,0xA9,0xA4,0x9F,0x9A,0x94,0x8F,0x89,0x83,0x7D,0x78,0x72,0x6C,0x66,0x60,0x5A,0x55,0x4F,0x4A,0x44,0x3F,0x3A,0x36,0x31,0x2D,0x29,0x25,0x21,0x1E,0x1B,0x19,0x16,0x15,0x13,0x12,0x11,0x10,0x10,0x10,0x11,0x12,0x13,0x14,0x16,0x18,0x1B,0x1E,0x21,0x25,0x28,0x2C,0x31,0x35,0x3A,0x3F,0x44,0x49,0x4F,0x54,0x5A,0x5F,0x65,0x6B,0x71
};