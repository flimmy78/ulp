/*
 * 	miaofng@2009 initial version
 */

#include "config.h"
#include "vvt/misfire.h"
#include <stdio.h>
#include <stdlib.h>
#include "mcao.h"

//global

//private
static short misfire_speed; //unit: Hz, 1~32867hz(rpm)
static short misfire_strength; //0~100%
static short *misfire_curve; //0~+/-100%, 100% = 32867
static short misfire_offset;

//misfire - 1 cylinder, y = [-1*sin(t(1:60)*4*pi),1/3*sin(t(1:180)*4*pi/3)];
static const short curve_a[240] = {	\
	0x0000, 0xf94e, 0xf29f, 0xebfb, 0xe564, 0xdee0, 0xd873, 0xd222, \
	0xcbf1, 0xc5e5, 0xc001, 0xba4a, 0xb4c5, 0xaf74, 0xaa5b, 0xa57f, \
	0xa0e2, 0x9c88, 0x9873, 0x94a8, 0x9127, 0x8df5, 0x8b12, 0x8882, \
	0x8645, 0x845e, 0x82ce, 0x8195, 0x80b5, 0x802e, 0x8001, 0x802e, \
	0x80b5, 0x8195, 0x82ce, 0x845e, 0x8645, 0x8882, 0x8b12, 0x8df5, \
	0x9127, 0x94a8, 0x9873, 0x9c88, 0xa0e2, 0xa57f, 0xaa5b, 0xaf74, \
	0xb4c5, 0xba4a, 0xc001, 0xc5e5, 0xcbf1, 0xd222, 0xd873, 0xdee0, \
	0xe564, 0xebfb, 0xf29f, 0xf94e, 0x0000, 0x00be, 0x017d, 0x023b, \
	0x02f9, 0x03b7, 0x0475, 0x0533, 0x05f0, 0x06ac, 0x0768, 0x0824, \
	0x08de, 0x0998, 0x0a52, 0x0b0a, 0x0bc2, 0x0c79, 0x0d2f, 0x0de3, \
	0x0e97, 0x0f4a, 0x0ffb, 0x10ab, 0x115a, 0x1207, 0x12b4, 0x135e, \
	0x1407, 0x14af, 0x1555, 0x15f9, 0x169b, 0x173c, 0x17db, 0x1878, \
	0x1913, 0x19ad, 0x1a44, 0x1ad9, 0x1b6c, 0x1bfd, 0x1c8c, 0x1d19, \
	0x1da3, 0x1e2b, 0x1eb0, 0x1f34, 0x1fb4, 0x2033, 0x20ae, 0x2128, \
	0x219e, 0x2212, 0x2284, 0x22f3, 0x235f, 0x23c8, 0x242e, 0x2492, \
	0x24f3, 0x2550, 0x25ab, 0x2603, 0x2658, 0x26aa, 0x26fa, 0x2746, \
	0x278f, 0x27d4, 0x2817, 0x2857, 0x2893, 0x28cd, 0x2903, 0x2936, \
	0x2965, 0x2992, 0x29bb, 0x29e1, 0x2a04, 0x2a23, 0x2a40, 0x2a58, \
	0x2a6e, 0x2a80, 0x2a8f, 0x2a9b, 0x2aa3, 0x2aa8, 0x2aaa, 0x2aa8, \
	0x2aa3, 0x2a9b, 0x2a8f, 0x2a80, 0x2a6e, 0x2a58, 0x2a40, 0x2a23, \
	0x2a04, 0x29e1, 0x29bb, 0x2992, 0x2965, 0x2936, 0x2903, 0x28cd, \
	0x2893, 0x2857, 0x2817, 0x27d4, 0x278f, 0x2746, 0x26fa, 0x26aa, \
	0x2658, 0x2603, 0x25ab, 0x2550, 0x24f3, 0x2492, 0x242e, 0x23c8, \
	0x235f, 0x22f3, 0x2284, 0x2212, 0x219e, 0x2128, 0x20ae, 0x2033, \
	0x1fb4, 0x1f34, 0x1eb0, 0x1e2b, 0x1da3, 0x1d19, 0x1c8c, 0x1bfd, \
	0x1b6c, 0x1ad9, 0x1a44, 0x19ad, 0x1913, 0x1878, 0x17db, 0x173c, \
	0x169b, 0x15f9, 0x1555, 0x14af, 0x1407, 0x135e, 0x12b4, 0x1207, \
	0x115a, 0x10ab, 0x0ffb, 0x0f4a, 0x0e97, 0x0de3, 0x0d2f, 0x0c79, \
	0x0bc2, 0x0b0a, 0x0a52, 0x0998, 0x08de, 0x0824, 0x0768, 0x06ac, \
	0x05f0, 0x0533, 0x0475, 0x03b7, 0x02f9, 0x023b, 0x017d, 0x00be, \
};

