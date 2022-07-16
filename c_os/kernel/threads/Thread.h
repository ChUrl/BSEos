/*****************************************************************************
 *                                                                           *
 *                                 T H R E A D                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung eines kooperativen Thread-Konzepts.      *
 *                  Thread-Objekte werden vom Scheduler in einer verketteten *
 *                  Liste 'readylist' verwaltet.                             *
 *                                                                           *
 *                  Im Konstruktor wird der initialie Kontext des Threads    * 
 *                  eingerichtet. Mit 'start' wird ein Thread aktiviert.     *
 *                  Die CPU sollte mit 'yield' freiwillig abgegeben werden.  * 
 *                  Um bei einem Threadwechsel den Kontext sichern zu        *
 *                  koennen, enthaelt jedes Threadobjekt eine Struktur       *
 *                  ThreadState, in dem die Werte der nicht-fluechtigen      *
 *                  Register gesichert werden koennen.                       *
 *                                                                           *
 *                  Zusaetzlich zum vorhandenen freiwilligen Umschalten der  *
 *                  CPU mit 'Thread_switch' gibt es nun ein forciertes Um-   *
 *                  durch den Zeitgeber-Interrupt ausgeloest wird und in     *
 *                  Assembler in startup.asm implementiert ist. Fuer das     *
 *                  Zusammenspiel mit dem Scheduler ist die Methode          *
 *                  'prepare_preemption' in Scheduler.cc wichtig.            *
 *                                                                           *
 * Autor:           Michael, Schoettner, HHU, 16.12.2016                     *
 *****************************************************************************/

#ifndef __Thread_include__
#define __Thread_include__

#include "kernel/threads/ThreadState.h"
#include "user/lib/Logger.h"

class Thread {
private:
    Thread(const Thread& copy) = delete;  // Verhindere Kopieren

    unsigned int* stack;
    struct ThreadState regs;

protected:
    static Logger log;

public:
    unsigned int tid;  // Thread-ID (wird im Konstruktor vergeben)

    Thread();

    virtual ~Thread() {
        log << INFO << "Uninitialized thread, ID: " << dec << this->tid << endl;
        delete[] this->stack;
    }

    // Thread aktivieren
    void start();

    // Umschalten auf Thread 'next'
    void switchTo(Thread& next);

    // Methode des Threads, muss in Sub-Klasse implementiert werden
    virtual void run() = 0;
};

#endif
