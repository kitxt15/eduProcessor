#include <cstddef>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "colors.h"
#include "errno.h"
#define MAX_LENGTH  10*1000

struct Line {
    char* buffer;
    size_t len;
    int arg;
    int command_len;
};

struct Text{
     struct Line index_arr[MAX_LENGTH];
     char* text_buffer;
};

size_t GetSizeOfFile(const char* filename);
int ReadFile(const char* filename, struct Line data[], char** buffer);
size_t CountLenOfLines(const struct Line data[]);


size_t GetSizeOfFile(const char* filename) {

    struct stat baff = {};

    if (stat(filename, &baff)) {
        return 0;
    }

    return baff.st_size;

}

int ReadFile(const char* filename, struct Line data[], char** buffer) {

    int fd = open(filename, O_RDONLY);

    size_t flen = GetSizeOfFile(filename);

    void* ptr = calloc(1, flen+8); //8 для запаса
    *buffer = (char* )ptr;

    if (fd == -1){ printf(RED "ERROR: Не удалось открыть файл \"%s\"!\n" RESET, filename); return -errno; } //done


    size_t bytesRead = read(fd, ptr, flen);
    int count = 0;
    int index_count = 1;
    int len_count = 0;
    data[0].buffer = (char *)ptr;

    while (count < bytesRead) {

        if (*((char*)ptr + count + 1)  &&  *((char*)ptr + count) == '\n') {

            *((char*)ptr + count) = '\0';
            count++;

            data[index_count].buffer = (char*)ptr + count;

            data[index_count-1].len = len_count;
            len_count = 0;

            index_count++;
            continue;
        }
        else if( ! *((char*)ptr + count + 1)  &&  *((char*)ptr + count) == '\n'){
             *((char*)ptr + count) = '\0';
        }


        len_count++;
        count++;
    }

    data[index_count-1].len = len_count;
    return 0;

}

size_t CountLenOfLines(const struct Line data[]){
    if (!data[0].buffer){
        return 0;
    }
    size_t len = 0;
    while (data[len].buffer) {
        len++;
    }
    return len;
}
