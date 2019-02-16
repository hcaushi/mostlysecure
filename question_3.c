
#define _BSD_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>


int fetch_data(const char *item)
{
#define BUFLEN 1024
	// fetch data associated with item
	// data may be cached in a disk file
	// so first check if data is in cache

	char norm[BUFLEN];
	char buf[BUFLEN];
	FILE *f = NULL;
	struct stat fs;

	strncpy(norm, item, BUFLEN);
	norm[BUFLEN - 1] = 0;

	// make sure user data cannot cause problems with shell command
	unsigned int i;
	for(i = 0; i < strlen(norm); ++i) {
		if(norm[i] >= 33 && norm[i] <= 45) {
			norm[i] = '.';
		}
	}

	// base64 encode request to avoid users accessing arbitrary files
	//
	int r = snprintf(buf, BUFLEN, "/bin/echo -ne \"%s\" | base64", norm);

	if(r > BUFLEN) {
		return -1;
	}

	// read output
	f = popen(buf, "r");
	if(!f) {
		perror("popen");
		goto error;
	}

	char inp[BUFLEN];
	r = fread(inp, 1, BUFLEN, f);

	if(r <= 0) {
		perror("fread");
		goto error;
	}
	inp[r-1] = 0;

	fclose(f);
	f = NULL;

	if(stat(inp, &fs) != 0)
	{
		// no cache file
		return 0;
	}

	// cache file exists

	f = fopen(inp, "r");
	if(!f) {
		// error opening file
		perror("fopen");
		goto error;
	}

	r = fread(inp, 1, BUFLEN, f);
	fclose(f);
	f = NULL;

	if(r <= 0) {
		perror("fread");
		goto error;
	}
	inp[r-1] = 0;

	printf("read cached data= %s\n", inp);

	return inp[0];

error:
	if(f) {
		fclose(f);
	}

	return -1;
#undef BUFLEN
}


int main()
{
// Q3: make a call to fetch_data that reads /etc/environment
//     note that there are multiple ways to solve this
//     include as many ways as you like!
////QUESTION_3_START

	// replace this value with something that will cause /etc/environment to be read
	const char *param = “etc/environment“;

////QUESTION_3_END

	printf("fetch_data= %d\n", fetch_data(param));
	return 1;
}

