/* compile with gcc dtmf_detect.c -o dtmf_detect.exe -lspandsp */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <spandsp.h>

#define FORMAT_INVALID 0 
#define FORMAT_ALAW 1
#define FORMAT_SLINEAR 2 

#define CHUNK_SAMPLES 160

#define USAGE "USAGE: %s [alaw|slinear] [alaw or slinear file path]\n"

static void on_dtmf_detected(void *usrdata, const char *digits, int len)
{
	printf("Detected %s\n", digits);
}

int main(int argc, char *argv[])
{
	struct stat statbuf;
	FILE *audiofp;
	short slinear_buffer[CHUNK_SAMPLES];
	char alaw_buffer[CHUNK_SAMPLES];
	char *chunk_buffer;
	size_t chunksize = 0;
	int format = FORMAT_INVALID;
	int i = 0;
	char digit = 0;
	char currdigit = 0;
	dtmf_rx_state_t *rxstate = NULL;

	printf("Running DTMF Detection Test - alaw or slinear 8000hz only\n");

	if (argc < 3) {
		fprintf(stderr, USAGE, argv[0]);
		exit(1);
	}

	if (!strncasecmp(argv[1], "alaw", sizeof("alaw")-1)) {
		format = FORMAT_ALAW;
		chunksize = sizeof(alaw_buffer);
		chunk_buffer = alaw_buffer;
	} else if (!strncasecmp(argv[1], "slinear", sizeof("slinear")-1)) {
		format = FORMAT_SLINEAR;
		chunksize = sizeof(slinear_buffer);
		chunk_buffer = (char *)slinear_buffer;
	} else {
		fprintf(stderr, USAGE, argv[0]);
		exit(1);
	}

	printf("Using file %s\n", argv[2]);
	if (stat(argv[2], &statbuf)) {
		perror("could not stat audio file");
		exit(1);
	}

	audiofp = fopen(argv[2], "r");
	if (!audiofp) {
		perror("could not open audio file");
		exit(1);
	}

	rxstate = dtmf_rx_init(rxstate, on_dtmf_detected, NULL);
	printf("reading chunks of %d bytes\n", chunksize);
	while (fread(chunk_buffer, chunksize, 1, audiofp) == 1) {
		if (format == FORMAT_ALAW) {
			/* chunksize == bytes == samples */
			for (i = 0; i < chunksize; i++) {
				slinear_buffer[i] = alaw_to_linear(chunk_buffer[i]);
			}
		} 
		dtmf_rx(rxstate, slinear_buffer, CHUNK_SAMPLES);
		digit = dtmf_rx_status(rxstate);
		if (digit) {
			currdigit = digit;
			printf("%c ON\n", currdigit);
		} else if (currdigit) {
			printf("%c OFF\n", currdigit);
			currdigit = 0;
		}
	}
	dtmf_rx_free(rxstate);
	fclose(audiofp);
	return 0;
}