//misfire - 2 cylinder - type 1-2, y = -sin(2*pi*t)
static const short curve_b[240] = {	\
	0x0000, 0xfca7, 0xf94e, 0xf5f6, 0xf29f, 0xef4c, 0xebfb, 0xe8ad, \
	0xe564, 0xe21f, 0xdee0, 0xdba6, 0xd873, 0xd547, 0xd222, 0xcf05, \
	0xcbf1, 0xc8e6, 0xc5e5, 0xc2ed, 0xc001, 0xbd20, 0xba4a, 0xb781, \
	0xb4c5, 0xb215, 0xaf74, 0xace0, 0xaa5b, 0xa7e5, 0xa57f, 0xa328, \
	0xa0e2, 0x9eac, 0x9c88, 0x9a75, 0x9873, 0x9684, 0x94a8, 0x92de, \
	0x9127, 0x8f84, 0x8df5, 0x8c79, 0x8b12, 0x89c0, 0x8882, 0x8759, \
	0x8645, 0x8547, 0x845e, 0x838b, 0x82ce, 0x8226, 0x8195, 0x811a, \
	0x80b5, 0x8067, 0x802e, 0x800d, 0x8001, 0x800d, 0x802e, 0x8067, \
	0x80b5, 0x811a, 0x8195, 0x8226, 0x82ce, 0x838b, 0x845e, 0x8547, \
	0x8645, 0x8759, 0x8882, 0x89c0, 0x8b12, 0x8c79, 0x8df5, 0x8f84, \
	0x9127, 0x92de, 0x94a8, 0x9684, 0x9873, 0x9a75, 0x9c88, 0x9eac, \
	0xa0e2, 0xa328, 0xa57f, 0xa7e5, 0xaa5b, 0xace0, 0xaf74, 0xb215, \
	0xb4c5, 0xb781, 0xba4a, 0xbd20, 0xc001, 0xc2ed, 0xc5e5, 0xc8e6, \
	0xcbf1, 0xcf05, 0xd222, 0xd547, 0xd873, 0xdba6, 0xdee0, 0xe21f, \
	0xe564, 0xe8ad, 0xebfb, 0xef4c, 0xf29f, 0xf5f6, 0xf94e, 0xfca7, \
	0x0000, 0x0359, 0x06b2, 0x0a0a, 0x0d61, 0x10b4, 0x1405, 0x1753, \
	0x1a9c, 0x1de1, 0x2120, 0x245a, 0x278d, 0x2ab9, 0x2dde, 0x30fb, \
	0x340f, 0x371a, 0x3a1b, 0x3d13, 0x3fff, 0x42e0, 0x45b6, 0x487f, \
	0x4b3b, 0x4deb, 0x508c, 0x5320, 0x55a5, 0x581b, 0x5a81, 0x5cd8, \
	0x5f1e, 0x6154, 0x6378, 0x658b, 0x678d, 0x697c, 0x6b58, 0x6d22, \
	0x6ed9, 0x707c, 0x720b, 0x7387, 0x74ee, 0x7640, 0x777e, 0x78a7, \
	0x79bb, 0x7ab9, 0x7ba2, 0x7c75, 0x7d32, 0x7dda, 0x7e6b, 0x7ee6, \
	0x7f4b, 0x7f99, 0x7fd2, 0x7ff3, 0x7fff, 0x7ff3, 0x7fd2, 0x7f99, \
	0x7f4b, 0x7ee6, 0x7e6b, 0x7dda, 0x7d32, 0x7c75, 0x7ba2, 0x7ab9, \
	0x79bb, 0x78a7, 0x777e, 0x7640, 0x74ee, 0x7387, 0x720b, 0x707c, \
	0x6ed9, 0x6d22, 0x6b58, 0x697c, 0x678d, 0x658b, 0x6378, 0x6154, \
	0x5f1e, 0x5cd8, 0x5a81, 0x581b, 0x55a5, 0x5320, 0x508c, 0x4deb, \
	0x4b3b, 0x487f, 0x45b6, 0x42e0, 0x3fff, 0x3d13, 0x3a1b, 0x371a, \
	0x340f, 0x30fb, 0x2dde, 0x2ab9, 0x278d, 0x245a, 0x2120, 0x1de1, \
	0x1a9c, 0x1753, 0x1405, 0x10b4, 0x0d61, 0x0a0a, 0x06b2, 0x0359, \
};

