/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__

#include "kernel.h"

#include "synchconsole.h"

void SysHalt()
{
	kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
	return op1 + op2;
}

int SysCreate(char *filename, int filesize)
{
	// return value
	// 1: success
	// 0: failed
	return kernel->fileSystem->Create(filename, filesize);
}
OpenFileId SysOpen(char *filename)
{
	OpenFile* file = kernel->fileSystem->Open(filename);
	if (file) return 1;
	return 0;
}

int SysRead(char *buf, int size, OpenFileId id){
	return kernel->fileSystem->Read(buf, size, id);
}

int SysWrite(char *buf, int size, OpenFileId id){
	return kernel->fileSystem->Write(buf, size, id);
}

int SysClose(OpenFileId id){
	return kernel->fileSystem->Close(id);
}

#ifdef FILESYS_STUB
int SysCreate(char *filename)
{
	// return value
	// 1: success
	// 0: failed
	return kernel->interrupt->CreateFile(filename);
}
#endif

#endif /* ! __USERPROG_KSYSCALL_H__ */
