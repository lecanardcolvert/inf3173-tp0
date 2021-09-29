/*  INF3173 - TP0 
 *  Session : automne 2021
 *  Alexandre H. Bourdeau (HAMA12128907)
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

const char * OUTPUT_DIR = "./copies/";
unsigned int copied_bytes = 0;

void quit(int exit_code) {
    printf("%u\n", copied_bytes);
    exit(exit_code);
}

void generate_output_file_path(char * input_file_path, char * output_file_path) {
    char * strrchr_result = strrchr(input_file_path, '/');
    
    strcpy(output_file_path, OUTPUT_DIR);
    
    (strrchr_result == NULL) ?
        strcat(output_file_path, input_file_path) :
        strcat(output_file_path, strrchr_result);
}

void make_dir() {
    const __mode_t MODE = 0777;
    int mkdir_result = mkdir(OUTPUT_DIR, MODE);

    ((mkdir_result == -1) && (errno != EEXIST)) ? quit(1) : NULL;
}

void make_file(char * output_file_path, char * data) {
    const int MODE = 0644;
    const mode_t FLAGS = O_CREAT|O_WRONLY|O_TRUNC;
    int close_result;
    int write_result;
    
    int fd = open(output_file_path, FLAGS, MODE);
    (fd == -1) ? quit(1) : NULL;
    
    write_result = write(fd, data, strlen(data));
    (write_result == -1) ? quit(1) : NULL;
    
    copied_bytes = copied_bytes + write_result;
    
    close_result = close(fd);
    (close_result == -1) ? quit(1) : NULL;
}

int open_file(char * file_path) {
    char complete_file_path[2 + strlen(file_path)];
    strcpy(complete_file_path, "./");
    strcat(complete_file_path, file_path);
    
    int fd = open(complete_file_path, O_RDONLY);
    (fd == -1) ? quit(1) : NULL;
    
    return fd;
}

void position_offset(unsigned int fd, unsigned int offset) {
    const int WHENCE = SEEK_SET;
    int lseek_result = lseek(fd, offset, WHENCE);
    
    (lseek_result != offset) ? quit(1) : NULL;
}

void read_file(unsigned int fd, char * data, unsigned int count) {
    int read_result = read(fd, data, count);
    
    (read_result == -1) ? quit(1) : NULL;
}

int main(int argc, char ** argv) {
    make_dir();
    
    for (unsigned int i = 1; i < argc; i++) {
        if (i % 2 == 0) {
            char data[4096];
            char output_file_path[4096];
            char * input_file_path = argv[i - 1];
            unsigned int bytes;
            unsigned int fd;
            unsigned int offset;
            
            sscanf(argv[i], "%u-%u", &offset, &bytes);
            fd = open_file(input_file_path);
            position_offset(fd, offset);
            read_file(fd, data, bytes);
            generate_output_file_path(input_file_path, output_file_path);
            make_file(output_file_path, data);
        }
    }

    quit(0);
    return 0;
}
