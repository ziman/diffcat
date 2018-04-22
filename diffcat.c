#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define STDIN  0
#define STDOUT 1
#define STDERR 2
#define BUFSIZE 4096

void die(const char * msg)
{
	perror(msg);
	exit(1);
}

int main(void)
{
	char buf_in[BUFSIZE];
	char buf_out[BUFSIZE];
	off_t offset = 0;
	ssize_t bytes_read;

	while (bytes_read = read(STDIN, buf_in, BUFSIZE))
	{
		off_t ret_in = lseek(STDOUT, offset, SEEK_SET);
		if (ret_in < 0) die("lseek in outfile");
		
		ssize_t ret_out = read(STDOUT, buf_out, bytes_read);
		if (ret_out < 0) die("read from outfile");

		if (memcmp(buf_in, buf_out, bytes_read))
		{
#ifdef DEBUG
			fprintf(stderr, "overwriting at 0x%zx\n", offset);
#endif
			// outfile contains a different block, overwrite
			ret_in = lseek(STDOUT, offset, SEEK_SET);
			if (ret_in < 0) die("lseek in outfile before write");

			ssize_t bytes_written = write(STDOUT, buf_in, bytes_read);
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
