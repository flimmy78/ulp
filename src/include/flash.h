/* flash.h
 * 	dusk@2010 initial version
 *	miaofng@2010, revise this file to a common flash api header<device independent>
 */
#ifndef __FLASH_H_
#define __FLASH_H_

#include <stddef.h>

//erase n-pages of flash sectors, which is given by address of dest(must be PAGE_SIZE aligned)
int flash_Erase(void *dest, size_t n);

/*write/read flash,  return bytes have been wrote/read
note: 
	1, flash mem space must be erased before program
	2, dest, n must be 4 bytes aligned
*/
int flash_Write(void *dest, const void *src, size_t n);
int flash_Read(void *dest, const void *src, size_t n);

#endif /*__FLASH_H_*/