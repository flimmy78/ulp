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
