#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef BUFSIZE
#define BUFSIZE 4096
#endif

void die(const char * msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char * argv[])
{
	if (argc != 2)
	{
		printf("usage: zcat image.gz | ./diffcat /dev/mmcblk0\n");
		exit(1);
	}

	int f_out = open(argv[1], O_CREAT | O_RDWR);
	if (f_out < 0) die("open outfile");

	char buf_in[BUFSIZE];
	char buf_out[BUFSIZE];
	off_t offset = 0;
	ssize_t bytes_read;

	while (bytes_read = read(STDIN_FILENO, buf_in, BUFSIZE))
	{
		off_t ret_in = lseek(f_out, offset, SEEK_SET);
		if (ret_in < 0) die("lseek in outfile");
		
		ssize_t ret_out = read(f_out, buf_out, bytes_read);
		if (ret_out < 0) die("read from outfile");

		if (memcmp(buf_in, buf_out, bytes_read))
		{
#ifdef DEBUG
			fprintf(stderr, "overwriting at 0x%zx\n", offset);
#endif
			// outfile contains a different block, overwrite
			ret_in = lseek(f_out, offset, SEEK_SET);
			if (ret_in < 0) die("lseek in outfile before write");

			ssize_t bytes_written = write(f_out, buf_in, bytes_read);
			if (bytes_written != bytes_read) die("write to outfile");
		}
#ifdef DEBUG
		else {
			fprintf(stderr, "no need to overwrite 0x%zx\n", offset);
		}
#endif

		offset += bytes_read;
	}
}
