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


int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "r");
    struct mp3frame_h *fr = (struct mp3frame_h *)malloc(sizeof(struct mp3frame_h));
    if (fr) {
        fread((void *)fr, sizeof(struct mp3frame_h), 1, f);
        print_frame((void *)fr);
        free(fr);
    }

    if(f)
        fclose(f);
      
}
