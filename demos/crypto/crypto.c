#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "if_alg.h"

#ifndef AF_ALG
#define AF_ALG 		38
#define SOL_ALG 	279
#endif
#define BUF_SIZE 	16


static void aes_crypt(__u8 *protocol, int encrypt, 
							char *in, int inlen, char *out, const char *key, char *oiv)
{
	int opfd;
	int tfmfd;
	struct sockaddr_alg sa = {
	.salg_family = AF_ALG,
	.salg_type = "skcipher",
	.salg_name = "ecb(aes)"
 	};
 	struct msghdr msg = {};
 	struct cmsghdr *cmsg;
 	char cbuf[CMSG_SPACE(4) + CMSG_SPACE(20)] = {};
 	struct af_alg_iv *iv;
 	struct iovec iov;
 	
 	strcpy(sa.salg_name, protocol);

 	tfmfd = socket(AF_ALG, SOCK_SEQPACKET, 0);
 	bind(tfmfd, (struct sockaddr *)&sa, sizeof(sa));
 	setsockopt(tfmfd, SOL_ALG, ALG_SET_KEY, key, 16);
 	opfd = accept(tfmfd, NULL, 0);
 	msg.msg_control = cbuf;
 	msg.msg_controllen = sizeof(cbuf);
 	cmsg = CMSG_FIRSTHDR(&msg);
 	cmsg->cmsg_level = SOL_ALG;
 	cmsg->cmsg_type = ALG_SET_OP;
 	cmsg->cmsg_len = CMSG_LEN(4);
 	
 	if (encrypt)
 		*(__u32 *)CMSG_DATA(cmsg) = ALG_OP_ENCRYPT; 
 	else
 		*(__u32 *)CMSG_DATA(cmsg) = ALG_OP_DECRYPT; 
 
 	cmsg = CMSG_NXTHDR(&msg, cmsg); 
 	cmsg->cmsg_level = SOL_ALG; 
 	cmsg->cmsg_type = ALG_SET_IV; 
 	cmsg->cmsg_len = CMSG_LEN(20); 
 	iv = (void *)CMSG_DATA(cmsg); 
 	memcpy(iv->iv, oiv, 16);
 	iv->ivlen = 16;
 	iov.iov_base = in;
 	iov.iov_len = inlen;
 	msg.msg_iov = &iov;
 	msg.msg_iovlen = 1;
 	sendmsg(opfd, &msg, 0);
 	read(opfd, out, inlen);
 	close(opfd);
 	close(tfmfd);
}


static void des_crypt(int is_3des, __u8 *protocol, int encrypt, 
							char *in, int inlen, char *out, const char *key, char *oiv)
{
	int opfd;
	int tfmfd;
	struct sockaddr_alg sa = {
	.salg_family = AF_ALG,
	.salg_type = "skcipher",
	.salg_name = "ecb(des)"
 	};
 	struct msghdr msg = {};
 	struct cmsghdr *cmsg;
 	char cbuf[CMSG_SPACE(4) + CMSG_SPACE(20)] = {};
 	struct af_alg_iv *iv;
 	struct iovec iov;
 	
 	strcpy(sa.salg_name, protocol);

 	tfmfd = socket(AF_ALG, SOCK_SEQPACKET, 0);
 	bind(tfmfd, (struct sockaddr *)&sa, sizeof(sa));
 	if (is_3des)
 	 	setsockopt(tfmfd, SOL_ALG, ALG_SET_KEY, key, 24);
 	else
   		setsockopt(tfmfd, SOL_ALG, ALG_SET_KEY, key, 8);
 	opfd = accept(tfmfd, NULL, 0);
 	msg.msg_control = cbuf;
 	msg.msg_controllen = sizeof(cbuf);
 	cmsg = CMSG_FIRSTHDR(&msg);
 	cmsg->cmsg_level = SOL_ALG;
 	cmsg->cmsg_type = ALG_SET_OP;
 	cmsg->cmsg_len = CMSG_LEN(4);
 	
 	if (encrypt)
 		*(__u32 *)CMSG_DATA(cmsg) = ALG_OP_ENCRYPT; 
 	else
 		*(__u32 *)CMSG_DATA(cmsg) = ALG_OP_DECRYPT; 
 
 	cmsg = CMSG_NXTHDR(&msg, cmsg); 
 	cmsg->cmsg_level = SOL_ALG; 
 	cmsg->cmsg_type = ALG_SET_IV; 
 	cmsg->cmsg_len = CMSG_LEN(20); 
 	iv = (void *)CMSG_DATA(cmsg); 
 	memcpy(iv->iv, oiv, 8);
 	iv->ivlen = 8;
 	iov.iov_base = in;
 	iov.iov_len = inlen;
 	msg.msg_iov = &iov;
 	msg.msg_iovlen = 1;
 	sendmsg(opfd, &msg, 0);
 	read(opfd, out, inlen);
 	close(opfd);
 	close(tfmfd);
}


void  print_data(char *str, char *buff, int len)
{
	int  i;
	
	printf("%s: ", str);
 	for (i = 0; i < len; i++) 
 	 printf("%02x", (unsigned char)buff[i]);
 	printf("\n");
}


