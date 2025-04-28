#include "syscall.h"

int main() {
    char file[1000];
    int f1, f2, i=0;
    int CloseReturnVal;
   
    f1 = Open("num_100.txt");
    PrintInt(f1);
    f1 = Open("test_write.txt");
    PrintInt(f1);
    f1 = Open("num_1000.txt");
    PrintInt(f1);
    for (;i < 21;i++){
	f2 = Open("num_100.txt");
    }
    
    // PrintInt(f1);
    CloseReturnVal = Close(f1);
  
    PrintInt(CloseReturnVal);
    CloseReturnVal = Close(f1);
    PrintInt(CloseReturnVal);
   
    CloseReturnVal = Close(100);
    PrintInt(CloseReturnVal);
    Halt();
    /* not reached */
}
