#include "syscall.h"

int main(){
    int fileID;
    fileID = Open("test_write.txt");
    Write("awjegflbewajflaewdawenfjeawnfjlewanfjawelfnbjeawlfbjewfbeawfjewab",20, fileID);
    Write("awjegflbewajflaewdawenfjeawnfjlewanfjawelfnbjeawlfbjewfbeawfjewab",20, fileID);
    Halt();
}

