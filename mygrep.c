#define _GNU_SOURCE
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUF_SIZE 1000000
#define PRINT_MORE 10000

char buf[BUF_SIZE*2] = {0};

static inline my_isprint(char c) {
	return isprint(c) || c == '\r' || c == '\n' || c == '\t';
}

int main(int argc, char** argv) {
	if (argc < 2)
		return 1;
	char* needle = argv[1];
	long needle_len = strlen(argv[1]);
	printf("searching for \"%s\"\n", needle);
	fflush(stdout);
	long prev_len = 0;
	long last_size = 0;
	while (1) {
		long read_size = read(0, buf+last_size, BUF_SIZE);
		if (read_size == 0 || read_size == -1)
			return 0;
		long data_size = last_size + read_size;
		while (data_size >= needle_len) {
			char *match = memmem(buf, data_size, needle, needle_len);
			if (match != NULL) {
				printf("===== %ld =====\n", match - buf + prev_len);
				long i;
				for (i=0; i<PRINT_MORE; i++) {
					if (!my_isprint(*(match+i))) {
						printf("\nInvisible char: 0x%x", *(match+i));
						break;
					}
					printf("%c", *(match+i));
				}
				printf("\n\n");
				fflush(stdout);
				long eaten = match - buf + needle_len;
				data_size -= eaten;
				memmove(buf, buf + eaten, data_size);
				prev_len += eaten;
			}
			else break;
		}
		last_size = data_size > needle_len ? needle_len : data_size;
		memmove(buf, buf + data_size - last_size, last_size);
		prev_len += data_size - last_size;
	}
	return 0;
}
