/*
 * Copyright (c) 2018 Nuvoton technology corporation
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
 
#ifndef _NUC970_CRYPTO_H_
#define _NUC970_CRYPTO_H_

#include <linux/types.h>
#include <linux/ioctl.h>

#define NVT_AES_KEYSZ_128			(0x00000000)			/*!<AES 128 bits key */
#define NVT_AES_KEYSZ_192			(0x00000004)			/*!<AES 192 bits key */
#define NVT_AES_KEYSZ_256			(0x00000008)			/*!<AES 256 bits key */
#define NVT_AES_ECB_MODE			(0x00000000)			/*!<AES ECB mode */
#define NVT_AES_CBC_MODE			(0x00000100)			/*!<AES CBC mode */
#define NVT_AES_CFB_MODE			(0x00000200)			/*!<AES CFB mode */
#define NVT_AES_OFB_MODE			(0x00000300)			/*!<AES OFB mode */
#define NVT_AES_CTR_MODE			(0x00000400)			/*!<AES CTR mode */
#define NVT_AES_CBCCS1_MODE			(0x00001000)			/*!<AES CBC CS1 mode */
#define NVT_AES_CBCCS2_MODE			(0x00001100)			/*!<AES CBC CS2 mode */
#define NVT_AES_CBCCS3_MODE			(0x00001200)			/*!<AES CBC CS3 mode */
#define NVT_AES_ENCRYPT				(0x00010000)			/*!<AES engine execute encryption */
#define NVT_AES_DECRYPT				(0x00000000)			/*!<AES engine execute decryption */
#define NVT_AES_OUTSWAP				(0x00400000)			/*!<AES engine output data transform */
#define NVT_AES_INSWAP				(0x00800000)			/*!<AES engine input data transform */

#define NVT_SHA1					(0x00000000)			/*!<SHA1 */
#define NVT_SHA224					(0x00000500)			/*!<SHA224 */
#define NVT_SHA256					(0x00000400)			/*!<SHA256 */
#define NVT_SHA384					(0x00000700)			/*!<SHA384 */
#define NVT_SHA512					(0x00000600)			/*!<SHA512 */
#define NVT_SHA_OUTSWAP				(0x00400000)			/*!<HMAC engine output data transform */
#define NVT_SHA_INSWAP				(0x00800000)			/*!<HMAC engine input data transform */

#define CRYPTO_IOC_MAGIC		'C'

#define AES_IOC_SET_MODE		_IOW(CRYPTO_IOC_MAGIC,  1, unsigned long)
#define AES_IOC_SET_LEN			_IOW(CRYPTO_IOC_MAGIC,  2, unsigned long)
#define AES_IOC_GET_BUFSIZE     _IOW(CRYPTO_IOC_MAGIC,  3, unsigned long *)
#define AES_IOC_SET_IV			_IOW(CRYPTO_IOC_MAGIC,  5, unsigned long *)
#define AES_IOC_SET_KEY			_IOW(CRYPTO_IOC_MAGIC,  6, unsigned long *)
#define AES_IOC_START			_IOW(CRYPTO_IOC_MAGIC,  8, unsigned long)
#define AES_IOC_C_START			_IOW(CRYPTO_IOC_MAGIC,  9, unsigned long)
#define AES_IOC_UPDATE_IV		_IOW(CRYPTO_IOC_MAGIC, 11, unsigned long *)

#define SHA_IOC_INIT	  		_IOW(CRYPTO_IOC_MAGIC, 21, unsigned long)
#define SHA_IOC_FINISH			_IOW(CRYPTO_IOC_MAGIC, 25, unsigned long)


#endif

