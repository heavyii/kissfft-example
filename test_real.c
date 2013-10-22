#include "kiss_fftr.h"
#include "_kiss_fft_guts.h"
#include <sys/times.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

int read_data(char *filename, kiss_fft_cpx *cin, int n) {
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
		cin[i++].r = (float) atoi(p + 4);
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

int write_data(char *filename, kiss_fft_cpx *cout, int n) {
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

void convert_to_freq(kiss_fft_cpx *cout, int n) {
	const float NC = n/2.0 + 1;
	while (n-- > 0) {
		cout->r /= NC;
		cout->i /= NC;
		cout++;
	}
}

void complex_abs(kiss_fft_cpx *cout, int n) {
	while (n-- > 0) {
		cout->r = sqrt(cout->r * cout->r + cout->i * cout->i);
		cout->i = 0;
		cout++;
	}
}

int get_peak_pos(kiss_fft_cpx *cout, int n) {
	int pos = 0;
	float maxdata = 0;
	int i;
	for (i = 0; i < n; i++) {
		if ((cout->r - maxdata) > 0.0001) {
			maxdata = cout->r;
			pos = i;
		}
		cout++;
	}
	return pos;
}

typedef struct data_processor {
	float sample_hz;
	int nfft;
    kiss_fft_cpx *cin;
    kiss_fft_cpx *cout;
    kiss_fft_cfg  kiss_fft_state;
}*data_processor_t;

data_processor_t data_processor_init(int nfft, int hz) {
	data_processor_t dfft = NULL;
	dfft = (data_processor_t)malloc(sizeof(struct data_processor));
	if (!dfft) {
		perror("malloc");
		return NULL;
	}
	//printf("dfft len = %d\n", sizeof(*data_processor_t));
	dfft->sample_hz = hz;
	dfft->nfft = nfft;
	dfft->cin = (kiss_fft_cpx *)malloc(sizeof(kiss_fft_cpx) * dfft->nfft);
	if (!dfft->cin) {
		perror("malloc cin");
		free(dfft);
		return NULL;
	}
	dfft->cout = (kiss_fft_cpx *)malloc(sizeof(kiss_fft_cpx) * dfft->nfft);
	if (!dfft->cout) {
		perror("malloc cout");
		free(dfft->cin);
		free(dfft);
		return NULL;
	}

	dfft->kiss_fft_state = kiss_fft_alloc(nfft,0,0,0);

	return dfft;
}

float data_processor_run(data_processor_t dfft) {
	int pos = 0;
	kiss_fft(dfft->kiss_fft_state, dfft->cin, dfft->cout);
    convert_to_freq(dfft->cout, dfft->nfft);
    complex_abs(dfft->cout, dfft->nfft);
    pos = get_peak_pos(dfft->cout + 1, dfft->nfft / 2) + 1;
    printf("pos = %d\n", pos);
    return 1.0 * pos * dfft->sample_hz / dfft->nfft;
}

void data_processor_close(data_processor_t dfft) {
	free(dfft->cin);
	free(dfft->cout);
	free(dfft->kiss_fft_state);
	free(dfft);
}

int main(int argc,char ** argv) {
	float hz = 0;
	data_processor_t dfft = NULL;
	dfft = data_processor_init(2048, 214000);
	if (!dfft) {
		fprintf(stderr, "data_processor_init error\n");
		return -1;
	}

	int i = 10;
	while (i-- > 0) {
		read_data("dump-raw-3.txt", dfft->cin, dfft->nfft);
		hz = data_processor_run(dfft);
		printf("hz = %12f\n", hz);
		write_data("test-output.txt", dfft->cout, dfft->nfft);
	}

	data_processor_close(dfft);
	dfft = NULL;
	return 0;
}
