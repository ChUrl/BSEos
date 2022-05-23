/*****************************************************************************
 *                                                                           *
 *                         I N T D I S P A T C H E R                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Zentrale Unterbrechungsbehandlungsroutine des Systems.   *
 *                  Der Parameter gibt die Nummer des aufgetretenen          *
 *                  Interrupts an. Wenn eine Interrupt Service Routine       *
 *                  registriert ist, wird diese aufgerufen.                  *
 *                                                                           *
 * Autor:           Michael Schoettner, 31.8.2016                            *
 *****************************************************************************/
#include "kernel/interrupts/IntDispatcher.h"
#include "kernel/CPU.h"
#include "kernel/Globals.h"

extern "C" void int_disp(unsigned int slot);

/*****************************************************************************
 * Prozedur:        int_disp                                                 *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Low-Level Interrupt-Behandlung.                          *
 *                  Diese Funktion ist in der IDT fuer alle Eintraege einge- *
 *                  tragen. Dies geschieht bereits im Bootloader.            *
 *                  Sie wird also fuer alle Interrupts aufgerufen. Von hier  *
 *                  aus sollen die passenden ISR-Routinen der Treiber-Objekte*
 *                  mithilfe von 'IntDispatcher::report' aufgerufen werden.  *
 * Parameter:                                                                *
 *      vector:     Vektor-Nummer der Unterbrechung                          *
 *****************************************************************************/
void int_disp(unsigned int vector) {

    /* hier muss Code eingefuegt werden */

    // kout << "Ein Interrupt ist aufgetreten (vector: " << vector << ")" << endl;
    intdis.report(vector);
}

/*****************************************************************************
 * Konstruktor:     IntDispatcher::IntDispatcher                             *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Initialisierung der ISR map mit einer Default-ISR.       *
 *****************************************************************************/
IntDispatcher::IntDispatcher() {
    for (unsigned int slot = 0; slot < size; slot++) {
        map[slot] = 0;
    }
}

/*****************************************************************************
 * Methode:         IntDispatcher::assign                                    *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Registrierung einer ISR.                                 *
 *                                                                           *
 * Parameter:                                                                *
 *      vector:     Vektor-Nummer der Unterbrechung                          *
 *      isr:        ISR die registriert werden soll                          *
 *                                                                           *
 * Rueckgabewert:   0 = Erfolg, -1 = Fehler                                  *
 *****************************************************************************/
int IntDispatcher::assign(unsigned int vector, ISR& isr) {

    /* hier muss Code eingefuegt werden */

    if (vector >= this->size) {
        if (DEBUG) kout << "Invalid vector number when assigning" << endl;
        return -1;
    }

    this->map[vector] = &isr;
    if (DEBUG) kout << "Registered ISR for vector " << dec << vector << endl;

    return 0;
}

/*****************************************************************************
 * Methode:         IntDispatcher::report                                    *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Eingetragene ISR ausfuehren.                             *
 *                                                                           *
 * Parameter:                                                                *
 *      vector:     Gesuchtes ISR-Objekt fuer diese Vektor-Nummer.           *
 *                                                                           *
 * Rueckgabewert:   0 = ISR wurde aufgerufen, -1 = unbekannte Vektor-Nummer  *
 *****************************************************************************/
int IntDispatcher::report(unsigned int vector) {

    /* hier muss Code eingefuegt werden */

    if (vector >= this->size) {
        return -1;
    }

    ISR* isr = this->map[vector];

    if (isr == 0) {
        if (DEBUG) kout << "No ISR registered for vector " << vector << endl;
        return -1;
    }

    isr->trigger();

    return 0;
}
