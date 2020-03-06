/*
 * main.c
 *
 *  Created on: Mar 4, 2020
 *      Author: anhpt
 */

/*
 ============================================================================
 Name        : openSSL_https.c
 Author      : AnhPT
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "openssl/crypto.h"
#include "openssl/x509.h"
#include "openssl/pem.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "mbedtls/aes.h"
#include "mbedtls/md.h"
#include "base64.h"

#define MAX_BUFF 1024
#define MAX_LENGTH_DATA 1024


static void encrypt128(char input[], char output[]){
	mbedtls_aes_context context_in;

	unsigned char key1[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P'};
	unsigned char iv1[16] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P' };

	unsigned char encrypt[128];

	for (unsigned index = 0; index < 16; ++index)
		printf("%c", (char) input[index]);

	printf("Encrypt func\n");
	mbedtls_aes_init(&context_in);
	mbedtls_aes_setkey_enc(&context_in, key1, 128);
	mbedtls_aes_crypt_cbc(&context_in, MBEDTLS_AES_ENCRYPT, 32, iv1, (const unsigned char*)input,encrypt);
	strcpy((char *)output, (char *)encrypt);
}

static void decrypt128(unsigned char intput[], unsigned char output[]) {
	mbedtls_aes_context context_out;
	unsigned char key1[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P'};
	unsigned char iv1[16] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P' };
	unsigned char decrypt[128] = {'\0'};
	mbedtls_aes_init(&context_out);
	mbedtls_aes_setkey_dec(&context_out, key1, 128);
	mbedtls_aes_crypt_cbc(&context_out, MBEDTLS_AES_DECRYPT, 32, iv1, intput,decrypt);


	printf("\nInside of decrypt128 function:\n");

	for (unsigned index = 0; index < 128; ++index)
		printf("%x", (char) decrypt[index]);

	strncpy((char *)output, decrypt, strlen(decrypt));
}

int main(int argc, char * argv[]){
	int socket_desc, client_sock, c,ret;
	struct sockaddr_in server, client;
	int opt = 1;
	struct timeval timeout_receive;
	unsigned char encrypt_bufferr[MAX_LENGTH_DATA] = { 0 };
	unsigned char decrypt_bufferr[MAX_LENGTH_DATA] = { 0 };

	// Creating socket file descriptor
	printf("Creating socket...!!!!!\n");
	if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 4434
	if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
			sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(4434);

	// Forcefully attaching socket to the port 4434
	if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	c = sizeof(struct sockaddr_in);

	printf("Start waiting for incoming connections!\n");
	listen(socket_desc, 1);

	client_sock = accept(socket_desc, (struct sockaddr*) &client,
			(socklen_t*) &c);
	if (client_sock < 0) {
		printf("Failed: accept connection");
		exit(EXIT_FAILURE);
	}

	printf("Connection accepted\n");
	timeout_receive.tv_sec = 5;
	timeout_receive.tv_usec = 0;
	setsockopt(client_sock, SOL_SOCKET, SO_RCVTIMEO,
			(const void*) &timeout_receive, sizeof(struct timeval));

	memset(encrypt_bufferr, 0x00, sizeof(encrypt_bufferr));
	memset(decrypt_bufferr, 0x00, sizeof(decrypt_bufferr));
    ret = recv(client_sock, encrypt_bufferr, sizeof(encrypt_bufferr), 0);
    size_t decode64_len = 0;
    unsigned char decode64_buf[MAX_LENGTH_DATA] = {0};
    unsigned char *decode64_pt = base64_decode(encrypt_bufferr, ret, &decode64_len);
    //char *decode64_pt = base64Decoder(encrypt_bufferr, ret);
    strncpy((char *)decode64_buf, (char *)decode64_pt, decode64_len);
    //strcpy((char *)decode64_buf, decode64_pt);
    printf("\n+++++++++++++byte receive: %d\n", ret);
    printf("\n+++++++++++++decode64_len: %ld\n", decode64_len);
    printf("\n+++++++++++++received buff: %s\n", encrypt_bufferr);

    decrypt128(decode64_buf, decrypt_bufferr);
    printf("\nFinal decrypted data = %s", decrypt_bufferr);

}

