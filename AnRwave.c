#include <stdio.h>
#include <string.h>
#include <stdint.h>

/*
Offset 	Size 	Description 	Value
0x00 	4 	Chunk ID 	"RIFF" (0x52494646)
0x04 	4 	Chunk Data Size 	(file size) - 8
0x08 	4 	RIFF Type 	"WAVE" (0x57415645)
0x10 	    Wave chunks
*/
struct riff_type_chunk {
	uint32_t chunk_id;
	uint32_t chunk_data_size;
	uint32_t riff_type;
};


/*
Offset 	Size 	Description
0x00 	4 	Chunk ID
0x04 	4 	Chunk Data Size
0x08 	    Chunk Data Bytes
*/

struct wave_file_chunk {
	uint32_t chunk_id;
	uint32_t chunk_data_size;
	uint8_t	 data[];
};

/*
Offset 	Size 	Description 	Value
0x00 	4 	Chunk ID 	"fmt " (0x666D7420)
0x04 	4 	Chunk Data Size 	16 + extra format bytes
0x08 	2 	Compression code 	1 - 65,535
0x0a 	2 	Number of channels 	1 - 65,535
0x0c 	4 	Sample rate 	1 - 0xFFFFFFFF
0x10 	4 	Average bytes per second 	1 - 0xFFFFFFFF
0x14 	2 	Block align 	1 - 65,535
0x16 	2 	Significant bits per sample 	2 - 65,535
0x18 	2 	Extra format bytes 	0 - 65,535
0x1a 	    Extra format bytes *
*/

struct format_chunk {
	uint32_t chunk_id;
	uint32_t chunk_data_size;
	uint16_t compression_code;
	uint16_t number_channels;
	uint32_t sample_rate;
	uint32_t avg_bytes_per_sec;
	uint16_t block_align;
	uint16_t significant_bits_per_sample;
	uint16_t extra_fmt_bytes_cnt;
	uint8_t  bytes[];
};


/* 
Offset 	Length 	Type 	Description 	Value
0x00 	4 	char[4] 	chunk ID 	"data" (0x64617461)
0x04 	4 	dword 	chunk size 	depends on sample length and compression
0x08 	            sample data
*/
struct data_chunk {
		uint32_t chunk_id;
		uint16_t chunk_size;
		uint8_t  data[];
};


int main(int argc, char *argv[]){
		struct riff_type_chunk rtc;
		struct wave_file_chunk wfc;

		uint32_t to_read = 0;

		FILE *fp = NULL;
		int n=0;
		if (argc < 2)
			printf("usage: %s [wave_file]\n", argv[0]);

		if ((n=strlen(argv[1]))>4){
				if(strnstr(argv[1],".wav",n)==NULL)
					printf(".wav extension missing in file\n");
		}
		fp = fopen(argv[1], "r");
		if (fp == NULL) {
			printf("Could not open %s \n", argv[1]);
			return -1;
		}

		fread(&rtc, sizeof rtc, 1, fp);
		if (rtc.chunk_id == 0x46464952) {
			printf("riff file size:  %d \n", rtc.chunk_data_size);
			to_read = rtc.chunk_data_size;
			if (rtc.riff_type == 0x45564157) {
				printf("riff type:  wave\n");
			}
		} else {
			printf("Unsupported file format\n");
			fclose(fp);
			return -1;
		}

		while (to_read > 0){
			int i=0;
			uint8_t byte;
			fread(&wfc, sizeof wfc, 1, fp);
			printf("chunk_id: %x chunk_data_size: %d \n",
							wfc.chunk_id, wfc.chunk_data_size);
			to_read = to_read - (8 + wfc.chunk_data_size);
			printf("%d more to read \n", to_read);
		}

		fclose(fp);
		return 0;
}

