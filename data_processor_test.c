/*
 * data_processor_test.c
 *
 *  Created on: Oct 22, 2013
 *      Author: heavey
 */
#include <stdio.h>
#include <unistd.h>
#include "data_processor.h"

static int read_data(const char *filename, kiss_fft_cpx *cin, int n) {
	int i = 0;
	char line[1024];
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		perror("fopen");
		return -1;
	}

	//line: cnt=0_ch1=1091_ch2=939_ch3=2325_ch4=913
	while (fgets(line, sizeof(line), fp) != NULL) {
		char *p = line;
		p = strstr(p, "ch3=");
		if (!p)
			continue;
		cin[i].i = 0;
		cin[i++].r = atoi(p + 4);
		if (i >= n)
			break;
		//printf("i=%d, %s, (%f)", i, p+4, in[i-1]);
	}
	while (i < n) {
		cin[i].i = 0;
		cin[i++].r = 0;
	}
	fclose(fp);
	return 0;
}


static int write_data(const char *filename, const kiss_fft_cpx *cout, int n) {
	FILE *fp = fopen(filename, "w+");
	if (!fp) {
		perror("fopen");
		return -1;
	}
	while (n-- > 0) {
		fprintf(fp, "(%12f, %12f)\n", cout->r, cout->i);
		cout++;
	}
	fclose(fp);
	return 0;
}

int main(int argc,char ** argv) {
	const char *infile = "dump-raw-3.txt";
	const char *outfile = "test-output.txt";
	if (argc == 3) {
		infile = argv[1];
		outfile = argv[2];
	}
	float hz = 0;
	data_processor_t dfft = NULL;
	dfft = data_processor_init(2048, 214000);
	if (!dfft) {
		fprintf(stderr, "data_processor_init error\n");
		return -1;
	}

	read_data(infile, dfft->cin, dfft->nfft);

	int i = 100;
	while (i-- > 0) {
		hz = data_processor_run(dfft);
		printf("hz = %12f\n", hz);
	}

	write_data(outfile, dfft->cout, dfft->nfft);

	data_processor_close(dfft);
	dfft = NULL;
	return 0;
}

