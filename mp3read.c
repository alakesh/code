/* A small C program to read genre of an mp3 file *
 * Usage:  ./mp3read  <filename>
 * Author: Alakesh Haloi 
 * This is a free software distributed under GNU GPL version 3
 */

#include<stdio.h>
#include<stdlib.h>

#define ID3LEN 128
#define BUFLEN 129
FILE *fp;

char *genre[] = {
	"Blues",
     "Classic Rock",
     "Country",
     "Dance",
     "Disco",
     "Funk",
     "Grunge",
     "Hip-Hop",
     "Jazz",
     "Metal",
     "New Age",
     "Oldies",
     "Other",
     "Pop",
     "R&B",
     "Rap",
     "Reggae",
     "Rock",
     "Techno",
     "Industrial",
     "Alternative",
     "Ska",
     "Death Metal",
     "Pranks",
     "Soundtrack",
     "Euro-Techno",
     "Ambient",
     "Trip-Hop",
     "Vocal",
     "Jazz+Funk",
     "Fusion",
     "Trance",
     "Classical",
     "Instrumental",
     "Acid",
     "House",
     "Game",
     "Sound Clip",
     "Gospel",
     "Noise",
     "AlternRock",
     "Bass",
     "Soul",
     "Punk",
     "Space",
     "Meditative",
     "Instrumental Pop",
     "Instrumental Rock",
     "Ethnic",
     "Gothic",
     "Darkwave",
     "Techno-Industrial",
     "Electronic",
     "Pop-Folk",
     "Eurodance",
     "Dream",
     "Southern Rock",
     "Comedy",
     "Cult",
     "Gangsta",
     "Top 40",
     "Christian Rap",
     "Pop/Funk",
     "Jungle",
     "Native American",
     "Cabaret",
     "New Wave",
     "Psychadelic",
     "Rave",
     "Showtunes",
     "Trailer",
     "Lo-Fi",
     "Tribal",
     "Acid Punk",
     "Acid Jazz",
     "Polka",
     "Retro",
     "Musical",
     "Rock & Roll",
     "Hard Rock" };  //79



int main(int argc, char **argv)
{
	int ret=0;
	int read=0;
	int i=0;
	int gen=0;
	char bytes[BUFLEN];
	char *filename = argv[1];
	bytes[ID3LEN]='\0';
	if(argc<2){
		printf("USAGE: ./mp3read <filename>\n");
		exit(-1);
	}
	fp = fopen(filename, "r");
	if(ret=fseek(fp,-ID3LEN,SEEK_END))
		exit(-1);
	read=fread((void *)bytes,1,ID3LEN,fp);
	gen=bytes[127];
	printf("Genre is %s \n",genre[gen]);
	
}
