#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define BUFF_SIZE 1024

void cleanup(int fd, char *buf);

int main(int argc, char **argv) 
{

    ssize_t bytes_readed = -1, rd_offset = 0, buf_size = 0;
    char *buf;
    int fd = -1;

    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    buf_size = BUFF_SIZE + 1; // Plus null char
    buf = malloc(buf_size); 

    if (buf == NULL) 
    {
        perror("Error on allocating");
        exit(EXIT_FAILURE);
    }
    else
    {
        buf = memset(buf, 0, buf_size);
        buf[0] = '\0';
    }

    fd = open(argv[1], O_RDONLY);

    if (fd == -1) 
    {
        perror("Couldn't open the file");
        cleanup(fd, buf);
        exit(EXIT_FAILURE);
    }


    while((bytes_readed = read(fd, buf + rd_offset, buf_size - rd_offset)) > 0)
    {

        rd_offset += bytes_readed;

        if (rd_offset + bytes_readed >= buf_size)
        {
            buf_size *= 2;
            char *new_buf = realloc(buf, buf_size);

            if (!new_buf)
            {
                perror("Couldn't reallocate");
                cleanup(fd, buf);
                exit(EXIT_FAILURE);
            }

            buf = new_buf;
        }
    }

    buf[rd_offset] = '\0';

    if (bytes_readed == -1) 
    {
        perror("Couldn't read the file");
        cleanup(fd, buf);
        exit(EXIT_FAILURE);
    }

    cleanup(fd, buf);
    return 0;
}

void cleanup(int fd, char *buf) 
{
    if (fd != -1)
        close(fd);

    if (buf)
        free(buf);
}
