#include "syscall.h"

int main() {
    int fileID;
    int bytesRead;
    char buffer[20];

    fileID = Open("test_write.txt");
    bytesRead = Read(buffer, 20, fileID);
    //cout << "Bytes read : " << bytesRead << endl;
    Halt();   
}
