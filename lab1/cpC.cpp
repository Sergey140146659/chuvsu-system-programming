#include <stdio.h>

const int buffer_size = 4096;

int main(int argc, char* argv[]) {
    FILE* from_file = fopen(argv[1], "rb");
    if (!from_file) {
        perror("Error opening source file\n");
        return 1;
    }

    FILE* to_file = fopen(argv[2], "wb");
    if (!to_file) {
        perror("Error creating destination file\n");
        fclose(from_file);
        return 1;
    }

    char buffer[buffer_size];
    size_t from_file_info;
    while ((from_file_info = fread(buffer, 1, buffer_size, from_file)) > 0) {
        fwrite(buffer, 1, from_file_info, to_file);
    }

    fclose(from_file);
    fclose(to_file);
    printf("File copied successfully\n");
    return 0;
}