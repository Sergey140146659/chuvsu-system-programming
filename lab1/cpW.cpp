#include <windows.h>
#include <stdio.h>

const int buffer_size = 4096;

int main(int argc, char* argv[]) {
    HANDLE from_handle = CreateFileA(
        argv[1],
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN,
        NULL
    );
    if (from_handle == INVALID_HANDLE_VALUE) {
        printf("Error opening source file\n");
        return 1;
    }

    HANDLE to_handle = CreateFileA(
        argv[2],
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_FLAG_SEQUENTIAL_SCAN,
        NULL
    );
    if (to_handle == INVALID_HANDLE_VALUE) {
        printf("Error creating destination file\n");
        CloseHandle(from_handle);
        return 1;
    }

    char buffer[buffer_size];
    DWORD from_file_info;
    DWORD bytes_written;
    while (ReadFile(from_handle, buffer, buffer_size, &from_file_info, NULL) && from_file_info > 0) {
        WriteFile(to_handle, buffer, from_file_info, &bytes_written, NULL);
    }

    CloseHandle(from_handle);
    CloseHandle(to_handle);
    printf("File copied successfully\n");
    return 0;
}