// scheduler.h
//	Data structures for the thread dispatcher and scheduler.
//	Primarily, the list of threads that are ready to run.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "copyright.h"
#include "list.h"
#include "thread.h"

// The following class defines the scheduler/dispatcher abstraction --
// the data structures and operations needed to keep track of which
// thread is running, and which threads are ready but not running.

int compareBurstTime(Thread* t1, Thread* t2);
int CompareL2Priority(Thread* t1, Thread* t2);

class MultiLevelFeedBackQueue{
   public:
      MultiLevelFeedBackQueue();
      ~MultiLevelFeedBackQueue();
      
      Thread* Front();
      Thread* RemoveFront();
      void Append(Thread* item);
      void Aging();
      bool IsEmpty();
      bool ShouldPreempt();
      void Apply(void (*f)(Thread*)) const;

   protected:
	int getLevel(int priority);
      	int numInList;
	int l1ApproxBurst;
     	int L3TimeQuantum;
	int maxWaitTime;

	SortedList<Thread*>* L1;
     	SortedList<Thread*>* L2;
      	List<Thread*>* L3;
};


class Scheduler {
   public:
    Scheduler();   // Initialize list of ready threads
    ~Scheduler();  // De-allocate ready list

    void ReadyToRun(Thread* thread);
    // Thread can be dispatched.
    Thread* FindNextToRun();  // Dequeue first thread on the ready
                              // list, if any, and return thread.
    void Run(Thread* nextThread, bool finishing);
    // Cause nextThread to start running
    void CheckToBeDestroyed();  // Check if thread that had been
                                // running needs to be deleted
    void Print();               // Print contents of ready list
    
    bool ShouldPreempt(){return readyList->ShouldPreempt();}    
    void Aging();
    // SelfTest for scheduler is implemented in class Thread

   private:
    MultiLevelFeedBackQueue* readyList;
    Thread* toBeDestroyed;     // finishing thread to be destroyed
                               // by the next thread that runs
};

#endif  // SCHEDULER_H
