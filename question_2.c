
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>


//
// manage quoting and normalising case of input data
//
const char *quote(const char *input, char **output)
{
#define SZLIMIT 10

	if(*output == NULL) {
		*output = malloc(SZLIMIT + 2);
		if(!*output) {
			return NULL;
		}
	}

	char *buf = *output;
	unsigned int i;

	/* normalise and quote input */

	buf[0] = '"';

	strncpy(buf + 1, input, SZLIMIT);

	for(i = 1; i < strlen(buf); ++i) {
		if(buf[i] > 64 && buf[i] < 91) {
			buf[i] |= 32;
		} else {
			switch(buf[i]) {
				case 33:
				case 34:
				case 36:
				case 92:
					buf[i] = '.';
					continue;
			}
		}
	}

	strcat(buf, "\"");

	return buf;
	
}

int main()
{
	char *moo = NULL;
	printf("%s\n", quote("moobaa", &moo));

//Q2: make calls to the quote function such that this program will crash
//
////QUESTION_2_START
	printf("%s\n", quote("Here is a sentence!@!>?", NULL));
////QUESTION_2_END

	free(moo);

	return 1;
}

