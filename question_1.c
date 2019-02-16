
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define USERS 4

const char *users[] = { "bob", "alice", "chas", "dave" };
const char *pw[] = { "secret", "unknown", "enigma", "wibble" };

//
// verify credentials against defined list
//
int check_login(const char *username, const char *password)
{

	int i;

	for(i = 0; i < USERS; ++i)
	{
		if(strncmp(username, users[i], strlen(username)) == 0)
		{
			if(strncmp(password, pw[i], strlen(password)) == 0)
			{
				return 0;
			}
		}
	}

	return 1;
}


int main()
{
	// Q1:
	// without making use of knowledge of the usernames and passwords, make a call
	// to check_login that returns '1'...
////QUESTION_1_START

	// replace these values with ones to pass the test
	const char *user = "test";
	const char *pass = "test";
	printf("check_login= %d\n", check_login(user, pass));
	user = users[0];
	pass = pw[0];

////QUESTION_1_END
	printf("check_login= %d\n", check_login(user, pass));

	return 1;
}
	


