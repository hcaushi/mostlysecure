
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>


#define BUFLEN 16

// define a hash-table entry
struct entry {
	char name[BUFLEN];
	char pass[BUFLEN];
	int used;
	int ever_used;
};


#define HASHTABSZ 131

// declare the hash table
static struct entry hash_table[HASHTABSZ];

// vector of numbers for an efficient hash function
const uint16_t H[] = { 271, 8281, 2845, 9045, 2353, 6287, 4713, 5649, 
                      7757, 2407, 9369, 1995, 9731, 7739, 4775, 1971 };

//
// a simple function to hash a string
static uint32_t hash(const char *s)
{
	uint16_t h = 0;

	unsigned int i;

	for(i = 0; i < strnlen(s, BUFLEN); ++i) {
		h += s[i] * H[i];
	}

	return h;
}


//
// find an entry in the hash table; return a pointer
// to a valid empty slot (if not found), or the item, if found
//
static struct entry *find(const char *name, const char *pass)
{
	uint32_t h = hash(pass) ^ hash(name);
	unsigned int i = h % HASHTABSZ;
	int search = 1;
	struct entry *empty = NULL;

	while(search) {
		switch(hash_table[i].used) {
			case 0:
				if(hash_table[i].ever_used) {
					// keep looking...
					// ... but remember empty slot
					empty = hash_table + i;
				} else {
					return hash_table + i;
				}

			case 1:
				if(strcmp(name, hash_table[i].name) == 0) {
					return hash_table + i;
				}

			default:
				i = (i + h) % HASHTABSZ;
		}
	}
				
	return empty;
}


//
// add a new user to the hash table
//
int add(const char *name, const char *pass)
{
	struct entry *e = find(name, pass);

	if(e->used) {
		// duplicate name
		return 1;
	} else {
		// fill in record
		e->used = 1;
		e->ever_used = 1;
		strcpy(e->name, name);
		strcpy(e->pass, pass);
	}

	return 0;
}


//
// check whether a username/password are in the hash table
//
int check(const char *name, const char *pass)
{
	struct entry *e = find(name, pass);

	if(e->used) {
		return strcmp(name, e->name) && 
		       strcmp(pass, e->pass);
	}
				
	return 1;
}


//
// change a user's password -- verifies existing password
//
int update_pw(const char *name, const char *old, const char *new)
{
	if(check(name, old) == 0) {
		struct entry *e = find(name, old);
		e->used = 0;
		add(name, new);
		return 0;
	}
	return 1;
}


int main()
{
	add("alice", "hardtoguess");
	add("bob", "hardertoguess");

// Q4: assume that you do NOT know alice's password...
// you can only use the external (non-static) interface, above
// make one or more calls to allow you to log in as alice with
// a password of your choosing
//
////QUESTION_4_START
	// 0, 1, 0, 0, 0, -1, 0, 0,
	// 0, 0, 0, 0, -1, 1, 0, 0
	char repeater[] = { 0, 1, 0, 0, 0, -1, 0, 0,
			0, 0, 0, 0, -1, 1, 0, 0 };
	char repeater2[] = { 1/4336, 1/132496, 1/45520, 1/144720,
    	                1/37648, 1/100592, 1/75408, 1/90384,
    	                1/124112, 1/38513, 1/149904, 1/31920,
    	                1/155696, 1/123824, 1/76400, 1/31536 };
	int i;
	int j;
    
	while (1){
    	++i;
        char array[i*16];
        for (j=0;j<i*16;++j){
            array[j] = repeater[j%16];
        }
        update_pw("alice", array ,"yourpassword");
	}
////QUESTION_4_END
	printf("%d\n", check("alice", "yourpassword"));

	return 1;
}


