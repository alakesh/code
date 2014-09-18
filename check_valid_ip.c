#include <stdio.h>


int is_valid_ip(char *ip) {
    char c;
    int dot=0;
    int gap=0;
    int i=0;
    int tmp=0;
    while((c = ip[i]) != '\0'){
        printf("%c", c);
        if (c == '.')
        {
            if (tmp > 255)
                return 0;
            if (gap > 3)
            {
                printf("gap: %d", gap);
                return 0;
            }
            else {
                dot++;
                gap = 0;
                tmp = 0;
            }
        } else {
            tmp = tmp*10 + (c - 48);
			printf("tmp: %d \n", tmp);
            gap++;
        }
        if (c != '.' && (c < 48 || c > 57))
            return 0;
        i++;
    }
    if (c == '\0' && (gap >  3 || tmp > 255))
        return 0;
    if (dot > 3)
        return 0;
	printf("tmp: %d \n", tmp);
    return 1;
}


int main(int argc, char *argv[])
{
	if (argc < 2)
		return;
	printf("\n %d \n", is_valid_ip(argv[1]));
}
