#!/bin/bash

FILES="question_1
question_2
question_3
question_4
question_5
question_6"

rm -f output

for item in $FILES 
do
	awk '/QUESTION_._START/,/QUESTION_._END/' ${item}.c >> output
	./$item >> output
	echo "------------------------------------------------------------" >> output
done

