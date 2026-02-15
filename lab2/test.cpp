#include "EvryThng.h"

int main() {
    HANDLE h_out = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE h_err = GetStdHandle(STD_ERROR_HANDLE);
    
    PrintMsg(h_out, "Test 1: PrintMsg\n");
    PrintStrings(h_out, "Test 2: ", "PrintStrings ", "with ", "multiple ", "args\n", NULL);
    
    CHAR response[128];
    ConsolePrompt("Enter your name: ", response, 128, TRUE);
    PrintStrings(h_out, "Hello, ", response, "!\n", NULL);
    
    PrintMsg(h_out, "\nPassword test (no echo):\n");
    CHAR password1[128], password2[128];
    ConsolePrompt("Password: ", password1, 128, FALSE);
    PrintMsg(h_out, "\nConfirm password: ");
    ConsolePrompt("", password2, 128, FALSE);
    
    if (lstrcmpA(password1, password2) == 0){
        PrintMsg(h_out, "\nPasswords match.\n");
    } else {
        PrintMsg(h_err, "Error: Passwords do not match.\n");
    }
    
    return 0;
}