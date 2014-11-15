/************** A Simple MP3 Player for linux command line ********************
 * Written by Alakesh Haloi { alakesh.haloi@gmail.com }
 *
 * For mp3 info refer to http://www.mpgedit.org/mpgedit/mpeg_format/mpeghdr.htm
 *****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


struct mp3frame_h {
    uint16_t frame_sync_bits    : 11;
    uint8_t mpeg_audio_version  : 2;
    uint8_t layer_desc          : 2;
    uint8_t protection_bit      : 1;
    uint8_t bitrate_index       : 4;
    uint8_t sampling_freq       : 2;
    uint8_t padding_bit         : 1;
    uint8_t private_bit         : 1;
    uint8_t channel_mode        : 2;
    uint8_t mode_extension      : 2;
    uint8_t copyright           : 1;
    uint8_t original            : 1;
    uint8_t emphasis            : 1;
};

void print_frame (void *p){
    struct mp3frame_h *f = (struct mp3frame_h *)p;
    printf("frame_sync_bits:    %x \n", f->frame_sync_bits); 
    printf("mpeg_audio_version: %x \n", f->mpeg_audio_version); 
    printf("layer_desc:         %x \n", f->layer_desc); 
    printf("protection_bit: %x \n", f->protection_bit); 
    printf("bitrate_index:  %x \n", f->bitrate_index); 
    printf("sampling_freq:  %x \n", f->sampling_freq); 
    printf("padding_bit:    %x \n", f->padding_bit); 
    printf("private_bit:    %x \n", f->private_bit); 
    printf("channel_mode:   %x \n", f->channel_mode); 
    printf("mode_extension: %x \n", f->mode_extension); 
    printf("copyright:      %x \n", f->copyright); 
    printf("original:       %x \n", f->original); 
    printf("emphasis:       %x \n", f->emphasis); 
}

int is_id3(char *p, char *e)
{
	if (p > e) return 0;
	if (e - p < 10) return 0;
	if (p[0] == 'I' && p[1] == 'D' && p[2] == '3') {
		return 1;
	}
	return 0;
}

int is_mp3_frame(char *p, char *e)
{
	if (p > e) return 0;
	if (e - p < 4) return 0;

	/* look for sync pattern */
	if (p[0] == 0xff && (p[1] & 0xe0) = 0xe0) {
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "r");
	unsigned char *buf, *p, *e, *q;
	long length, skip;
	long hbytes = 0;
	fseek(f, 0, SEEK_END);
	length = ftell(f);
	if (length <= 0) {
		fprintf(stderr, "could not find end of file \n");
		return 1;
	}

	fprintf(stdout, "file has %ld bytes \n", length);
	fseek(f, 0, SEEK_SET);

	buf = malloc(length);
	if (!buf) {
		fprintf(stderr, "could not allocate buffer for file data %ld bytes\n",
				length);
		return 2;
	}
	if (fread(buf, 1, length, f) < length) {
		fprintf(stderr, "could not read the whole file \n");
		return 3;
	}

	e = buf + length - 2;
	p = buf;
	q = p;

	while (p < e) {
		if(is_id3(p, e)) {
			/* adjust pointers */
			p = p + 10;
			q = p;
		} else if(is_mp3_frame(p, e)) {
			/* read mp3 frame */
			/* dump frame info */
		}
	}

	free(buf);
    if(f)
        fclose(f);
	return 0;
}