void AES_demo()
{
 	int i;
 	char out[BUF_SIZE] = {0};
 	char in[BUF_SIZE] = "Single block msg";
 	const char key[16] =  "\x06\xa9\x21\x40\x36\xb8\xa1\x5b"
  							"\x51\x2e\x03\xd5\x34\x12\x00\x06";
 	char iv[16] = "\x3d\xaf\xba\x42\x9d\x9e\xb4\x30\xb4\x22\xda\x80\x2c\x9f\xac\x41";

    printf("\nBefore AES encode =>\n");
	print_data("IN", in, BUF_SIZE);
	print_data("OUT", out, BUF_SIZE);
 
	aes_crypt("cbc(aes)", 1, in, BUF_SIZE, out, key, iv);

    printf("\nAES encrypt result =>\n");
	print_data("IN", in, BUF_SIZE);
	print_data("OUT", out, BUF_SIZE);

	aes_crypt("cbc(aes)", 0, out, BUF_SIZE, in, key, iv);

    printf("\nAES descrypt result =>\n");
	print_data("IN", out, BUF_SIZE);
	print_data("OUT", in, BUF_SIZE);
	printf("\n\n");
}


void DES_demo()
{
 	int i;
 	char out[BUF_SIZE] = {0};
 	char in[BUF_SIZE] = "Single block msg";
 	const char key[24] =  "\x06\xa9\x21\x40\x36\xb8\xa1\x5b"
  							"\x51\x2e\x03\xd5\x34\x12\x00\x06"
  							"\xa0\x53\x07\x1e\x22\x00\x96\x77";
 	char iv[8] = "\x3d\xaf\xba\x42\x9d\x9e\xb4\x30";

    printf("\nBefore TDES encode =>\n");
	print_data("IN", in, BUF_SIZE);
	print_data("OUT", out, BUF_SIZE);
 
	des_crypt(0, "ecb(des)", 1, in, BUF_SIZE, out, key, iv);

    printf("\nTDES encrypt result =>\n");
	print_data("IN", in, BUF_SIZE);
	print_data("OUT", out, BUF_SIZE);

	des_crypt(0, "ecb(des)", 0, out, BUF_SIZE, in, key, iv);

    printf("\nTDES descrypt result =>\n");
	print_data("IN", out, BUF_SIZE);
	print_data("OUT", in, BUF_SIZE);
}

void TDES_demo()
{
 	int i;
 	char out[BUF_SIZE] = {0};
 	char in[BUF_SIZE] = "Single block msg";
 	const char key[24] =  "\x06\xa9\x21\x40\x36\xb8\xa1\x5b"
  							"\x51\x2e\x03\xd5\x34\x12\x00\x06"
  							"\xa0\x53\x07\x1e\x22\x00\x96\x77";
 	char iv[8] = "\x3d\xaf\xba\x42\x9d\x9e\xb4\x30";

    printf("\nBefore TDES encode =>\n");
	print_data("IN", in, BUF_SIZE);
	print_data("OUT", out, BUF_SIZE);
 
	des_crypt(1, "ecb(3des)", 1, in, BUF_SIZE, out, key, iv);

    printf("\nTDES encrypt result =>\n");
	print_data("IN", in, BUF_SIZE);
	print_data("OUT", out, BUF_SIZE);

	des_crypt(1, "ecb(3des)", 0, out, BUF_SIZE, in, key, iv);

    printf("\nTDES descrypt result =>\n");
	print_data("IN", out, BUF_SIZE);
	print_data("OUT", in, BUF_SIZE);
}

int SHA_demo(void)
{
    int opfd;
    int tfmfd;
    struct sockaddr_alg sa = {
        .salg_family = AF_ALG,
        .salg_type = "hash",
        .salg_name = "sha1"
    };
    char msg1[] = { 0xdf, 0x4b, 0xd2 };              // bf36ed5d74727dfd5d7854ec6b1d49468d8ee8aa
    char msg2[] = { 0xf7, 0xfb, 0x1b, 0xe2, 0x05 };  // 60b7d5bb560a1acf6fa45721bd0abb419a841a89
    char msg3[] = { 0x7e, 0x3d, 0x7b, 0x3e, 0xad, 0xa9, 0x88, 0x66 };  // 24a2c34b976305277ce58c2f42d5092031572520
    char buf[20];
    int i;
 
    tfmfd = socket(AF_ALG, SOCK_SEQPACKET, 0);
 
    bind(tfmfd, (struct sockaddr *)&sa, sizeof(sa));
 
    opfd = accept(tfmfd, NULL, 0);

    write(opfd, msg1, sizeof(msg1));
    read(opfd, buf, 20);
	printf("Output digest:\n  "); 
    for (i = 0; i < 20; i++) {
        printf("%02x", (unsigned char)buf[i]);
    }
    printf("\n");

    write(opfd, msg2, sizeof(msg2));
    read(opfd, buf, 20);
	printf("Output digest:\n  "); 
    for (i = 0; i < 20; i++) {
        printf("%02x", (unsigned char)buf[i]);
    }
    printf("\n");

    write(opfd, msg3, sizeof(msg3));
    read(opfd, buf, 20);
	printf("Output digest:\n  "); 
    for (i = 0; i < 20; i++) {
        printf("%02x", (unsigned char)buf[i]);
    }
    printf("\n");
 
    close(opfd);
    close(tfmfd);
}

int main(int argc, char **argv)
{
	AES_demo();
	DES_demo();
	TDES_demo();
	SHA_demo();
}



