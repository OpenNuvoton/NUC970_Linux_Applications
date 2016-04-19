#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "if_alg.h"

//#define DO_MTP_PROGRAM_TEST
//#define DO_MTP_LOCK_TEST

#ifndef AF_ALG
#define AF_ALG 		38
#define SOL_ALG 	279
#endif
#define BUF_SIZE 	16


__u32  _mtp_key[9] = {
	0x98fc0099, 0x3424a76c, 0xac9879f8, 0xef9a8c80, 0xc76c76c8, 0x9f898f8f, 0xa987f87a, 0x897c87b8,
};

#define MTP_USER_DATA		0x81


static void aes_crypt(__u8 *protocol, int encrypt, int use_mtp_key,
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
 	
 	if (use_mtp_key)
 		setsockopt(tfmfd, SOL_ALG, ALG_USE_MTP_KEY, NULL, 0);
 	else
 		setsockopt(tfmfd, SOL_ALG, ALG_USE_REG_KEY, NULL, 0);
 	
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


void MTP_demo(void)
{
	int tfmfd;
	__u32  mtp_status;
	struct sockaddr_alg sa = {
	.salg_family = AF_ALG,
	.salg_type = "skcipher",
	.salg_name = "mtp"
 	};

    printf("\n+--------------------------------+\n");
	printf("|  MTP key demo                    |\n");
    printf("+----------------------------------+\n");
 	
 	tfmfd = socket(AF_ALG, SOCK_SEQPACKET, 0);
 	bind(tfmfd, (struct sockaddr *)&sa, sizeof(sa));
 	
#ifdef DO_MTP_PROGRAM_TEST
	_mtp_key[8] = MTP_USER_DATA;	
 	if (setsockopt(tfmfd, SOL_ALG, ALG_MTP_PROGRAM, _mtp_key, 36) == 0)
 		printf("MTP program succeed.\n");
 	else
 		printf("Failed to program MTP key!\n");
#endif

#ifdef DO_MTP_LOCK_TEST
 	if (setsockopt(tfmfd, SOL_ALG, ALG_MTP_LOCK, NULL, 0) == 0)
 		printf("MTP lock succeed.\n");
 	else
 		printf("Failed to lock MTP key!\n");
#endif

 	mtp_status = setsockopt(tfmfd, SOL_ALG, ALG_MTP_STATUS, NULL, 0);
 	if (mtp_status == 0xFFFF)
 		printf("Failed to get MTP status!\n");
 	else
 	{
    	printf("MTP status: 0x%x (", mtp_status);
    	if (mtp_status & 0x1)
    		printf(" ENABLED");
    	if (mtp_status & 0x2)
    		printf(" KEY_VALID");
    	if (mtp_status & 0x4)
    		printf(" NO_KEY");
    	if (mtp_status & 0x8)
    		printf(" LOCKED");
    	if (mtp_status & 0x10)
    		printf(" PROG_FAIL");
    	if (mtp_status & 0x1000000)
    		printf(" BUSY");
    	printf(" PRGCNT=%d)\n", (mtp_status >> 16) & 0xf);
	}	
 	close(tfmfd);
}


void AES_demo(int use_mtp_key)
{
 	int i;
 	char out[BUF_SIZE] = {0};
 	char in[BUF_SIZE] = "Single block msg";
 	const char key[16] =  "\x06\xa9\x21\x40\x36\xb8\xa1\x5b"
  							"\x51\x2e\x03\xd5\x34\x12\x00\x06";
 	char iv[16] = "\x3d\xaf\xba\x42\x9d\x9e\xb4\x30\xb4\x22\xda\x80\x2c\x9f\xac\x41";

    printf("\n+---------------------------------------------------------------+\n");
    if (use_mtp_key)
	printf("|  AES-128 CBC mode encrypt/descrpt demo (use MTP key)          |\n");
	else
	printf("|  AES-128 CBC mode encrypt/descrpt demo                        |\n");
    printf("+---------------------------------------------------------------+\n");

	aes_crypt("cbc(aes)", 1, use_mtp_key, in, BUF_SIZE, out, key, iv);

    printf("\nAES encrypt result =>\n");
	print_data("IN", in, BUF_SIZE);
	print_data("OUT", out, BUF_SIZE);

	aes_crypt("cbc(aes)", 0, use_mtp_key, out, BUF_SIZE, in, key, iv);

    printf("\nAES descrypt result =>\n");
	print_data("IN", out, BUF_SIZE);
	print_data("OUT", in, BUF_SIZE);
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

    printf("\n+---------------------------------------------------------------+\n");
	printf("|  DES ECB mode encrypt/descrpt demo                            |\n");
    printf("+---------------------------------------------------------------+\n");

	des_crypt(0, "ecb(des)", 1, in, BUF_SIZE, out, key, iv);

    printf("\nDES encrypt result =>\n");
	print_data("IN", in, BUF_SIZE);
	print_data("OUT", out, BUF_SIZE);

	des_crypt(0, "ecb(des)", 0, out, BUF_SIZE, in, key, iv);

    printf("\nDES descrypt result =>\n");
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

    printf("\n+---------------------------------------------------------------+\n");
	printf("|  Tripple-DES ECB mode encrypt/descrpt demo                    |\n");
    printf("+---------------------------------------------------------------+\n");
 
	des_crypt(1, "ecb(3des)", 1, in, BUF_SIZE, out, key, iv);

    printf("\nTDES encrypt result =>\n");
	print_data("IN", in, BUF_SIZE);
	print_data("OUT", out, BUF_SIZE);

	des_crypt(1, "ecb(3des)", 0, out, BUF_SIZE, in, key, iv);

    printf("\nTDES descrypt result =>\n");
	print_data("IN", out, BUF_SIZE);
	print_data("OUT", in, BUF_SIZE);
}

int SHA_demo(int digest_size, int is_hmac)
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
    char hmac_key[8] = { 0x89, 0x11, 0x32, 0xef, 0x10, 0x8a, 0x22, 0x90 };
    char buf[64];
    int  i, bytes;

    printf("\n+---------------------------------------------------------------+\n");
	if (is_hmac)
		printf("|  HMAC-SHA-%d demo                                            |\n", digest_size);
	else	
		printf("|  SHA-%d demo                                                 |\n", digest_size);
    printf("+---------------------------------------------------------------+\n");

	switch (digest_size)
	{
		case 160:
			strcpy(sa.salg_name, is_hmac ? "hmac-sha1" : "sha1");
			break;
			
		case 224:
			strcpy(sa.salg_name, is_hmac ? "hmac-sha224" : "sha224");
			break;

		case 256:
			strcpy(sa.salg_name, is_hmac ? "hmac-sha256" : "sha256");
			break;

		case 384:
			strcpy(sa.salg_name, is_hmac ? "hmac-sha384" : "sha384");
			break;

		case 512:
			strcpy(sa.salg_name, is_hmac ? "hmac-sha512" : "sha512");
			break;
		
		default:
			printf("Invalid parameter!\n");
			return -1;
	}
	
	bytes = digest_size/8;
 
    tfmfd = socket(AF_ALG, SOCK_SEQPACKET, 0);
 
    bind(tfmfd, (struct sockaddr *)&sa, sizeof(sa));

    opfd = accept(tfmfd, NULL, 0);

	if (is_hmac)
 		setsockopt(tfmfd, SOL_ALG, ALG_SET_KEY, hmac_key, 8);  // must be x4 length

    write(opfd, msg1, sizeof(msg1));
    read(opfd, buf, bytes);
	printf("Output digest:\n  "); 
    for (i = 0; i < bytes; i++) {
        printf("%02x", (unsigned char)buf[i]);
    }
    printf("\n");

	if (is_hmac)
 		setsockopt(tfmfd, SOL_ALG, ALG_SET_KEY, hmac_key, 8);  // must be x4 length

    write(opfd, msg2, sizeof(msg2));
    read(opfd, buf, bytes);
	printf("Output digest:\n  "); 
    for (i = 0; i < bytes; i++) {
        printf("%02x", (unsigned char)buf[i]);
    }
    printf("\n");

	if (is_hmac)
 		setsockopt(tfmfd, SOL_ALG, ALG_SET_KEY, hmac_key, 8);  // must be x4 length

    write(opfd, msg3, sizeof(msg3));
    read(opfd, buf, bytes);
	printf("Output digest:\n  "); 
    for (i = 0; i < bytes; i++) {
        printf("%02x", (unsigned char)buf[i]);
    }
    printf("\n");
 
    close(opfd);
    close(tfmfd);
}

int main(int argc, char **argv)
{
	MTP_demo();
	AES_demo(0);
	AES_demo(1);            // use MTP key
	DES_demo();
	TDES_demo();
	SHA_demo(160, 0);       // SHA-1
	SHA_demo(160, 1);       // HMAC-SHA-1
	SHA_demo(224, 0);       // SHA-224
	SHA_demo(224, 1);       // HMAC-SHA-224
	SHA_demo(256, 0);       // SHA-256
	SHA_demo(256, 1);       // HMAC-SHA-256
	SHA_demo(384, 0);       // SHA-384
	SHA_demo(384, 1);       // HMAC-SHA-384
	SHA_demo(512, 0);       // SHA-512
	SHA_demo(512, 1);       // HMAC-SHA-512
}



