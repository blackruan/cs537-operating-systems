#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "sort.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>

void usage() {
	fprintf(stderr, "Usage: rangesort -i inputfile -o outputfile -l lowvalue -h highvalue\n");
	exit(1);
}

int compare (const void *a, const void *b) {
	return ((const rec_t *) a)->key - ((const rec_t *) b)->key;
}

int main(int argc, char *argv[]) {
	char *inFile = "/no/such/file";
	char *outFile = "/no/such/file"; 
	unsigned long int lowValue_ul = 0, highValue_ul = 0;
	unsigned int lowValue_u, highValue_u;
	char *pEnd;

	int c;
    opterr = 0;
	errno = 0;
	while ((c = getopt(argc, argv, "i:o:l:h:")) != -1 ) {
		switch (c) {
			case 'i':
				inFile = strdup(optarg);
				break;
			case 'o':
				outFile = strdup(optarg);
				break;
			case 'l':
				lowValue_ul = strtoul(optarg, &pEnd, 10);
				if (errno == ERANGE || *pEnd != '\0') {
					fprintf(stderr, "Error: Invalid range value\n");
					exit(1);
				}
				break;
			case 'h':
	            highValue_ul = strtoul(optarg, &pEnd, 10);
				if (errno == ERANGE || *pEnd != '\0') {
					fprintf(stderr, "Error: Invalid range value\n");
					exit(1);
				}
				break;
			default:
				usage();
		}
	}

	// Too few or many arguments
	if (argc != 9) {
		usage();
	}
	
	// Invalid files
	int inFD = open(inFile, O_RDONLY);
	if (inFD < 0) {
        fprintf(stderr, "Error: Cannot open file %s\n", inFile);
		exit(1);
	}
	int outFD = open(outFile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
	if (outFD < 0) {
		fprintf(stderr, "Error: Cannot open file %s\n", outFile);
		exit(1);
	}

	// Invalid range
	if (highValue_ul < lowValue_ul || highValue_ul > UINT_MAX) {
		fprintf(stderr, "Error: Invalid range value\n");
		exit(1);
	}
	lowValue_u = lowValue_ul;
	highValue_u = highValue_ul;

    struct stat buf;
    int err = fstat(inFD, &buf);
	if (err != 0) {
        fprintf(stderr, strerror(errno));
		exit(1);
	}
	int numRecords = (buf.st_size) / sizeof(rec_t);
	// Number of records left after discard
	int numRecLeft = 0;
	
	rec_t *records = NULL;
	records = (rec_t *)malloc(numRecords * (sizeof(rec_t)));
	if (records == NULL) {
		fprintf(stderr, "Error: Cannot allocate memory\n");
		exit(0);
	}
	rec_t *r = records;
	while (1) {
		int rc = read(inFD, r, sizeof(rec_t));
		if (rc == 0) {
			break;  // end of the file
		}
		if (rc < 0) {
			fprintf(stderr, "Error: Cannot read file %s\n", inFile);
		}
		if (r->key >= lowValue_u && r->key <= highValue_u) {
			++r;
			++numRecLeft;
		}
	}

	qsort(records, numRecLeft, sizeof(rec_t), compare);

	int i;
	r = records;
	for(i = 0; i < numRecLeft; ++i) {
		int rc = write(outFD, r, sizeof(rec_t));
		if (rc != sizeof(rec_t)) {
			fprintf(stderr, "Error: Cannot write to file %s\n", outFile);
			exit(1);
		}
		++r;
	}
	
	free(records);
	free(inFile);
	free(outFile);
	close(inFD);
	close(outFD);

	return 0;
}
