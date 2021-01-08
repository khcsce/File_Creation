# NAME: Khoa Quach
# EMAIL: khoaquachschool@gmail.com
# ID: 105123806
CC = gcc
CFLAGS = -Wall -Wextra -g
.SILENT:

default:
	$(CC) $(CFLAGS) -o lab0 lab0.c

check: default basic_one basic_two basic_three invalid_option nonwritable_outputfile noexist_inputfile segfault_test catch_test 	
	if [ -s log.txt ]; \
	then \
		echo "OH NO ERRORS EXISTS" \
	        cat log.txt; \
		rm -f log.txt; \
	else \
	        echo "Smoke Check SUCCESSFUL"; \
	fi
clean:
	rm -f lab0 *.o *.tar.gz *~

dist: 
	tar -czvf lab0-105123806.tar.gz lab0.c Makefile *.png README
#Smoke Check cases
basic_one:
	echo "hello word" > file.txt; \
	./lab0 --input=file.txt --output=file2.txt; \
	cmp file.txt file2.txt; \
	if [[ $$? -ne 0 ]]; \
	then \
		echo "Error: input file to output file comparison failure" > log.txt; \
	fi; \
	rm -f file.txt file2.txt

# no comparison between input file and output file
basic_two:
	echo "hello world" > file.txt; \
	./lab0 --input=file.txt --output=file2.txt; \
	if [[ $$? -ne 0 ]]; \
	then \
		echo "Error: input file to output file (no exit code = 0)" > log.txt; \
	fi; \
	rm -f file.txt file2.txt
# basic stdin to std out (no files)
basic_three:
	echo "hello world" | ./lab0; \
	if [[ $$? -ne 0 ]]; \
	then \
		echo "Error: copy stdin to stdout: no exit code = 0" > log.txt; \
	fi;
# invalid option
invalid_option:
	echo "hello world" | ./lab0 --what; \
	if [[ $$? -ne 1 ]]; \
	then \
		echo "Error: unrecognizable option: no exit code = 1. Use --input=filename, --output=filename, --segfault, or --catch" > log.txt; \
	fi;
# nonwritable output file
nonwritable_outputfile:
	touch output.txt; chmod -w output.txt; \
	./lab0 --output=output.txt; \
	if [[ $$? -ne 3 ]]; \
        then \
                echo "Error: unwritable output file case: no exit code = 3" > log.txt; \
        fi; \
	rm -f output.txt;
# input file that doesn't exists


noexist_inputfile: 
	./lab0 --input=noexist.txt; \
	if [[ $$? -ne 2 ]]; \
        then \
                echo "Error: invalid input file (not existsing) case: no exit code = 2" > log.txt; \
        fi;
# segmentation fault and --check case
segfault_test:
	./lab0 --segfault; \
	if [[ $$? -ne 139 ]]; \
	then \
		echo "Error: segfault case. no error code = 139" > log.txt; \
	fi;
catch_test:
	./lab0 --segfault --catch; \
	if [[ $$? -ne 4 ]]; \
	then \
		echo "Error: --catch case. no error code = 4" > log.txt; \
	fi;

