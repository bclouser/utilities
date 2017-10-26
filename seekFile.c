#define _FILE_OFFSET_BITS 64
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    /* Arguments: fd [offset [whence]]
     * where
     * fd: file descriptor to seek
     * offset: number of bytes from position specified in whence
     * whence: one of
     *  SEEK_SET (==0): from start of file
     *  SEEK_CUR (==1): from current position
     *  SEEK_END (==2): from end of file
     */
    int fd;
    char* fileName = NULL;
    long long scan_offset = 0;
    off_t offset = 0;
    int whence = SEEK_SET;
    int errsv; int rv;
    if (argc == 1) {
        fprintf(stderr, "usage: seekFile file [offset [whence]]\n");
        exit(1);
    }
    if (argc >= 2) {
        printf("argv[1]=%s\n",argv[1]);
        fileName = argv[1];
    }
    if (argc >= 3) {
        scan_offset = argv[2];
        offset = (off_t) scan_offset;
    }
    if (argc >= 4) {
        if (sscanf(argv[3], "%d", &whence) == EOF) {
            errsv = errno;
            fprintf(stderr, "%s: %s\n", argv[0], strerror(errsv));
            exit(1);
        }
    }

    fd=open(fileName);

    if(!fd){
        printf("failed to open file\n");
        exit(2);
    }

    printf("About to seek file to offset %d\n", offset);
    if (lseek(fd, offset, whence) == (off_t) -1) {
        errsv = errno;
        fprintf(stderr, "%s: %s\n", argv[0], strerror(errsv));
        close(fd);
        exit(2);
    }

    return 0;
}

