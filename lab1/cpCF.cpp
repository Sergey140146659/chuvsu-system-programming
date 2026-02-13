#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (!CopyFileA(argv[1], argv[2], FALSE)) {
        printf("Copy failed\n");
        return 1;
    }

    printf("File copied successfully\n");
    return 0;
}