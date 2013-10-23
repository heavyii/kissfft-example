#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "data_processor.h"

static void convert_to_freq(kiss_fft_cpx *cout, int n) {
	const float NC = n/2.0 + 1;
	while (n-- > 0) {
		cout->r /= NC;
		cout->i /= NC;
		cout++;
	}
}

static void complex_abs(kiss_fft_cpx *cout, int n) {
	while (n-- > 0) {
		cout->r = sqrt(cout->r * cout->r + cout->i * cout->i);
		cout->i = 0;
		cout++;
	}
}

static int get_peak_pos(const kiss_fft_cpx *cout, int nfft, int start_pos) {
	int pos = 0;
	float maxdata = 0;
	int i;
	for (i = start_pos; i < nfft/2; i++) {
		if ((cout[i].r - maxdata) > 0.0001) {
			maxdata = cout[i].r;
			pos = i;
		}
	}
	return pos;
}

static float get_peak_frequence(const kiss_fft_cpx *cout, int nfft, float start_hz, float sample_hz) {
	int start_pos = start_hz * nfft / sample_hz;
	return get_peak_pos(cout, nfft, start_pos) * sample_hz / nfft;
}


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
	kiss_fft(dfft->kiss_fft_state, dfft->cin, dfft->cout);
    convert_to_freq(dfft->cout, dfft->nfft);
    complex_abs(dfft->cout, dfft->nfft);
    return get_peak_frequence(dfft->cout, dfft->nfft, 500.0, dfft->sample_hz);
}

void data_processor_close(data_processor_t dfft) {
	free(dfft->cin);
	free(dfft->cout);
	free(dfft->kiss_fft_state);
	free(dfft);
}
