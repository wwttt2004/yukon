
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE

#define _GNU_SOURCE
#define _BSD_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <memory.h>
#include <string.h>

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc != 4) {
		printf("Usage: %s [binary] [search] [replace]\n", argv[0]);
		return 0;
	}
	
	int len = strlen(argv[2]);
	
	if (len != strlen(argv[3])) {
		printf("Usage: %s [binary] [search] [replace]\n", argv[0]);
		printf("!!! [search] and [replace] need to have the same length\n");
		return 0;
	}
	
	int fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		printf("Could not open binary\n");
		return 0;
	}
	
	struct stat sbuf;
	fstat(fd, &sbuf);
	
	char *mem = mmap(0, sbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	for (int i = 0; i < sbuf.st_size - len; ++i) {
		if (memcmp(mem + i, argv[2], len) == 0) {
			memcpy(mem + i, argv[3], len);
		} 
	}
	
	munmap(mem, sbuf.st_size);
	close(fd);

	return 0;
}
