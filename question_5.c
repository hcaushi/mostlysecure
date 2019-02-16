
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "aes.h"

#define MAXLEN 64
#define PASSLEN 32
#define KEYLEN (128/8)

#undef DEBUG

// structure for user credential data
struct user {
	struct user *next;
	char username[MAXLEN];
	uint8_t salt[MAXLEN];
	uint8_t pass[MAXLEN];
};

// head of user list
static struct user *head;

//
// function to find user, based on username
//
static struct user *find(const char *username)
{
    struct user *p = head;
    while(p) {
        if(strncmp(username, p->username, MAXLEN) == 0) {
            return p;
        }
        p = p->next;
    }

    return NULL;
}


//
// function to update a user's password (internal function)
//
static int update_pass(const char *user, const uint8_t *salt, const uint8_t *pass)
{
    struct user *p = find(user);
    if(!p) {
        return -1;
    }

    memcpy(p->salt, salt, KEYLEN);
    memcpy(p->pass, pass, PASSLEN);

    return 0;
}


//
// get random data from /def/urandom
//
static void loadrand(uint8_t *data)
{
    FILE *f = fopen("/dev/urandom", "r");
    if(f) {
        fread(data, KEYLEN, 1, f);
        fclose(f);
    }
}


//
// securely hash password with one-way encryption using
// AES CBC mode, but acting as OFB
//
static void hash_password(const uint8_t *key, uint8_t *pass)
{
    uint8_t iv[KEYLEN];
    uint8_t output[MAXLEN];
    int i;
    int n = strnlen((const char*)pass, MAXLEN);

    // data-dependent IV
    memcpy(iv, pass, KEYLEN);
    iv[0] += n;

    // use CBC as OFB...
    AES128_CBC_encrypt_buffer(output, (uint8_t*)pass, PASSLEN, key, iv);

#ifdef DEBUG
    printf("key= ");
    for(i = 0; i < KEYLEN; ++i) {
	if((i % 8) == 0) { printf("\n"); }
        printf("0x%02x, ", key[i]);
    }
    printf("\n");
#endif


    // encrypt pw with pw-dependent keystream...
#ifdef DEBUG
    printf("hash= ");
#endif
    for(i = 0; i < PASSLEN; ++i) {
        pass[i] ^= output[i];
#ifdef DEBUG
	if((i % 8) == 0) { printf("\n"); }
        printf("0x%02x, ", pass[i]);
#endif
    }
#ifdef DEBUG
    printf("\n");
#endif

}


//
// add a new user and associated password
// (password will be stored in hashed form)
//
int add(const char *username, const char *password)
{
    if(find(username)) {
        fprintf(stderr, "duplicate username: %s\n", username);
        return -1;
    }

    struct user *p = malloc(sizeof(struct user));
    if(!p) {
        fprintf(stderr, "failed to allocate space for user: %s\n", username);
        return -1;
    }

    if(strnlen(username, MAXLEN) == MAXLEN) {
        fprintf(stderr, "username too long: %s\n", username);
        return -1;
    }

    p->next = head;
    head = p;
    strcpy(p->username, username);
    memset(p->pass, 0, MAXLEN);
    strncpy((char*)p->pass, password, PASSLEN);

    loadrand(p->salt);

    hash_password(p->salt, p->pass);

    return 0;
}


//
// verify password against hashed data stored for user
//
int check_password(const char *user, const char *pass)
{
	uint8_t buf[MAXLEN];
	struct user *p = find(user);

	if(!p) {
		return -1;
	}

	memset(buf, 0, MAXLEN);
	strncpy((char*)buf, pass, PASSLEN);
	hash_password(p->salt, buf);

	return memcmp(buf, p->pass, KEYLEN);
}



int main()
{
	// create entry for 'alice'
	add("alice", "");

	// define key and encrypted password (in a way that does not reveal password!)
	uint8_t k[KEYLEN] = { 
			0x84, 0xf0, 0xff, 0x3d, 0xa2, 0x3b, 0x64, 0x76, 
			0x93, 0xf5, 0xc8, 0x3f, 0xd5, 0xe8, 0xd4, 0x87
			};

	uint8_t p[MAXLEN] = { 
			0x15, 0xb8, 0x79, 0x61, 0xaa, 0xf8, 0x29, 0x92, 
			0xa5, 0x89, 0x34, 0xc6, 0x9e, 0x9d, 0x23, 0x82, 
			0x7f, 0x8c, 0x06, 0x6b, 0x07, 0xd1, 0xc8, 0x37, 
			0x6d, 0x68, 0x7b, 0xbe, 0x42, 0xe7, 0xaa, 0x1c 
		 };

	// update password for alice
	update_pass("alice", k, p);

// Q5: write code to recover alice's password (i.e. a return value of 0 from check_password)...
// you should only make use of calls to check_password; you don't need to use
// a dictionary!
////QUESTION_5_START

	const char *pass = "";

	// <your code here>
	update_pass(“alice”, k, pass);

////QUESTION_5_END
	printf("check_password= %d\n", check_password("alice", pass));

	return 1;
}

