// filesys.h
//	Data structures to represent the Nachos file system.
//
//	A file system is a set of files stored on disk, organized
//	into directories.  Operations on the file system have to
//	do with "naming" -- creating, opening, and deleting files,
//	given a textual file name.  Operations on an individual
//	"open" file (read, write, close) are to be found in the OpenFile
//	class (openfile.h).
//
//	We define two separate implementations of the file system.
//	The "STUB" version just re-defines the Nachos file system
//	operations as operations on the native UNIX file system on the machine
//	running the Nachos simulation.
//
//	The other version is a "real" file system, built on top of
//	a disk simulator.  The disk is simulated using the native UNIX
//	file system (in a file named "DISK").
//
//	In the "real" implementation, there are two key data structures used
//	in the file system.  There is a single "root" directory, listing
//	all of the files in the file system; unlike UNIX, the baseline
//	system does not provide a hierarchical directory structure.
//	In addition, there is a bitmap for allocating
//	disk sectors.  Both the root directory and the bitmap are themselves
//	stored as files in the Nachos file system -- this causes an interesting
//	bootstrap problem when the simulated disk is initialized.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef FS_H
#define FS_H

#include "copyright.h"
#include "debug.h"  //just for test!!!
#include "openfile.h"
#include "sysdep.h"

#ifdef FILESYS_STUB  // Temporarily implement file system calls as
                     // calls to UNIX, until the real file system
                     // implementation is available
typedef int OpenFileId;

class FileSystem {
   public:
    FileSystem() {
        for (int i = 0; i < 20; i++) OpenFileTable[i] = NULL;
    }

    bool Create(char *name) {
        int fileDescriptor = OpenForWrite(name);

        if (fileDescriptor == -1)
            return FALSE;
        Close(fileDescriptor);
        return TRUE;
    }
    // The OpenFile function is used for open user program  [userprog/addrspace.cc]
    OpenFile *Open(char *name) {
        int fileDescriptor = OpenForReadWrite(name, FALSE);
        if (fileDescriptor == -1)
            return NULL;
        return new OpenFile(fileDescriptor);
    }

    //  The OpenAFile function is used for kernel open system call
    OpenFileId OpenAFile(char *name) {
	int i = 0;
        while (i < 20 && OpenFileTable[i] != NULL) {
            i++;
        }
        // file table is full.
        
       	if (i == 20) {
	    //cout << "File table is full." << endl;
	    return -1;
	}
	int fileDescriptor = OpenForReadWrite(name, FALSE);
        if (fileDescriptor == -1) { return -1;}
        for (int j = 0;j < 20;j++){
	    if (NameTable[j] != NULL && !strcmp(NameTable[j], name)){
		//cout << "File table is duplicated." << endl;
		return -1;
	    }
	}
	//cout << "File with id : " << i << " is " << fileDescriptor << endl;
	OpenFileTable[i] = new OpenFile(fileDescriptor);
        NameTable[i] = name;
        return i;	
    }
    int WriteFile(char *buffer, int size, OpenFileId id){
	if (id < 0 || id >= 20 || OpenFileTable[id] == NULL) return -1;
	int bytesWrite = OpenFileTable[id]->Write(buffer, size);
	//return OpenFileTable[id]->Write(buffer, size);
	if (bytesWrite >= 0) return bytesWrite;
        else return -1;
    }
    int ReadFile(char *buffer, int size, OpenFileId id){
	if (id < 0 || id >= 20 || OpenFileTable[id] == NULL) return -1;  

        int bytesRead = OpenFileTable[id]->Read(buffer, size);
        if (bytesRead >= 0) return bytesRead;  
        else return -1;  
    }
    int CloseFile(OpenFileId id){
	if(id < 0 || id >= 20 || OpenFileTable[id] == NULL) return -1;
        delete OpenFileTable[id];
        OpenFileTable[id] = NULL;
	NameTable[id] = NULL;
        return 1;
    }
    

    bool Remove(char *name) { return Unlink(name) == 0; }

    OpenFile *OpenFileTable[20];
    char* NameTable[20];
};

#else  // FILESYS
class FileSystem {
   public:
    FileSystem(bool format);  // Initialize the file system.
                              // Must be called *after* "synchDisk"
                              // has been initialized.
                              // If "format", there is nothing on
                              // the disk, so initialize the directory
                              // and the bitmap of free blocks.

    bool Create(char *name, int initialSize);
    // Create a file (UNIX creat)

    OpenFile *Open(char *name);  // Open a file (UNIX open)

    bool Remove(char *name);  // Delete a file (UNIX unlink)

    void List();  // List all the files in the file system

    void Print();  // List all the files and their contents

   private:
    OpenFile *freeMapFile;    // Bit map of free disk blocks,
                              // represented as a file
    OpenFile *directoryFile;  // "Root" directory -- list of
                              // file names, represented as a file
};

#endif  // FILESYS

#endif  // FS_H