//misfire - 2 cylinder - type 1-3, y = -sin(4*pi*t)
static const short curve_c[240] = {	\
	0x0000, 0xf94e, 0xf29f, 0xebfb, 0xe564, 0xdee0, 0xd873, 0xd222, \
	0xcbf1, 0xc5e5, 0xc001, 0xba4a, 0xb4c5, 0xaf74, 0xaa5b, 0xa57f, \
	0xa0e2, 0x9c88, 0x9873, 0x94a8, 0x9127, 0x8df5, 0x8b12, 0x8882, \
	0x8645, 0x845e, 0x82ce, 0x8195, 0x80b5, 0x802e, 0x8001, 0x802e, \
	0x80b5, 0x8195, 0x82ce, 0x845e, 0x8645, 0x8882, 0x8b12, 0x8df5, \
	0x9127, 0x94a8, 0x9873, 0x9c88, 0xa0e2, 0xa57f, 0xaa5b, 0xaf74, \
	0xb4c5, 0xba4a, 0xc001, 0xc5e5, 0xcbf1, 0xd222, 0xd873, 0xdee0, \
	0xe564, 0xebfb, 0xf29f, 0xf94e, 0x0000, 0x06b2, 0x0d61, 0x1405, \
	0x1a9c, 0x2120, 0x278d, 0x2dde, 0x340f, 0x3a1b, 0x3fff, 0x45b6, \
	0x4b3b, 0x508c, 0x55a5, 0x5a81, 0x5f1e, 0x6378, 0x678d, 0x6b58, \
	0x6ed9, 0x720b, 0x74ee, 0x777e, 0x79bb, 0x7ba2, 0x7d32, 0x7e6b, \
	0x7f4b, 0x7fd2, 0x7fff, 0x7fd2, 0x7f4b, 0x7e6b, 0x7d32, 0x7ba2, \
	0x79bb, 0x777e, 0x74ee, 0x720b, 0x6ed9, 0x6b58, 0x678d, 0x6378, \
	0x5f1e, 0x5a81, 0x55a5, 0x508c, 0x4b3b, 0x45b6, 0x3fff, 0x3a1b, \
	0x340f, 0x2dde, 0x278d, 0x2120, 0x1a9c, 0x1405, 0x0d61, 0x06b2, \
	0x0000, 0xf94e, 0xf29f, 0xebfb, 0xe564, 0xdee0, 0xd873, 0xd222, \
	0xcbf1, 0xc5e5, 0xc001, 0xba4a, 0xb4c5, 0xaf74, 0xaa5b, 0xa57f, \
	0xa0e2, 0x9c88, 0x9873, 0x94a8, 0x9127, 0x8df5, 0x8b12, 0x8882, \
	0x8645, 0x845e, 0x82ce, 0x8195, 0x80b5, 0x802e, 0x8001, 0x802e, \
	0x80b5, 0x8195, 0x82ce, 0x845e, 0x8645, 0x8882, 0x8b12, 0x8df5, \
	0x9127, 0x94a8, 0x9873, 0x9c88, 0xa0e2, 0xa57f, 0xaa5b, 0xaf74, \
	0xb4c5, 0xba4a, 0xc001, 0xc5e5, 0xcbf1, 0xd222, 0xd873, 0xdee0, \
	0xe564, 0xebfb, 0xf29f, 0xf94e, 0x0000, 0x06b2, 0x0d61, 0x1405, \
	0x1a9c, 0x2120, 0x278d, 0x2dde, 0x340f, 0x3a1b, 0x3fff, 0x45b6, \
	0x4b3b, 0x508c, 0x55a5, 0x5a81, 0x5f1e, 0x6378, 0x678d, 0x6b58, \
	0x6ed9, 0x720b, 0x74ee, 0x777e, 0x79bb, 0x7ba2, 0x7d32, 0x7e6b, \
	0x7f4b, 0x7fd2, 0x7fff, 0x7fd2, 0x7f4b, 0x7e6b, 0x7d32, 0x7ba2, \
	0x79bb, 0x777e, 0x74ee, 0x720b, 0x6ed9, 0x6b58, 0x678d, 0x6378, \
	0x5f1e, 0x5a81, 0x55a5, 0x508c, 0x4b3b, 0x45b6, 0x3fff, 0x3a1b, \
	0x340f, 0x2dde, 0x278d, 0x2120, 0x1a9c, 0x1405, 0x0d61, 0x06b2, \
};

