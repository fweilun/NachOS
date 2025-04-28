// scheduler.cc
//	Routines to choose the next thread to run, and to dispatch to
//	that thread.
//
// 	These routines assume that interrupts are already disabled.
//	If interrupts are disabled, we can assume mutual exclusion
//	(since we are on a uniprocessor).
//
// 	NOTE: We can't use Locks to provide mutual exclusion here, since
// 	if we needed to wait for a lock, and the lock was busy, we would
//	end up calling FindNextToRun(), and that would put us in an
//	infinite loop.
//
// 	Very simple implementation -- no priorities, straight FIFO.
//	Might need to be improved in later assignments.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "scheduler.h"
#include "copyright.h"
#include "debug.h"
#include "main.h"

//----------------------------------------------------------------------
// Scheduler::Scheduler
// 	Initialize the list of ready but not running threads.
//	Initially, no ready threads.
//----------------------------------------------------------------------

int CompareBurstTime(Thread* t1, Thread* t2){
    int bt1 = t1->getBurstTime();
    int bt2 = t2->getBurstTime();
    if (bt1 == bt2){
	bt1 = t1->getID();
	bt2 = t2->getID();
    }
    if (bt1 > bt2) return 1;
    else if (bt1 < bt2) return -1;
    return 0;
}

int CompareL2Priority(Thread* t1, Thread* t2) {
    int p1 = t1->getPriority();
    int p2 = t2->getPriority();

    if (p1 != p2) {
        return p2 - p1;  
    }

    return t1->getID() - t2->getID();
}

int getLevel(int priority){
    if (priority >= 100){
        return 1;
    }else if (priority >= 50){
        return 2;
    }else if (priority >= 0){
        return 3;
    }
    ASSERT(TRUE);
    return -1;
}


MultiLevelFeedBackQueue::MultiLevelFeedBackQueue(){
    l1ApproxBurst = 0;
    maxWaitTime = 1500;
    L1 = new SortedList<Thread*>(CompareBurstTime);
    L2 = new SortedList<Thread*>(CompareL2Priority);
    L3 = new List<Thread*>();
}

MultiLevelFeedBackQueue::~MultiLevelFeedBackQueue(){
    delete L1;
    delete L2;
    delete L3;
}

void MultiLevelFeedBackQueue::Append(Thread* item){
    int level = getLevel(item->getPriority());
    switch (level){
    	case 1:
	    L1->Insert(item);
	    break;
	case 2:
	    L2->Insert(item);
	    break;
	case 3:
	    L3->Append(item);
	    break;
    }
    DEBUG(dbgScheduler, "[A] Tick [" << kernel->stats->totalTicks  << "]: Thread [" << item->getID() << "] is inserted into queue L[" << level << "]");
}


bool MultiLevelFeedBackQueue::ShouldPreempt() {
    Thread* curThread = kernel->currentThread;
    int curLevel = getLevel(curThread->getPriority());
    if (curLevel == 3 && curThread->accTime >= L3TimeQuantum) {return TRUE;}    
    // curLevel < nextThread
    Thread* next = Front();
    if (!next) return FALSE;
    int nextLevel = getLevel(next->getPriority());    

    // nextLevel < curLevel then should be preempted.
    if (nextLevel < curLevel) return TRUE;
    if (nextLevel == curLevel && curLevel == 1) {
        return CompareBurstTime(curThread, next) == 1;
    }
    return FALSE;
}


bool MultiLevelFeedBackQueue::IsEmpty(){
    return L1->IsEmpty() && L2->IsEmpty() && L3->IsEmpty();
}

Thread* MultiLevelFeedBackQueue::Front(){
    Thread* selected = NULL;
    if (!L1->IsEmpty()){
	selected = L1->Front();
    }
    if (!selected && !L2->IsEmpty()){
	selected = L2->Front();
    }
    if (!selected && !L3->IsEmpty()){
	selected = L3->Front();
    }
    return selected;
}

static void agingForQueue(Thread* t){
    if (t->getStatus() != READY) return;
    int waitTime = kernel->stats->totalTicks - t->lastTick;
    // int oldLevel = getLevel(t->getPriority());
    int oldPriority = t->getPriority();
    if (waitTime >= 1500){
        t->lastTick = kernel->stats->totalTicks;
        t->setPriority(oldPriority + 10);
        DEBUG(dbgScheduler, "[C] Tick [" << kernel->stats->totalTicks  << "]: Thread ["<< t->getID()  << "] changes its priority from [" << oldPriority << "] to [" << t->getPriority() << "]");
    }
}

void MultiLevelFeedBackQueue::Aging(){
    Apply(agingForQueue);
    
    Thread * to_ch = NULL;
    for (ListIterator<Thread *> it(L3); ; it.Next()) {
	if (to_ch) {
	    L3->Remove(to_ch);
            Append(to_ch);
            to_ch = NULL;
    	}
    	if (it.IsDone()){
            break;
	}
    	if (getLevel(it.Item()->getPriority()) != 3){
            to_ch = it.Item();
	}
    }

    for (ListIterator<Thread *> it(L2); ; it.Next()) {
	if (to_ch){
	    L2->Remove(to_ch);
    	    Append(to_ch);
    	    to_ch = NULL;
	}
    
    	if (it.IsDone()){
	    break;
    	}
    	if (getLevel(it.Item()->getPriority()) != 2){
            to_ch = it.Item();
    	}
    }
}

