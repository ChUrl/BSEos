#ifndef __preemptive_thread_include__
#define __preemptive_thread_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"
#include "lib/Semaphore.h"

class PreemptiveLoopThread : public Thread {
private:
    int id;
    Semaphore* sem;

    PreemptiveLoopThread(const PreemptiveLoopThread& copy) = delete;  // Verhindere Kopieren

public:
    // Gibt der Loop einen Stack und eine Id.
    PreemptiveLoopThread(int i, Semaphore* sem) : id(i), sem(sem) {}

    // Zaehlt einen Zaehler hoch und gibt ihn auf dem Bildschirm aus.
    void run() override;
};

class PreemptiveThreadDemo : public Thread {
private:
    PreemptiveThreadDemo(const PreemptiveThreadDemo& copy) = delete;  // Verhindere Kopieren

public:
    PreemptiveThreadDemo() {
        kout << "Initialized PreemptiveThreadDemo" << endl;
    }

    // Thread-Startmethode
    void run() override;
};

#endif