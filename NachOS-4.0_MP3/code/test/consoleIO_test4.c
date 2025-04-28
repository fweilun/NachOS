#include "syscall.h"
int mm(int a){
	int i = 190;
	int j = 19;
	return i + j - a;
}

int nn(int a){
	int i = 130;
	int j = 18;
	return i + j - a;
}
int n1(int a){
	int i = 230;
	int j = 18;
	return i + j - a;
}

int n2(int a){
	int i = 130;
	int j = 138;
	return i + j - a;
}
int n3(int a){
	int i = 130;
	int j = 28;
	return i + j - a;
}

int nn2(int a){
	int i = 130;
	int j = 18;
	return i + j - a;
}
int n11(int a){
	int i = 230;
	int j = 18;
	return i + j - a;
}

int n22(int a){
	int i = 130;
	int j = 138;
	return i + j - a;
}
int n33(int a){
	int i = 130;
	int j = 28;
	return i + j - a;
}

int nn22(int a){
	int i = 130;
	int j = 18;
	return i + j - a;
}
int n121(int a){
	int i = 230;
	int j = 18;
	return i + j - a;
}

int n222(int a){
	int i = 130;
	int j = 138;
	return i + j - a;
}
int n323(int a){
	int i = 130;
	int j = 28;
	return i + j - a;
}
void shift(char *start, char *end){
	int store = *start;
	while (start != end){
		*start = *(start+1);
		start++;
	}
	*end = store;
}
int nwn22(int a){
	int i = 130;
	int j = 18;
	return i + j - a;
}
int n1w21(int a){
	int i = 230;
	int j = 18;
	return i + j - a;
}

int n2w22(int a){
	int i = 130;
	int j = 138;
	return i + j - a;
}
int n3w23(int a){
	int i = 130;
	int j = 28;
	return i + j - a;
}
int main() {
	int i;

	for (i = 15; i <= 19; i++) {
		PrintInt(i);
	}
	for (i = 24; i <= 100; i++) {
		PrintInt(i);
	}
	for (i = 15; i <= 19; i++) {
		PrintInt(i);
	}
	for (i = 24; i <= 100; i++) {
		PrintInt(i);
	}
	

	return 0;
}