Thread* MultiLevelFeedBackQueue::RemoveFront(){
    Thread* toBeRemoved;
    int level = -1;
    if (!L1->IsEmpty()){
	toBeRemoved = L1->RemoveFront();
	level = 1;
    }else if (!L2->IsEmpty()){
	toBeRemoved = L2->RemoveFront();
	level = 2;
    }else if (!L3->IsEmpty()){
	toBeRemoved = L3->RemoveFront();
	level = 3;
    }
    DEBUG(dbgScheduler, "[B] Tick [" << kernel->stats->totalTicks << "]: Thread [" << toBeRemoved->getID() << "] is removed from queue L[" << level << "]");
    ASSERT(toBeRemoved);
    return toBeRemoved;
}

void MultiLevelFeedBackQueue::Apply(void (*f)(Thread*)) const{
    L1->Apply(f);
    L2->Apply(f);
    L3->Apply(f);
}

int MultiLevelFeedBackQueue::getLevel(int priority){
    if (priority >= 100){
	return 1;
    }else if (priority >= 50){
	return 2; 
    }else if (priority >= 0){
	return 3;
    }
    ASSERT(TRUE);
    return -1;
}



Scheduler::Scheduler() {
    // readyList = new List<Thread *>;
    readyList = new MultiLevelFeedBackQueue();
    toBeDestroyed = NULL;
}

//----------------------------------------------------------------------
// Scheduler::~Scheduler
// 	De-allocate the list of ready threads.
//----------------------------------------------------------------------

Scheduler::~Scheduler() {
    delete readyList;
}

//----------------------------------------------------------------------
// Scheduler::ReadyToRun
// 	Mark a thread as ready, but not running.
//	Put it on the ready list, for later scheduling onto the CPU.
//
//	"thread" is the thread to be put on the ready list.
//----------------------------------------------------------------------

void Scheduler::ReadyToRun(Thread *thread) {
    ASSERT(kernel->interrupt->getLevel() == IntOff);
    DEBUG(dbgThread, "Putting thread on ready list: " << thread->getName());
    // cout << "Putting thread on ready list: " << thread->getName() << endl ;
    thread->lastTick = kernel->stats->totalTicks;
    thread->setStatus(READY);
    readyList->Append(thread);
}

//----------------------------------------------------------------------
// Scheduler::FindNextToRun
// 	Return the next thread to be scheduled onto the CPU.
//	If there are no ready threads, return NULL.
// Side effect:
//	Thread is removed from the ready list.
//----------------------------------------------------------------------

Thread *
Scheduler::FindNextToRun() {
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    if (readyList->IsEmpty()) {
        return NULL;
    } else {
        return readyList->RemoveFront();
    }
}
void Scheduler::Aging(){
    this->readyList->Aging();
}

//----------------------------------------------------------------------
// Scheduler::Run
// 	Dispatch the CPU to nextThread.  Save the state of the old thread,
//	and load the state of the new thread, by calling the machine
//	dependent context switch routine, SWITCH.
//
//      Note: we assume the state of the previously running thread has
//	already been changed from running to blocked or ready (depending).
// Side effect:
//	The global variable kernel->currentThread becomes nextThread.
//
//	"nextThread" is the thread to be put into the CPU.
//	"finishing" is set if the current thread is to be deleted
//		once we're no longer running on its stack
//		(when the next thread starts running)
//----------------------------------------------------------------------

void Scheduler::Run(Thread *nextThread, bool finishing) {
    Thread *oldThread = kernel->currentThread;

    ASSERT(kernel->interrupt->getLevel() == IntOff);

    if (finishing) {  // mark that we need to delete current thread
        ASSERT(toBeDestroyed == NULL);
        toBeDestroyed = oldThread;
    }

    if (oldThread->space != NULL) {  // if this thread is a user program,
        oldThread->SaveUserState();  // save the user's CPU registers
        oldThread->space->SaveState();
    }

    oldThread->CheckOverflow();  // check if the old thread
                                 // had an undetected stack overflow
    
    DEBUG(dbgScheduler, "[E] Tick [" << kernel->stats->totalTicks << "]: Thread [" << nextThread->getID() << "] is now selected for execution, thread [" << oldThread->getID() << "] is replaced, and it has executed [" << oldThread->accTime  << "] ticks");

    kernel->currentThread = nextThread;  // switch to the next thread
    nextThread->setStatus(RUNNING);      // nextThread is now running
    
    nextThread->updateLastTick(kernel->stats->totalTicks);
    DEBUG(dbgThread, "Switching from: " << oldThread->getName() << " to: " << nextThread->getName());

    // This is a machine-dependent assembly language routine defined
    // in switch.s.  You may have to think
    // a bit to figure out what happens after this, both from the point
    // of view of the thread and from the perspective of the "outside world".

    SWITCH(oldThread, nextThread);

    // we're back, running oldThread

    // interrupts are off when we return from switch!
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    DEBUG(dbgThread, "Now in thread: " << oldThread->getName());

    CheckToBeDestroyed();  // check if thread we were running
                           // before this one has finished
                           // and needs to be cleaned up

    if (oldThread->space != NULL) {     // if there is an address space
        oldThread->RestoreUserState();  // to restore, do it.
        oldThread->space->RestoreState();
    }
}

//----------------------------------------------------------------------
// Scheduler::CheckToBeDestroyed
// 	If the old thread gave up the processor because it was finishing,
// 	we need to delete its carcass.  Note we cannot delete the thread
// 	before now (for example, in Thread::Finish()), because up to this
// 	point, we were still running on the old thread's stack!
//----------------------------------------------------------------------

void Scheduler::CheckToBeDestroyed() {
    if (toBeDestroyed != NULL) {
        delete toBeDestroyed;
        toBeDestroyed = NULL;
    }
}

//----------------------------------------------------------------------
// Scheduler::Print
// 	Print the scheduler state -- in other words, the contents of
//	the ready list.  For debugging.
//----------------------------------------------------------------------
void Scheduler::Print() {
    cout << "Ready list contents:\n";
    readyList->Apply(ThreadPrint);
}
