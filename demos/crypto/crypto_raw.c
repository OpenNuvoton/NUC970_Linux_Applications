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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "nuc970-crypto.h"

#define BUF_SIZE    1024

#define NVT_AES		"/dev/nuvoton-aes"
#define NVT_SHA		"/dev/nuvoton-sha"

int fd_aes = -1;
int fd_sha = -1;

unsigned char *aes_map_inbuff, *aes_map_outbuff;
unsigned long aes_buff_size;

extern void print_data(char *str, char *buff, int len);

int Nuvoton_Init_AES(void)
{
	fd_aes = open(NVT_AES, O_RDWR);
	if (fd_aes < 0) {
		printf("open %s error\n", NVT_AES);
		return fd_aes;
	}

	if (ioctl(fd_aes, AES_IOC_GET_BUFSIZE, &aes_buff_size) < 0) {
		printf("Failed to get Nuvoton AES buffer size!\n");
		close(fd_aes);
		fd_aes = -1;
		return -1;
	}

	aes_map_inbuff = (unsigned char *)mmap(NULL, aes_buff_size,
					  PROT_READ|PROT_WRITE, MAP_SHARED, fd_aes, 0);
	if (aes_map_inbuff == NULL) {
		printf("Failed to mmap AES buffer!\n");
		close(fd_aes);
		fd_aes = -1;
		return -1;
	}
	aes_map_outbuff = aes_map_inbuff + aes_buff_size/2;

	printf("Nuvoton AES buffer size %d, map to 0x%x.\n", (int)aes_buff_size, (int)aes_map_inbuff);
	return 0;
}

void Nuvoton_Deinit_AES(void)
{
	munmap(aes_map_inbuff, aes_buff_size);
	close(fd_aes);
}

static void aes_crypt(int encrypt, char *in, int len, char *out,
		      const char *key, const char *iv)
{
	size_t dma_size;
	unsigned long mode;

	dma_size = (size_t)(aes_buff_size / 2);

	mode = NVT_AES_INSWAP | NVT_AES_OUTSWAP | NVT_AES_KEYSZ_256 | NVT_AES_CBC_MODE;

	if (encrypt)
		mode |= NVT_AES_ENCRYPT;
	else
		mode |= NVT_AES_DECRYPT;

	ioctl(fd_aes, AES_IOC_SET_MODE, mode);
	ioctl(fd_aes, AES_IOC_SET_IV, (unsigned long *)iv);
	ioctl(fd_aes, AES_IOC_SET_KEY, key);

	if (len < dma_size) {
		memcpy(aes_map_inbuff, in, len);
		ioctl(fd_aes, AES_IOC_SET_LEN, len);
		ioctl(fd_aes, AES_IOC_START, 1);
		memcpy(out, aes_map_outbuff, len);
		len = 0;
	} else {
		memcpy(aes_map_inbuff, in, dma_size);
		ioctl(fd_aes, AES_IOC_SET_LEN, dma_size);
		ioctl(fd_aes, AES_IOC_START, 1);
		memcpy(out, aes_map_outbuff, dma_size);
		len -= dma_size;
		in += dma_size;
		out += dma_size;
	}

	while (len > 0) {
		if (len < dma_size) {
			memcpy(aes_map_inbuff, in, len);
			ioctl(fd_aes, AES_IOC_SET_LEN, len);
			ioctl(fd_aes, AES_IOC_C_START, 1);
			memcpy(out, aes_map_outbuff, len);
			len = 0;
		} else {
			memcpy(aes_map_inbuff, in, dma_size);
			ioctl(fd_aes, AES_IOC_SET_LEN, dma_size);
			ioctl(fd_aes, AES_IOC_C_START, 1);
			memcpy(out, aes_map_outbuff, dma_size);
			len -= dma_size;
			in += dma_size;
			out += dma_size;
		}
	}
}

void AES_raw_demo(void)
{
	int i;
	char out[BUF_SIZE];
	char in[BUF_SIZE];
	static const char key[32] = "\x06\xa9\x21\x40\x36\xb8\xa1\x5b"
				    "\x51\x2e\x03\xd5\x34\x12\x00\x06"
				    "\x06\xa9\x21\x40\x36\xb8\xa1\x5b"
				    "\x51\x2e\x03\xd5\x34\x12\x00\x06";
	static const char iv[16] = "\x3d\xaf\xba\x42\x9d\x9e\xb4\x30\xb4\x22\xda\x80\x2c\x9f\xac\x41";

	printf("\n+---------------------------------------------------------------+\n");
	printf("|  [RAW] AES-256 CBC mode encrypt/descrpt demo                  |\n");
	printf("+---------------------------------------------------------------+\n");

	for (i = 0; i < BUF_SIZE; i++)
		in[i] = (i ^ 0xff) & 0xff;

	aes_crypt(1, in, BUF_SIZE, out, key, iv);

	printf("\nAES encrypt result =>\n");
	print_data("IN", in, BUF_SIZE);
	print_data("OUT", out, BUF_SIZE);

	aes_crypt(0, out, BUF_SIZE, in, key, iv);

	printf("\nAES descrypt result =>\n");
	print_data("IN", out, BUF_SIZE);
	print_data("OUT", in, BUF_SIZE);
}

int Nuvoton_Init_SHA(void)
{
	fd_sha = open(NVT_SHA, O_RDWR);
	if (fd_sha < 0) {
		printf("open %s error\n", NVT_SHA);
		return fd_sha;
	}
	return 0;
}

void Nuvoton_Deinit_SHA(void)
{
	close(fd_sha);
}

int SHA_raw_demo(void)
{
	int i;
	char in[BUF_SIZE];
	unsigned char  msg_dgst[66];

	for (i = 0; i < BUF_SIZE; i++)
		in[i] = i & 0xff;

	printf("\n+---------------------------------------------------------------+\n");
	printf("|  [RAW] SHA256 demo                                            |\n");
	printf("+---------------------------------------------------------------+\n");

	ioctl(fd_sha, SHA_IOC_INIT, NVT_SHA256 | NVT_SHA_INSWAP | NVT_SHA_OUTSWAP);
	write(fd_sha, in, BUF_SIZE);
	ioctl(fd_sha, SHA_IOC_FINISH, 1);
	memset(msg_dgst, 0, sizeof(msg_dgst));
	read(fd_sha, msg_dgst, 32);
	printf("\nSHA digest: ");
	for (i = 0; i < 32; i++)
		printf("%02x", msg_dgst[i]);
	printf("\n\n");
	return 0;
}

void crypto_raw_demo(void)
{
	/* AES */
	if (Nuvoton_Init_AES() != 0)
		return;
	AES_raw_demo();
	Nuvoton_Deinit_AES();

	/* SHA */
	if (Nuvoton_Init_SHA() != 0)
		return;
	SHA_raw_demo();
	Nuvoton_Deinit_SHA();
}