void misfire_Init(void)
{
#if CONFIG_VVT_MISFIRE_DEBUG == 1
	mcao_Init();
#endif

	misfire_strength = 0;
	misfire_curve = 0;
}

void misfire_SetSpeed(short hz)
{
	misfire_speed = hz << 1;
}

short misfire_GetSpeed(short gear)
{
	int speed;
	int coef, index;
	coef = 0;
	
	speed = misfire_speed;
	if(misfire_curve != 0) {
		gear += (gear >= misfire_offset) ? 0 : 240;
		index = gear - misfire_offset;
		coef = misfire_curve[index];
		coef *= misfire_strength;
		coef >>= 15;
		coef *= speed;
		coef >>= 15;
	}

#if CONFIG_VVT_MISFIRE_DEBUG == 1
	{
		int mv;
		//ch0, current gear nr
		mv = 2000;
		mv *= gear;
		mv /= 240;
		mcao_SetVolt(0, mv);
		
		//ch1, misfire speed
		mv = 2000 >> 1;
		mv *= coef;
		mv /= speed;
		mv += 2500 >> 1;
		mcao_SetVolt(1, mv);
	}
#endif

	speed += coef;
	return speed;
}

void misfire_Config(short strength, short pattern)
{
	int ch, x, y;
	if(pattern == 0) 
		return;
	x = y = -1;
	misfire_strength = strength;
	for(ch = 0; ch < 4; ch ++)
	{
		if(pattern & 0x01) {
			if(x == -1)
				x = ch;
			else {
				y = ch;
				break; //ignore more than 2 misfire channels here!!!
			}
		}
		pattern >>= 1;
	}
	
	//curve type?
	misfire_curve = (short *)curve_a;
	if(y != -1) {
		misfire_curve = (short *)curve_b;
		if(y - x == 2)
			misfire_curve = (short *)curve_c;
	}
	
	//offset?<tdc?>
	if(y - x == 3) //A - - D situation, then curve start from cylinder D 
		x = y;
	misfire_offset = (20 + x * 30) << 1;
}

