//NAME: Khoa Quach
//EMAIL: khoaquachschool@gmail.com
//ID: 105123806
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
/*
Summary of exit codes
0 ... copy successful
1 ... unrecognized argument
2 ... unable to open input file
3 ... unable to open output file
4 ... caught and received SIGSEGV
*/
/*-----------------Helper functions--------------------*/
// Segfault
void segfault_activate()
{
	char* seg_pointer = NULL;
       	*seg_pointer = 's';
}

// For  case c (catch), call this function
void sigsev_handler(int sig)
{
	if(sig == SIGSEGV) { 
		fprintf(stderr, "Segmentation fault (SIGSEGV handler that catches the segmentation fault)\n");
		exit(4); // exit(2) with a return code of 4.

	}
}

void read_write()
{
	char buf;
	ssize_t s = read(0,&buf,1);
	while(s > 0)
	{
		if(write(1,&buf,1) < 0)
		{
			fprintf(stderr, "--output error: Cannot write to output file: %s\n",strerror(errno));
			exit(3);
		}
		s = read(0,&buf,1);
	}
	if (s < 0){
	  fprintf(stderr, "--input error: Cannot read from input file: %s\n",strerror(errno));
	  exit(2);
	}
}
int main(int argc, char **argv){
	// Input and Output files
	int ifd;
	int ofd;
	//http://web.cs.ucla.edu/~harryxu/courses/111/winter21/ProjectGuide/fd_juggling.html
	// intialize to zero
	int seg_flag = 0;
	//int catch = 0;
	int choice = 0; // used with getopt_long
	// Command line options, used with getopt_long
	static struct option options[] = 
	{
		{"input", 1, 0, 'i'},
		{"output", 1, 0, 'o'},
		{"segfault",0,0,'s'},
		{"catch", 0, 0, 'c'},
		{0,0,0,0}
	};

	while((choice = getopt_long(argc, argv, "", options,NULL))!= -1)
	{
		switch(choice){
			case 'i':
				ifd = open(optarg, O_RDONLY);
				if (ifd >= 0) {
					close(0);
					dup(ifd);
					close(ifd);
				}
				else {
					fprintf(stderr, "--input option error: Could not open the input file %s ", optarg);
					fprintf(stderr, "%s\n", strerror(errno));
					exit(2);
					
				}
				break;
			// case i end
			case 'o':
				ofd = creat(optarg,0666);
				if (ofd >= 0) {
					close(1);
					dup(ofd);
					close(ofd);
				}
				else {
					fprintf(stderr, "--output option error: Could not create the output file or not writable %s ", optarg);
					fprintf(stderr, "%s\n", strerror(errno));
					exit(3);
					
				}
				break;
			// case o end
			case 's':
				seg_flag = 1;
				break;
			case 'c':
				signal(SIGSEGV, sigsev_handler);
				break;
			case '?':
				fprintf(stderr, "unrecognized argument! Correct usage: ./lab0 --input filename --output filename, --segfault, --catch\n");
				exit(1);
			default:
				fprintf(stderr, "unrecognized argument! Correct usage: ./lab0 --input filename --output filename, --segfault, --catch\n");
				exit(1);
		}// switch
	}// while
	if (seg_flag)
		segfault_activate();
	read_write();
	exit(0);
} // main
