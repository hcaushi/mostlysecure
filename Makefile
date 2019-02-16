
CFLAGS=-std=c99 -Wall -Wextra -Wpedantic

all: question_1 question_2 question_3 question_4 question_5 question_6

.PHONY: test clean

test:
	$(shell ./run.sh)
	

clean:
	rm -f question_1 question_2 question_3 question_4 question_5 question_6 output

question_1: question_1.c
	$(CC) -o question_1 $(CFLAGS) question_1.c

question_2: question_2.c
	$(CC) -o question_2 $(CFLAGS) question_2.c

question_3: question_3.c
	$(CC) -o question_3 $(CFLAGS) question_3.c

question_4: question_4.c
	$(CC) -o question_4 $(CFLAGS) question_4.c

question_5: question_5.c
	$(CC) -o question_5 $(CFLAGS) question_5.c aes.c

question_6: question_6.c
	$(CC) -o question_6 $(CFLAGS) question_6.c


