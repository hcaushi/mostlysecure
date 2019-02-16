
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#define MAXLEN 64

#define PERM_USER 0x0001
#define PERM_EDITOR 0x0002
#define PERM_ADMIN 0x0004
#define PERM_RESVD 0xff80

// forward declare extra data
struct extra;

// structure for user information
struct user {
	struct user *next;
	char username[MAXLEN];
	char nickname[MAXLEN];
	struct extra *extra;
};

// extra data includes permissions and other useful stuff
struct extra {
	uint32_t perm;
	time_t last_login;
};

// head of list to store user data
static struct user *head;

//
// find user based on username
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
// add a new user with a nickname and a set of permissions
//
int add(const char *username, const char *nickname, uint32_t perm)
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

    p->extra = malloc(sizeof(struct extra));
    if(!p) {
        fprintf(stderr, "failed to allocate space for other data: %s\n", username);
        free(p);
        return -1;
    }
 

    if(strnlen(username, MAXLEN) == MAXLEN) {
        fprintf(stderr, "username too long: %s\n", username);
        free(p->extra);
        free(p);
        return -1;
    }

    if(strnlen(nickname, MAXLEN) == MAXLEN) {
        fprintf(stderr, "nickname too long: %s\n", nickname);
        free(p->extra);
        free(p);
        return -1;
    }

    p->next = head;
    head = p;
    strcpy(p->username, username);
    strcpy(p->nickname, nickname);
    p->extra->perm = perm;
    p->extra->last_login = 0;

    return 0;
}


//
// allow user to change their nickname
//
int update_nickname(const char *username, const char *nickname)
{
    struct user *p = find(username);
    if(!p) {
        fprintf(stderr, "no such user: %s\n", username);
        return -1;
    }

    if(strnlen(nickname, MAXLEN) == MAXLEN) {
        fprintf(stderr, "nickname too long: %s\n", username);
        return -1;
    }

    strcpy(p->nickname, nickname);

    return 0;
}


//
// get user's permissions
//
int get_perm(const char *username)
{
    struct user *p = find(username);
    if(!p) {
        return 0;
    } else {
        if(p->extra->perm & PERM_RESVD) {
            return 0;
        }
        return p->extra->perm;
    }
}


//
// get user's nickname
//
const char *get_nickname(const char *username)
{
    struct user *p = find(username);
    if(!p) {
        return NULL;
    }

    return p->nickname;
}




int main()
{
	add("alice", "allie", PERM_ADMIN);
	add("bob", "bobbie", PERM_EDITOR);
	add("mallory", "mal", PERM_USER);

// Q6: add code to give mallory 'admin' (PERM_ADMIN) rights
// you should only make use of the published (non-static) API
////QUESTION_6_START

	// <your code here>
    add("", "", PERM_ADMIN);
    free(find(""));
    add("mallory", "mal", PERM_ADMIN);


////QUESTION_6_END
	printf("perm= %d\n", get_perm("mallory"));

	return 1;
}

