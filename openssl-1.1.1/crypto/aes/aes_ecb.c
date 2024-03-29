/*
 * Copyright 2002-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <assert.h>

#include <openssl/aes.h>
#include "aes_locl.h"


#ifdef USE_NUVOTON_AES

void AES_ecb_encrypt(const unsigned char *in, unsigned char *out,
                     size_t len, const AES_KEY *key, const int enc)
{
    size_t          dma_size;
    unsigned long   mode;
    unsigned long   ivec[4] = { 0, 0, 0, 0 };

    
    if (fd_aes < 0)
    	Nuvoton_Init_AES();
    
    dma_size = (size_t)(aes_buff_size / 2);

    mode = NVT_AES_INSWAP | NVT_AES_OUTSWAP | NVT_AES_ECB_MODE;
    
    if (key->rounds == 10)
    	mode |= NVT_AES_KEYSZ_128;
	else if (key->rounds == 12)
    	mode |= NVT_AES_KEYSZ_192;
	else
    	mode |= NVT_AES_KEYSZ_256;
    	
   	if (enc)
   	    mode |= NVT_AES_ENCRYPT;
   	else
   	    mode |= NVT_AES_DECRYPT;
	
	sem_wait(&aes_semaphore);

    ioctl(fd_aes, AES_IOC_SET_MODE, mode);
    ioctl(fd_aes, AES_IOC_SET_IV, (unsigned long *)ivec);
    ioctl(fd_aes, AES_IOC_SET_KEY, key->rd_key);

    if (len < dma_size)
    {
    	memcpy(aes_map_inbuff, in, len);
    	ioctl(fd_aes, AES_IOC_SET_LEN, len);
    	ioctl(fd_aes, AES_IOC_START, 1);
    	memcpy(out, aes_map_outbuff, len);
    	len = 0;
    }
    else
    {
    	memcpy(aes_map_inbuff, in, dma_size);
    	ioctl(fd_aes, AES_IOC_SET_LEN, dma_size);
    	ioctl(fd_aes, AES_IOC_START, 1);
    	memcpy(out, aes_map_outbuff, dma_size);
    	len -= dma_size;
    	in += dma_size;
    	out += dma_size;
    }
    
    while (len > 0)
    {
    	if (len < dma_size)
    	{
    		memcpy(aes_map_inbuff, in, len);
    		ioctl(fd_aes, AES_IOC_SET_LEN, len);
    		ioctl(fd_aes, AES_IOC_C_START, 1);
    		memcpy(out, aes_map_outbuff, len);
    		len = 0;
    	}
    	else
	    {
	    	memcpy(aes_map_inbuff, in, dma_size);
	    	ioctl(fd_aes, AES_IOC_SET_LEN, dma_size);
	    	ioctl(fd_aes, AES_IOC_C_START, 1);
	    	memcpy(out, aes_map_outbuff, dma_size);
	    	len -= dma_size;
	    	in += dma_size;
	    	out += dma_size;
	    }
	}

	ioctl(fd_aes, AES_IOC_UPDATE_IV, (unsigned long *)ivec);
	sem_post(&aes_semaphore);
}

#else

void AES_ecb_encrypt(const unsigned char *in, unsigned char *out,
                     const AES_KEY *key, const int enc)
{

    assert(in && out && key);
    assert((AES_ENCRYPT == enc) || (AES_DECRYPT == enc));

    if (AES_ENCRYPT == enc)
        AES_encrypt(in, out, key);
    else
        AES_decrypt(in, out, key);
}

#endif
